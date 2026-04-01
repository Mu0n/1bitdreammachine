#ifndef MUSTATES_C
#define MUSTATES_C

#include "muStates.h"
#include "midiInLoop.c"
#include "muDispatch.h"
#include "muWindow.h"


void setOutputMode(short which)
{
(*bSquare)->active = (which==1)?true:false;
(*b4Tones)->active = (which==2)?true:false;
(*bFreeform)->active = (which==3)?true:false;
(*bMIDI)->active = (which==4)?true:false;

}

void getFTGoing()
{
if(gCurSnd.gChoice == 2) 
	{
	if(SoundDone())StartSound(&myRec,256,nil);
	resetFT();
	}
}
void startPlayState()
{
HiliteControl(bPlay,1);
purgeBuffer();

FlushEvents(everyEvent,0);
disableRegControls();
goMIDITextInstr(true);

if(!SoundDone()) StopSound();
getFTGoing();

midiInLoop();
if(!SoundDone()) StopSound();

HiliteControl(bPlay,0);
enableRegControls();
goMIDITextInstr(false);
FlushEvents(everyEvent,0);

}

void lowerVolume()
{
if(gCurSnd.volume > 0) 
	{
	gCurSnd.volume--;
	SetSoundVol(gCurSnd.volume);
	updateVolText();
	soundTest();
	}
}

void increaseVolume()
{
if(gCurSnd.volume < 7) 
	{
	gCurSnd.volume++;
	SetSoundVol(gCurSnd.volume);
	updateVolText();
	soundTest();
	}
}

void soundTest()
{
swNoteSend(0x90, 0x3C, gCurSnd.volume<<4);
Delay(10,nil);
swNoteSend(0x80, 0x3C, gCurSnd.volume<<4);
}
#endif