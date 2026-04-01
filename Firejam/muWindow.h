#ifndef MUWINDOW_H
#define MUWINDOW_H

#define kBaseWinID 128
#define kMoveToFront (WindowPtr)-1L

#define bPlayID     129
#define bVolMinusID 130
#define bVolPlusID  131
#define bLowerOctID 132
#define bIncrOctID  133

#include "PopUp.h"

typedef struct pictButtonRec {
PicHandle pic;
Boolean active;
Rect r;
} pictButtonRec, *pictButtonPtr, **pictButtonH;


void prepWindow(void);
void updateVolText();
void goMIDITextInstr(Boolean);
void buildOutputButtons(void);

extern Rect muWinRect;
extern WindowPtr muWinPtr;
extern DialogPtr muDialogPtr;
extern ControlHandle bPlay, cPiano;
extern PopUpPtr gPopup;

extern pictButtonH bSquare, b4Tones, bFreeform, bMIDI;

#endif