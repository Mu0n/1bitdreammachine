#ifndef MUMIDIPORT_C
#define MUMIDIPORT_C

/*muMIDIport.c
* VERSION 1.0.0 - March 2026
* These routines will properly set a serial port (modem or printer)
* and keep a reference number for these opened ports with
* gOutputRefNum and gInputRefNum. Uses a 1024 byte buffer for input.
* set in a handle gInputBufHandle.
*/

#include "serial.h"
#include "muMIDIport.h"
#include "muDispatch.h"
#include "stdio.h"
#include "muSoundDriver.h"

// for serial stuff
short gOutputRefNum;
short gInputRefNum;
Handle gInputBufHandle;
Boolean gPortGreenFlag=false;

// Parameter blocks, used to stash your MIDI commands received or sent in serial
ParamBlockRec myParamBlock; //good for 3byte commands
ParamBlockRec myParamBlock2b; //good for 2byte commands

ParmBlkPtr myPBPtr;

//MIDI staging buffer area while midi-in reading

#define BUF_SIZE 128
unsigned char Buf[BUF_SIZE];
int head;
int tail;
unsigned char runningStatus; //for when MIDI cmds are run-on repeated

//helper function for the midi byte buffer
int bufCount(void)
	{
	return (head - tail + BUF_SIZE) & (BUF_SIZE-1);
	}
	
int bufHas(int n)
	{
	return bufCount() >= n;
	}
void bufPut(unsigned char b)
	{
	Buf[head] = b;
	head = (head +1) & (BUF_SIZE - 1);
	}

unsigned char bufGet(void)
	{
	unsigned char b = Buf[tail];
	tail = (tail + 1) & (BUF_SIZE - 1);
	return b;
	}


//setupMIDISerial: one-stop shop for setting everything up
//This respects the order in which the steps must be done
void setupMIDISerial()
	{
	MyOpenSerialDriver(); //serial port stuff
	MyChangeInputBuffer(); //serial port stuff
	MySetHandshakeOptions(gOutputRefNum); //serial port stuff
	MySetHandshakeOptions(gInputRefNum);
	TweakClock(true); //gets us that poorly documented 31.250 kbps speed on the modem serial port
	TweakClock(false); //gets us that poorly documented 31.250 kbps speed on the modem serial port
	Prep3bParamBlock();
	
	head = 0;
	tail = 0;
	runningStatus = 0;
	}
	
OSErr MyOpenSerialDriver()
	{
	OSErr theErr;
	theErr = OpenDriver("\p.AOut",&gOutputRefNum);
	if(theErr == noErr) 
		{
		theErr = OpenDriver("\p.AIn",&gInputRefNum);
		if(theErr == noErr) 
			{
			gPortGreenFlag = true;
			return theErr;
			}
		}
	return theErr;
	}

void MyChangeInputBuffer(void)
	{
	gInputBufHandle = NewHandleClear(kInputBufferSize);
	HLock(gInputBufHandle);
	SerSetBuf(gInputRefNum, *gInputBufHandle,kInputBufferSize);
	}
void resetInputBuffer(void)
	{
	DisposHandle(gInputBufHandle);
	gInputBufHandle = NewHandleClear(kInputBufferSize);
	}
	
OSErr MySetHandshakeOptions(short whichBuf)
	{
	SerShk mySerShkRec;
	mySerShkRec.fXOn = 0;
	mySerShkRec.fCTS = 0;
	mySerShkRec.errs = 0;
	mySerShkRec.evts = 0;
	mySerShkRec.fInX = 0;
	mySerShkRec.fDTR = 0;
	
	return Control(whichBuf, 14, &mySerShkRec);;
	}
	
// TweakClock: sets the serial port to 31,250 bps for MIDI
// if OutOrIn is true, sets the out port speed
// if OutOrIn is false, sets the in port speed
// ideally you want to do both if you intend to use both MIDI in & out
void TweakClock(Boolean OutOrIn)
	{
	Ptr addr;
	if(OutOrIn) addr = *(Ptr*)0x1DC;
	else addr = *(Ptr*)0x1D8;
	
	addr[2] = 4; //WR0 = register pointer set to WR4
	asm{MOVE.B (SP),(SP)} //2.2 us delay
	
	addr[2]=0x84; //WR4 = x32, 1 stop bit, no parity
	asm{MOVE.B (SP),(SP)} //2.2 us delay
	
	addr[2] = 11; //WR0 = register pointer set to WR11
	asm{MOVE.B (SP),(SP)} //2.2 us delay
	
	addr[2]=0x28; //WR11 = Rx and Tx clock from TRxC pin (HSKi on Macs)
	asm{MOVE.B (SP),(SP)} //2.2 us delay
	}
	
	
// Prep3bParamBlock
// used to carry 3 bytes MIDI commands. Good for Note On, Note Off
// clone this for other sizes like 2 bytes commands (ie change program))
// for your MIDI needs
void Prep3bParamBlock(void)
	{
	myParamBlock.ioParam.ioRefNum = gOutputRefNum;
	myParamBlock.ioParam.ioReqCount = 3;
	myParamBlock.ioParam.ioCompletion = nil;
	myParamBlock.ioParam.ioVRefNum = 0;
	myParamBlock.ioParam.ioPosMode = 0;
	
	myParamBlock2b.ioParam.ioRefNum = gOutputRefNum;
	myParamBlock2b.ioParam.ioReqCount = 2;
	myParamBlock2b.ioParam.ioCompletion = nil;
	myParamBlock2b.ioParam.ioVRefNum = 0;
	myParamBlock2b.ioParam.ioPosMode = 0;
	}

OSErr midiChangeProgram(char whichChan, char whichProg)
	{
	char myMessage[2];
	myMessage[0]=0xC0 | whichChan;
	myMessage[1]=whichProg;
	
	myParamBlock2b.ioParam.ioBuffer = myMessage;
	myPBPtr = (ParamBlockRec *)&myParamBlock2b;
	
	return PBWrite(myPBPtr, FALSE);
	}
OSErr midiNoteSend(char whichCmd, char whichNote, char whichVelo)
	{
	char myMessage[3];
	myMessage[0]=whichCmd;
	myMessage[1]=whichNote;
	myMessage[2]=whichVelo;
	myParamBlock.ioParam.ioBuffer = myMessage;
	myPBPtr = (ParamBlockRec *)&myParamBlock;
	return PBWrite(myPBPtr, FALSE);
	}
	
void MyRestoreInputBuffer(void)
	{
	SerSetBuf(gInputRefNum, *gInputBufHandle,0);
	//HUnlock(gInputBufHandle); //this froze my mac for some reason...
	}
	
void MyCloseSerialDriver(void)
	{
	OSErr theErr;
	theErr = KillIO(gOutputRefNum);
	if(theErr == noErr) theErr = CloseDriver(gInputRefNum);
	if(theErr == noErr) theErr = CloseDriver(gOutputRefNum);
	}


unsigned char midiNoteDetect(unsigned char *lastCmd, unsigned char *lastNote, 
 					   unsigned char *lastVelo, Boolean *hit, Boolean *done)
	{
	long nbBytes = 0;
	long readByte = 16L;
	Boolean toCmd = true, toNote  = false, toVelo = false;
	unsigned char lastByte, d1, d2; //for MIDI in detection
	int remain=0;
	int i=0;
	
	
	SerGetBuf(gInputRefNum, &nbBytes);
	remain = nbBytes;
	for(i=0; i<remain; i++)
		{
		readByte = 1L;
		FSRead(gInputRefNum, &readByte, &lastByte);
		bufPut(lastByte);
		}	

	while(bufCount() > 0) 
		{
		lastByte = Buf[tail];
		//STATUS BYTE
		if(lastByte & 0x80)
			{
			runningStatus = lastByte;
			bufGet(); //consume that status
			
			if(runningStatus < 0xF0)
				{
				if(!bufHas(2)) break; //need 2 data bytes
				d1 = bufGet();
				d2 = bufGet();
				dispatch(runningStatus, d1, d2);
				if(d1  == 0x6C)
					{
					gDone = true;
					}
				continue;
				}
				continue;
			}
		//DATA BYTE
		if(runningStatus == 0)
			{
			bufGet(); //discard
			continue;
			}
		if(!bufHas(2)) break;
		
		d1 = bufGet();
		d2 = bufGet();
		dispatch(runningStatus, d1, d2);
					if(d1  == 0x6C)
					{
					gDone = true;
					}
		/*
		readByte = 1L;
		FSRead(gInputRefNum, &readByte, &lastByte); //read one byte at a time
 		
		if(lastByte == 0xFE) continue; //skip these keepalive signals
		if(toCmd)
			{
			if((lastByte&0xF0) == 0x90)
				{
				*hit = true;
				*lastCmd = lastByte;
				toNote = true;
				toCmd = false;
				}
			else if((lastByte & 0xF0) == 0x80)
				{
				*hit = false;
				*lastCmd = lastByte;
				toNote = true;
				toCmd = false;
				}
			}
		else if(toNote)
			{
			*lastNote = lastByte;
			toNote = false;
			toVelo = true;
			}
		else if(toVelo)
			{
			*lastVelo = lastByte;
			*done = true;			
			dispatch(*lastCmd, *lastNote, *lastVelo);
			if(*lastNote == 0x6C) 
				{
				gDone = true;
				return;
				}
			toCmd = true;
			toNote = false;
			toVelo = false;
			}
		remain--;
		*/
		} // end while
	} //end of function
#endif