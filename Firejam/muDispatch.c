#ifndef MUDISPATCH_C
#define MUDISPATCH_C

#include "muDispatch.h"
#include "muSoundDriver.c"
#include "midiLUT.c"
#include "muWindow.c"
#include "muGraphics.c"

struct gCurrentSound gCurSnd;


void prepDispatch()
{
gCurSnd.gChoice = 1; //global sound output choice. 0=MIDI, 1=SW, 2=FT, 3=FF
gCurSnd.gChosenFF = 4; //global for selecting the freeform wave element
gCurSnd.midiChannel = 0;
gCurSnd.midiInst = 1;
gCurSnd.detectChar = 0; //no key detected at first
gCurSnd.swPoly = 0;
gCurSnd.countFT = 0;
gCurSnd.ffPoly = 0;
gCurSnd.lastMIDI = 0;
gCurSnd.volume = 5;
gCurSnd.octShift=0;
}

unsigned char minOf(unsigned char a, unsigned char b)
{
if(a>b) return b;
else return a;
}

void dispatch(unsigned char lastCmd, 
			  unsigned char lastNote, 
			  unsigned char lastVelo)
{

PianoStates **ps;
CDEFProcPtr proc;

ps = (PianoStates**)((*cPiano)->contrlData);
HLock((Handle)ps);
//HLock((*cPiano)->contrlDefProc);
proc = (CDEFProcPtr)*((*cPiano)->contrlDefProc);



if((lastCmd&0xF0)==0x90) 
	{
	(*ps)->noteOn[lastNote] = 1;
	proc(0, cPiano, 100, (long)lastNote);
	}
else if((lastCmd&0xF0)==0x80) 
	{
	(*ps)->noteOn[lastNote] = 0;
	proc(0, cPiano, 100, (long)lastNote);
	}

HUnlock((*cPiano)->contrlDefProc);
HUnlock((Handle)ps);


		
switch(gCurSnd.gChoice)
	{
	case 0:	//midi out
	 	midiNoteSend(lastCmd, lastNote, lastVelo);
		break;
	case 1: //sw
		swNoteSend(lastCmd, lastNote, minOf(lastVelo,((gCurSnd.volume+1)<<4)-1));
		break;
	case 2: //ft
		ftNoteSend(lastCmd, lastNote, lastVelo, gCurSnd.ptrWave);
		break;
	case 3: //ff
		ffNoteSend(lastCmd, lastNote, lastVelo, 
					sHands[gCurSnd.gChosenFF], 
					sBufLen[gCurSnd.gChosenFF], 
					sRefNote[gCurSnd.gChosenFF]);
		break;
	}		    	
}

void purgeBuffer() //call this when you want to start play mode again
{
unsigned char b[16];
long readByte,nbBytes;

tail=0;
head=0;
runningStatus = 0;

SerGetBuf(gInputRefNum, &nbBytes);
while(nbBytes > 0)
	{
	readByte = 1L;
	FSRead(gInputRefNum, &readByte, b);
	nbBytes--;
	}

 	gCurSnd.swPoly=0;
 	gCurSnd.ffPoly=0;
 	gCurSnd.countFT=0;
 	ftPoly[0]=0;
 	ftPoly[1]=0;
 	ftPoly[2]=0;
 	ftPoly[3]=0;
 	if(!SoundDone()) StopSound();
}
#endif