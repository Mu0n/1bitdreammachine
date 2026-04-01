#ifndef MUMENU_H
#define MUMENU_H


#define kBaseMenuID 128
#define mApple kBaseMenuID
#define iAbout 1

#define mFile  kBaseMenuID + 1
#define iPlay  1
#define iOpen  2
#define iQuit  4

#define mEdit  kBaseMenuID + 2

#define mMidi  kBaseMenuID + 3
#define iModem   1
#define iPrinter 2
#define iTest    4

#define mOutput kBaseMenuID + 4
#define iSW 1
#define iFT 2
#define iFF 3
#define iMD 4
#define iSinus 6
#define iSquare 7
#define iTriangle 8
#define iSawTooth 9
#define iEep 11
#define iDizzy 12
#define iIndigo 13
#define iSosumi 14
#define iBeep 15

#define mMIDIInst kBaseMenuID + 5

#define i_start_ft iSinus
#define i_end_ft   iSawTooth

#define i_start_ff iEep
#define i_end_ff   iBeep


void menuBarInit();
void HandleMenuChoice(long);
void HandleAppleChoice(short);
void HandleFileChoice(short);
void HandleMIDIChoice(short);
void HandleOutputChoice(short);
extern void midiInLoop(void);
void SetFTItem(short);
void SetFFItem(short);
void SetModemPrinter(short);
void openSndFile(void);
short OldLoadFile(SFTypeList);
void studioSessionRead(short, long *, Handle *);
extern short openedOnce;

extern Boolean gDone;

#endif