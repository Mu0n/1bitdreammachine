#ifndef MUMENU_C
#define MUMENU_C

/*muMenu.c
* VERSION 1.0.0 - March 2026
* Manages the menus
*/

#include "muMenu.h"
#include "midiInLoop.h"
#include "muMIDIport.h"
#include "muStates.c"

short openedOnce;

void menuBarInit()
{
Handle menuBar;
MenuHandle menu;
ControlHandle control;
OSErr myErr;
long feature;

menuBar = GetNewMBar(kBaseMenuID);
SetMenuBar(menuBar);

menu = GetMenu(mApple);
AddResMenu(menu, 'DRVR');

DrawMenuBar();

openedOnce = 0;

}

	
void HandleMenuChoice(long menuChoice)
{
short menu;
short item;

if(menuChoice !=0)
	{
	menu = HiWord(menuChoice);
	item = LoWord(menuChoice);
	
	switch(menu)
		{
		case mApple:
			HandleAppleChoice(item);
			break;
		case mFile:
			HandleFileChoice(item);
			break;
		case mEdit:
			break;
		case mMidi:
			HandleMIDIChoice(item);
			break;
		case mOutput:
			HandleOutputChoice(item);
			break;
		case mMIDIInst:
			midiChangeProgram(0x00, item-1);
			CheckItem(GetMHandle(menu),gCurSnd.midiInst,false);
			gCurSnd.midiInst = item;
			CheckItem(GetMHandle(menu),gCurSnd.midiInst,true);
		    break;
		}
	}
	HiliteMenu(0);
}


void HandleAppleChoice(short item)
{
MenuHandle appleMenu;
Str255 accName;
short accNumber;

switch(item)
	{
	case iAbout:
		showAbout();
		break;
	default:
		appleMenu = GetMHandle(mApple);
		GetItem(appleMenu, item, accName);
		accNumber = OpenDeskAcc(accName);
		break;
	}
}

void HandleFileChoice(short item)
{
switch(item)
	{
	case iPlay:
		startPlayState();
		break;
	case iOpen:
		openSndFile();
		break;
	case iQuit:
		gDone = true;
		break;
	}
}

void openSndFile()
{
  SFReply reply;
  OSErr err;
  short fRefNum = 0;
  Point dialogPos={30,30};
  long count;
  SFTypeList myTypes;
  long bytesSS = 0L;
  
  myTypes[0]='FSSD';
  myTypes[1]='snd ';
  

  fRefNum = OldLoadFile(myTypes);
  if(openedOnce == 0) 
  	{
  	    
	if(fRefNum !=0)
  		{
  	studioSessionRead(fRefNum, &bytesSS, &sHands[8]);  
  	FSClose(fRefNum);	
  	
    sBufLen[8]=bytesSS;
    sRefNote[8]=0x3C;
    gCurSnd.gChosenFF=8;
  
  	openedOnce = 1;
  		}
  	}
  
  
}

void studioSessionRead(short file, long *bufferSize, Handle *hand)
{
long sizeToRead = 13226L;

*bufferSize = sizeToRead;


//if(hand != NULL) DisposHandle((Handle)hand);   
*hand = NewHandleClear(sizeToRead);
HLock((Handle)*hand);
FSRead(file,bufferSize,*(*hand));
HUnlock((Handle)*hand);
}

short OldLoadFile(SFTypeList myTypes)
  {
  SFReply reply;      //GetFile reply
  short openResult=0; //trails file number
  long count;         //used to read the data
  Point where={50,50};//SFGetFile dialog location
  
  SFGetFile(where,"\pSelect a file",0L,2,myTypes,0L,&reply);
  
  if(reply.good) FSOpen(reply.fName,reply.vRefNum,&openResult);
  return openResult;
  }
  
  
void HandleMIDIChoice(short item)
{
switch(item)
	{
	case iModem:
		SetModemPrinter(iModem);
		break;
	case iPrinter:
		SetModemPrinter(iPrinter);
		break;
	case iTest:
		SetModemPrinter(iTest);
		break;
	}
}

void SetModemPrinter(short item)
{
MenuHandle menu;

menu = GetMHandle(mMidi);
if(item == iModem)
	{
	CheckItem(menu, iModem, true);
	CheckItem(menu, iPrinter, false);
	}
if(item == iPrinter)
	{
	CheckItem(menu, iModem, false);
	CheckItem(menu, iPrinter, true);
	}
if(item == iTest)
	{
	midiNoteSend(0x90, 0x3C, 0x7F);
	}
}			
void SetFFItem(short item)
{
int i=0;
MenuHandle menu;

menu = GetMHandle(mOutput);
for(i=i_start_ff; i<= i_end_ff; i++)
	{
		if(i==item) 
		{
		CheckItem(menu, i, true);
		}
	else 
		{
		CheckItem(menu, i, false);
		}
	}
}

void SetFTItem(short item)
{
int i=0;
MenuHandle menu;

menu = GetMHandle(mOutput);
for(i=i_start_ft; i<= i_end_ft; i++)
	{
	if(i==item) 
		{
		CheckItem(menu, i, true);
		}
	else 
		{
		CheckItem(menu, i, false);
		}
	}
}
void SetOutputMenu(short choice)
{
MenuHandle menu;
int i=0;

menu = GetMHandle(mOutput);
for(i=1; i<=4; i++)
	{
	if(i==choice) CheckItem(menu, i, true);
	else CheckItem(menu,i,false);
	}
for(i=i_start_ft; i<= i_end_ff; i++)
	{
	if(choice==iFT && i >= i_start_ft && i <= i_end_ft) 
		{
		EnableItem(menu, i);
		continue;
		}
	if(choice==iFF && i >= i_start_ff) 
		{
		EnableItem(menu, i);
		continue;
		}
	DisableItem(menu,i);
	}
}
void HandleOutputChoice(short item)
{
Rect bOutputAll;
SetRect(&bOutputAll, (*bSquare)->r.left, (*bSquare)->r.top, 
                     (*bMIDI)->r.right, (*bMIDI)->r.bottom);

switch(item)
	{
	case iSW:
		gCurSnd.gChoice = 1;
		setOutputMode(1);
		SetOutputMenu(iSW);
		InvalRect(&bOutputAll);
		break;
	case iFT:
		gCurSnd.gChoice = 2;
		setOutputMode(2);
		getFTGoing();
		SetOutputMenu(iFT);
		InvalRect(&bOutputAll);
		break;
	case iFF:
		gCurSnd.gChoice = 3;
		setOutputMode(3);
		SetOutputMenu(iFF);
		InvalRect(&bOutputAll);
		break;
	case iMD:
		gCurSnd.gChoice = 0;
		setOutputMode(4);
		SetOutputMenu(iMD);
		InvalRect(&bOutputAll);
		break;
	case iSinus:
		gCurSnd.ptrWave = &sinWave;
		SetFTItem(iSinus);
		break;
	case iSquare:
		gCurSnd.ptrWave = &squWave;
		SetFTItem(iSquare);
		break;
	case iTriangle:
		gCurSnd.ptrWave = &triWave;
		SetFTItem(iTriangle);
		break;
	case iSawTooth:
		gCurSnd.ptrWave = &sawWave;
		SetFTItem(iSawTooth);
		break;
	case iEep:
		gCurSnd.gChosenFF = 2;
		SetFFItem(iEep);
		break;
	case iDizzy:
		gCurSnd.gChosenFF = 5;
		SetFFItem(iDizzy);
		break;
	case iIndigo:
		gCurSnd.gChosenFF = 1;
		SetFFItem(iIndigo);
		break;
	case iSosumi:
		gCurSnd.gChosenFF = 3;
		SetFFItem(iSosumi);
		break;
	case iBeep:
		gCurSnd.gChosenFF = 0;
		SetFFItem(iBeep);
		break;
	}
}

#endif