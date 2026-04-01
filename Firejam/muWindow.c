#ifndef MUWINDOW_C
#define MUWINDOW_C

/*muWindow.c
* VERSION 1.0.0 - March 2026
* Manages the main window
*/

#include "muWindow.h"
#include "PopUp.h"


Rect muWinRect;
WindowPtr muWinPtr;
DialogPtr muDialog;

ControlHandle bPlay;
ControlHandle bVolPlus, bVolMinus;
ControlHandle bLowerOct, bIncrOct;
ControlHandle cPiano;


PopUpPtr gPopup;
pictButtonH bSquare, b4Tones, bFreeform, bMIDI;


disableRegControls()
{
HiliteControl(bVolPlus,255);
HiliteControl(bVolMinus,255);
HiliteControl(bLowerOct,255);
HiliteControl(bIncrOct,255);
}


enableRegControls()
{
HiliteControl(bVolPlus,0);
HiliteControl(bVolMinus,0);
HiliteControl(bLowerOct,0);
HiliteControl(bIncrOct,0);
}
void prepWindow()
{
int i=0;
Rect r, popupRect = {50,10,69,200};
Str255 name;
MenuHandle thePopupMenu;

muWinPtr = GetNewWindow(kBaseWinID, nil, kMoveToFront);

//SetResLoad(true);

SetRect(&r,22,200,542,280);
bPlay = GetNewControl(bPlayID, muWinPtr);
bVolPlus = GetNewControl(bVolPlusID, muWinPtr);
bVolMinus = GetNewControl(bVolMinusID, muWinPtr);
bLowerOct = GetNewControl(bLowerOctID, muWinPtr);
bIncrOct = GetNewControl(bIncrOctID, muWinPtr);

cPiano = NewControl(muWinPtr, &r,"\p",true,128,0,127,2048,0L);


//General MIDI popup menu in the main window
thePopupMenu = GetMenu(133);

HLock((Handle)thePopupMenu);
for(i=1; i<=128; i++)
	{
	GetIndString(name, 128, i);
	AppendMenu(thePopupMenu,name);
	}
InsertMenu(thePopupMenu,0);
//gPopup = CreatePopUp(muWinPtr, &popupRect, thePopupMenu, kPopUpStyleDefault);
HUnlock((Handle)thePopupMenu);
CheckItem(thePopupMenu, 1, true);

buildOutputButtons();

SetPort(muWinPtr);
ShowWindow(muWinPtr);
}

void buildOutputButtons()
{
Rect r = {30,30,71,91};
//Populate the 4 custom PICT using buttons
bSquare = (pictButtonH)NewHandle(sizeof(pictButtonRec));
b4Tones = (pictButtonH)NewHandle(sizeof(pictButtonRec));
bFreeform = (pictButtonH)NewHandle(sizeof(pictButtonRec));
bMIDI = (pictButtonH)NewHandle(sizeof(pictButtonRec));

(*bSquare)->pic = GetPicture(130);
HLock((Handle)(*bSquare)->pic);
(*b4Tones)->pic = GetPicture(131);
HLock((Handle)(*b4Tones)->pic);
(*bFreeform)->pic = GetPicture(132);
HLock((Handle)(*bFreeform)->pic);
(*bMIDI)->pic = GetPicture(133);
HLock((Handle)(*bMIDI)->pic);

(*bSquare) -> active = true;
(*b4Tones) -> active = false;
(*bFreeform) -> active = false;
(*bMIDI) -> active = false;

(*bSquare)->r = r;
OffsetRect(&r,70,0);
(*b4Tones) ->r = r;
OffsetRect(&r,-70,50);
(*bFreeform) ->r = r;
OffsetRect(&r,70,0);
(*bMIDI)->r = r; 

}


void showAbout()
{
DialogPtr aboutDialog;
short itemHit;
VersRec *v;
Str255 versionName;

Handle h = GetResource('vers', 128);
HLock((Handle)h);

v = (VersRec *) *h;
BlockMove(v->shortVersion, versionName, v->shortVersion[0]+1);
 
aboutDialog = GetNewDialog(kBaseWinID,0,kMoveToFront);

SetPort(aboutDialog);
MoveTo(12,166);
TextSize(12);
DrawString(versionName);
do{
ModalDialog(0,&itemHit);

} while(!Button());
DisposDialog(aboutDialog);

SetPort(muWinPtr);
//SetPort(muDialog);

HUnlock((Handle)h);
}

void updateVolText()
{
Rect r;
Str255 volStr;

SetPort(muWinPtr);
//SetPort(muDialog);

SetRect(&r,410,160,450,180);
FillRect(&r,qd.white);
MoveTo(410,176);
DrawString("\pVol:");
NumToString(gCurSnd.volume, volStr);
MoveTo(440,176);
DrawString(volStr);

}


void goMIDITextInstr(Boolean wantText)
{
short oldSize;
Rect r;

//SetPort(muDialog);
SetPort(muWinPtr);

SetRect(&r,24,180,180,200);

FillRect(&r,qd.white);

if(wantText)
{
oldSize = qd.thePort->txSize;
MoveTo(24,194);
TextSize(10);
DrawString("\pPlay Mode. Press space to stop.");
TextSize(oldSize);
}
}



#endif