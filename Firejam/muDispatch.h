#ifndef MUDISPATCH_H
#define MUDISPATCH_H

typedef struct gCurrentSound
{
short gChoice;
int gChosenFF;
short midiChannel;
short midiInst;
unsigned char detectChar;
Wave *ptrWave; //four-tone wave pointer
short swPoly;
short countFT;
short ffPoly;
unsigned char lastMIDI;
short volume;
short octShift;
};

typedef struct {
	Boolean noteOn[128];
	Pattern blackPat;
	Pattern whitePat;
	Pattern grayPat;
	Pattern actBlackPat;
	Pattern actWhitePat;
	Pattern actWhitePat2;
	Pattern actWhitePat3;
	Pattern actWhitePat4;
	RgnHandle noteRgn[88];
} PianoStates;

typedef pascal long (*CDEFProcPtr)(
   short varCode, 
   ControlHandle c, 
   short message,
   long param
);

unsigned char minOf(unsigned char, unsigned char);
void prepDispatch(void);
void dispatch(unsigned char, unsigned char, unsigned char);
void purgeBuffer(void);

extern int gChoice;
extern struct gCurrentSound gCurSnd;
extern long   sBufLen[]; //array of buffer lengths
extern Handle sHands[]; //array of sound Handles, 8 prebuilt, 1 free to load
extern short  sRefNote[]; //array of what the sound needs to be as ref note
extern Wave *ptrWave;

#endif