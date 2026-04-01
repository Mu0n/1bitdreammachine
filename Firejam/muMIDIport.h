#ifndef MUMIDIPORT_H
#define MUMIDIPORT_H

#define kInputBufferSize 1024 //used for an input buffer size in bytes


int bufCount(void);
int bufHas(int);
void bufPut(unsigned char);
unsigned char bufGet(void);
void setupMIDISerial(void);
OSErr MyOpenSerialDriver(void);
void MyChangeInputBuffer(void);
void resetInputBuffer(void);
OSErr MySetHandshakeOptions(short);
void TweakClock(Boolean);
void MyConfigureThePort(void);
void InitSCCA(void);
void MyRestoreInputBuffer(void);
void MyCloseSerialDriver(void);
void Prep3bParamBlock(void);
OSErr midiChangeProgram(char, char);
OSErr midiNoteSend(char, char, char);
unsigned char midiNoteDetect(unsigned char *, unsigned char *, unsigned char *, 
				    Boolean *, Boolean *);


extern short gOutputRefNum; //used for MIDI out
extern short gInputRefNum; //used for MIDI in
extern Handle gInputBufHandle; //used for the input buffer

extern Boolean gPortGreenFlag; // if true, go ahead and use the port
extern Boolean gDone;
extern int midiBufLen;
extern unsigned char Buf[];

#endif