#ifndef MUGRAPHICS_H
#define MUGRAPHICS_H

#define kPianoPICT   128


void LoadGfxAssets(void);
void DrawAll(void);
void DrawPicAtXY(PicHandle, int, int);
void updateVolKeybRange(void);
void DrawOutputButtons(void);
void DrawOutputButton(pictButtonH);

extern PicHandle pPianoH;
extern Rect rMainPianoBounds;

#endif