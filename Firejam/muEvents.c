#ifndef MUEVENTS_C
#define MUEVENTS_C

#include "muEvents.h"
#include "muGraphics.h"
#include "muMenu.h"
#include "muSoundDriver.h"
#include "midiLUT.h"
#include "muDispatch.h"
#include "midiInLoop.h"
#include "muWindow.h"
#include "muStates.h"
#include "PopUp.h"

EventRecord event;
Boolean gClickUsed = false;
short backedMouseNote=0;
Boolean mousePianoTracking = false;
short currentMouseNote = -1;
	
void eventPass()
	{
	short part;
	Point pt;
	unsigned char lastCmd=0, lastNote=0, lastVelo=0; //for MIDI in detection
	Boolean isDone = false;
	Boolean isHit=false;
	ControlHandle control;
	
	unsigned char theChar;	
	
	
	//if(WaitNextEvent(everyEvent, &event, 1, NULL)!=FALSE)
	if(GetNextEvent(everyEvent, &event) == TRUE)
	{
	switch(event.what)
		{
		case mouseDown:
			HandleMouseDown(&event, &mousePianoTracking, &currentMouseNote);	
			break;
		case mouseUp:
			HandleMouseUp(&event, &mousePianoTracking, &currentMouseNote);
			break;
		case keyDown:
			theChar = event.message & charCodeMask;
			if(theChar==32) startPlayState();
			if(  (event.modifiers & cmdKey) != 0 )	HandleMenuChoice(MenuKey(theChar));
			break;
		case keyUp:
			theChar = event.message & charCodeMask;
			break;
		case updateEvt:
			DoUpdate(&event);
			break;
		} //end of switch
	}		
//handles mouse glissando across notes while the button is held down


	if(mousePianoTracking)
		{
		if(StillDown())
			{
			GetMouse(&pt);
					
			part = FindControl(pt, muWinPtr, &control);
					
			if(control != cPiano)
				{
				dispatch(0x80, currentMouseNote, 0x7F);
				//currentMouseNote = -1;
				}
						
			else if(control == cPiano && (part-107) != currentMouseNote)
				{
				if(currentMouseNote >=0)
					{
					dispatch(0x80, currentMouseNote, 0x7F);
					}
				if(part-107 >=0 && (part-107)<=128)
					{
					dispatch(0x90,  part-107,  0x7F);
					currentMouseNote = part-107;
					}
				} //end of if control is cPiano
			}	//end of StillDown		
		} //end mousePianoTracking
	
	
// MIDI in piano controller events	
	midiNoteDetect(&lastCmd, &lastNote, &lastVelo, &isHit, &isDone);
	

	} //end of function

void HandleMouseDown(EventRecord *eventPtr, Boolean *mousePianoTracking, short *currentNote)
	{
	WindowPtr whichWindow;
	GrafPtr oldPort;
	short thePart,theCPart;
	long menuChoice;
	ControlHandle control;
	short cdefCode, sVolValue = 0, i;
	Point localMouse, popUpUpperLeft;
	Str255  str;
	
	thePart = FindWindow(eventPtr->where, &whichWindow);
	
	switch(thePart)
		{
		case inMenuBar:
			menuChoice = MenuSelect(eventPtr->where);
			HandleMenuChoice(menuChoice);
			break;
		case inSysWindow:
			SystemClick(eventPtr, whichWindow);
			break;
		case inContent:
			SetPort(whichWindow);
	
			localMouse = eventPtr->where;
			GlobalToLocal(&localMouse);	

			//check the 4 output buttons
			if(PtInRect(localMouse,&((*bSquare)->r))) HandleOutputChoice(1);
			if(PtInRect(localMouse,&((*b4Tones)->r))) HandleOutputChoice(2); 
			if(PtInRect(localMouse,&((*bFreeform)->r))) HandleOutputChoice(3); 
			if(PtInRect(localMouse,&((*bMIDI)->r))) HandleOutputChoice(4); 

			//interact with other standard controls
			theCPart = FindControl(localMouse, whichWindow, &control);
			if(control != NULL)
				{
				if(control == bPlay)
					{
					startPlayState();
					}
				if(control == bVolPlus)
					{
					increaseVolume();
					}
				if(control == bVolMinus)
					{
					lowerVolume();
					}
				if(control == bLowerOct)
					{
					if(gCurSnd.octShift>-1) gCurSnd.octShift--;
					updateVolKeybRange();
					}
				if(control == bIncrOct)
					{
					if(gCurSnd.octShift<2) gCurSnd.octShift++;
					updateVolKeybRange();
					}
						
				if(control == cPiano)
					{
					*mousePianoTracking = true;
					
					//cdefCode = TrackControl(control, localMouse, nil);
					if(theCPart !=0) 
						{
						if(SoundDone() && gCurSnd.gChoice == 2) StartSound(&myRec,256,nil);
						dispatch(0x90, theCPart-107, 0x7F);
						*currentNote = theCPart-107;
						}
					}
		
				}
			break;
		
		} //end switch
	} //end function

void HandleMouseUp(EventRecord *eventPtr, Boolean *mousePianoTracking, short *currentNote)
{

	WindowPtr whichWindow;
	GrafPtr oldPort;
	short thePart,theCPart;
	ControlHandle control;
	short cdefCode;
	Point localMouse;
	
	thePart = FindWindow(eventPtr->where, &whichWindow);
	
	switch(thePart)
		{
		case inContent:
			SetPort(whichWindow);
	
			localMouse = eventPtr->where;
			GlobalToLocal(&localMouse);	
			
			theCPart = FindControl(localMouse, whichWindow, &control);

			if(control == cPiano)
				{
				if(*mousePianoTracking)
					{
					if(*currentNote >= 0)
						{
						dispatch(0x80, *currentNote, 0x7F);
						}
					}
				*currentNote = -1;
				*mousePianoTracking = false;
				}
	
	
			break;
		} //end switch
		
}
void DoUpdate(EventRecord *eventPtr)
{	

WindowPtr window;

window = (WindowPtr)eventPtr->message;
BeginUpdate(window);
DrawAll();
DrawControls(window);
//DrawPopUp(gPopup);
EndUpdate(window);
}

#endif