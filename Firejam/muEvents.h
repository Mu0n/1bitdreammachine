#ifndef MUEVENTS_H
#define MUEVENTS_H

#define kSleep 10L
#define kUseDefaultProc (void *)-1L

void eventPass(void);
void HandleMouseDown(EventRecord *, Boolean *, short *);
void HandleMouseUp(EventRecord *, Boolean *, short *);
void HandleMenuChoice(long);
void DoUpdate(EventRecord *);

extern Boolean mousePianoTracking;
extern short currentMouseNote;

#endif