//
//	Copyright (c) 2026 Michael Juneau, anybitfeverdreams@gmail.com
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//	SOFTWARE.
//
//    FireJam
//  C program that takes in MIDI IN signals and activates the old Sound Driver
//  to let you play real time notes with the square wave synth, 4-tone, etc.
//
// version 0.5 - 2026

#include <Sound.h>

#include "Serial.h"
#include "GestaltEqu.h"
#include "Timer.h"
#include <Files.h>
#include "muMIDIport.c"
#include "MIDIlut.c"
#include "muSoundDriver.c"
#include "muWindow.c"
#include "muMenu.c"
#include "muGraphics.c"
#include "muEvents.c"
#include "muDispatch.c"
#include "math.h"
#include "PopUp.c"

#define kQ 0x0D

#define sBeepID         1
#define sIndigoID       7
#define sEepID          8
#define sSosumiID       9
#define sStepID         200
#define sDizzyID        202
#define sFlatID 		204
#define sSFXID          205

#define NB_FFS          9

// *** global variables ***

OSErr gOSErr; //fetches error codes during mac file reading operations
Boolean isHit = 0; //for piano key hit detection
extern Boolean gDone = 0; //global flag for quitting the loop

Handle swHand, ftHand;

Wave *ptrWave; //for passing as argument, which FT instrument is loaded as main one
//freeform section
long   sBufLen[10]; //array of buffer lengths
Handle sHands[10]; //array of sound Handles, 8 prebuilt, 1 free to load
short  sRefNote[10]; //array of what the sound needs to be as ref note

// ***function declarations ***
Boolean mainLoop(void);
void MySendNote(char whichNote); //sends a test note of default volume and pressure according to the note pitch byte of the argument

void prepFFSounds(void);
extern void midiInLoop(void);


// *** function definitions ***

void myInitStuff()
	{
	InitGraf(&qd.thePort); //qd is the quickdraw global, lets you access thePort (viewable area)
	InitFonts(); //Do this to mess with text and fonts
	InitWindows(); //A must if you plan on drawing on controlled window surfaces as opposed to the raw screen
	InitMenus(); //Do this to mess with menus
	TEInit(); //Text Editing (for inputs)
	InitDialogs(nil); //Dialog manager for alerts, dialogs, etc. nil = no return resumeProc
	InitCursor(); //Cursor manager, if you don't do this, you'll be stuck with the waiting cursor (watch icon or more modern ones)
	GetDateTime((unsigned long*)(&qd.randSeed)); //if you ever need random numbers, this will prep it with a unique time related seed that changes every launch
	FlushEvents(everyEvent,0); //Do this to prevent queued events from messing with your app right at launch.
	}
	
	
//sends a test note of default volume and pressure according to the note pitch byte of the argument
void MySendNote(char whichNote)
	{
	char myMessage[3]={0x90,0x00,0x64};
	myMessage[1]=whichNote;
	myParamBlock.ioParam.ioBuffer = myMessage;
	
	gOSErr = PBWrite(myPBPtr, FALSE);
	}
	
void prepFFSounds()
{
sHands[0] = (Handle)loadFFSound(sBeepID, &sBufLen[0]);
sHands[1] = (Handle)loadFFSound(sIndigoID, &sBufLen[1]);
sHands[2] = (Handle)loadFFSound(sEepID, &sBufLen[2]);
sHands[3] = (Handle)loadFFSound(sSosumiID, &sBufLen[3]);
sHands[4] = (Handle)loadFFSound(sStepID, &sBufLen[4]);
sHands[5] = (Handle)loadFFSound(sDizzyID, &sBufLen[5]);
sHands[6] = (Handle)loadFFSound(sFlatID, &sBufLen[6]);
sHands[7] = (Handle)loadFFSound(sSFXID, &sBufLen[7]);

sRefNote[0]=0x41;
sRefNote[1]=0x3C;
sRefNote[2]=0x3C;
sRefNote[3]=0x41;
sRefNote[4]=0x3F;
sRefNote[5]=0x43;
sRefNote[6]=0x43;
sRefNote[7]=0x3C;

sRefNote[8]=0x0F;
gCurSnd.ffPoly=0;
}

void releaseFFSounds()
{
int i;
for(i=0; i< NB_FFS-1; i++) ReleaseResource((Handle)sHands[i]);
}



Boolean mainLoop()
{
while(!gDone)
	{
	eventPass();
	}
}
	
void main()
	{
	Boolean isDone = false; //condition for quitting the program
	short oldEventMask;
	short oldVolume;
	GrafPtr oldPort;
	
	myInitStuff();
	
	GetPort(&oldPort);
	
	MaxApplZone(); 
	MoreMasters(); //a mac strategy - bumps up the reserved space for master pointers. doubtful if really necessary
	MoreMasters();
	MoreMasters();
	MoreMasters();
	prepWindow();
	
	
	GetSoundVol(&oldVolume);
	SetSoundVol(5);
	
    menuBarInit(); //done in muMenu.c
	
	setupMIDISerial();
	prepDispatch();
	
	if(!SoundDone()) StopSound();
	
	prepSWSynth(); //prepare the square-wave synth
    prepFTWaves();//prepare the 256 byte Waves: sinWaves, triWaves, sawWaves, squWaves
	              //and emptyWaves
    gCurSnd.ptrWave = &sinWave; //select the first instrument for FT  
	prepFFSounds(); //prepare the freeform loaded snd resources
	
	LoadGfxAssets();
	DrawAll();
	SetOutputMenu(iSW);
	
	oldEventMask = SysEvtMask;
	SetEventMask(everyEvent); //ditching all 4 app masks
	
	mainLoop();
		
	//MyCloseSerialDriver();
	//releaseFFSounds();
	//return; //Go back to Finder
	
	SetEventMask(oldEventMask);
	SetSoundVol(oldVolume);
	FlushEvents(everyEvent,0); //Do this to prevent queued events from messing with your app right at launch.
	SetPort(oldPort);
	}
	
