#ifndef MUSOUNDDRIVER_H
#define MUSOUNDDRIVER_H

/*muSoundDriver.h
* VERSION 1.0.0 - March 2026
*/

#define PI 3.14159
#define kSndOffset 32 //attempt to reduce pops for freeform playback
#define kClick 740 //attempt to reduce pops for freeform playback
 
void prepSWSynth(void);
void swNoteSend(char, char, char); //square-wave synth send a note

void resetFT(void);
void prepFTWaves(void);

void ftNoteSend(char, char, char, Wave *); //four-tone synth send a note
void findFreeFT(unsigned char, Wave *);
void liberateFT(unsigned char);

void ffNoteSend(char, char, char, Handle, long, unsigned char);
Handle loadFFSound(int, long *);

extern long swWords[]; //for square-wave synth
extern long FTLongs[]; //for four-tone synth

extern short swPoly;
extern unsigned char ftPoly[];
extern short countFT;
extern short ffPoly;

extern Handle swHand;
extern SWSynthPtr mySWPtr;

extern Wave triWave; //for triangle, amp goes from 0 to 0xFF
extern Wave squWave; //for square, amp goes from 0 to 0xFF
extern Wave sinWave; //for sinus, amp goes from 0 to 0xFF
extern Wave sawWave; //for sinus, amp goes from 0 to 0xFF
extern Wave emptyWave; //for silence, 0x7F values for the four-tone synth


#endif
