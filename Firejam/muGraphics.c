#ifndef MUGRAPHICS_C
#define MUGRAPHICS_C

#include "muGraphics.h"
#include "muWindow.h"

PicHandle pPianoH;
Rect rMainPianoBounds;

void LoadGfxAssets()
{
//pPianoH = GetPicture(kPianoPICT);
}

void DrawAll()
{
DrawOutputButtons();
updateVolText();
updateVolKeybRange();
}  
void DrawPicAtXY(PicHandle picH, int x, int y)
{
//SetPort(muDialog);
SetRect(&rMainPianoBounds, (*picH)->picFrame.left,
		    (*picH)->picFrame.top,
		    (*picH)->picFrame.right,
		    (*picH)->picFrame.bottom);
OffsetRect(&rMainPianoBounds, x, y);
DrawPicture(picH, &rMainPianoBounds);
}

void DrawOutputButtons()
{
DrawOutputButton(bSquare);
DrawOutputButton(b4Tones);
DrawOutputButton(bFreeform);
DrawOutputButton(bMIDI);
}

void DrawOutputButton(pictButtonH but)
{
Rect insetR;

HLock((Handle)but);
insetR = (*but)->r;
InsetRect(&insetR,2,2);

SetPort(muWinPtr);
FillRect(&((*but)->r), qd.white);
DrawPicture((*but)->pic, &((*but)->r));
if((*but)->active) InvertRect(&insetR);
HUnlock((Handle)but);
}

void updateVolKeybRange()
{
Rect r;

//SetPort(muDialog);
SetPort(muWinPtr);
SetRect(&r,0,284,512,290);
FillRect(&r,qd.white);

SetRect(&r,136+(gCurSnd.octShift)*64,284,336+(gCurSnd.octShift)*64,290);
FillRect(&r,qd.black);
}    
      
#endif