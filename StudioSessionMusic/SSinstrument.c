#include <Sound.h>#include "Misc.c"Handle LoadSSInst(short openResult,long *bufferSize);void main(void){SFTypeList myTypes;short openResult=0;long bufferSize=0;Handle sndH;Ptr myPtr;FFSynthPtr sndPtr;int loop;InitMacStuff();HideMyMenuBar();WholeScreen(white,false);myTypes[0]='DEWF';openResult = OldLoadFile(myTypes);sndH = LoadSSInst(openResult,&bufferSize);if(bufferSize == 0) ExitToShell();FSClose(openResult);HLock((Handle)sndH);myPtr = *sndH;sndPtr = (FFSynthPtr)myPtr;sndPtr->mode = ffMode;sndPtr->count = FixRatio(1,1);while(!Button()) {while(!SoundDone());StopSound();StartSound(sndPtr,bufferSize,nil); }StopSound();HUnlock((Handle)sndH);}Handle LoadSSInst(short file, long *bufferSize){Handle readH;Ptr temp;long sizeToRead =0;long count;int loopStart;int loopEnd;int pitch;count = 2;SetFPos(file,fsFromStart,0);FSRead(file,&count,&loopStart);SetFPos(file,fsFromStart,2);FSRead(file,&count,&loopEnd);SetFPos(file,fsFromStart,3);count=1;FSRead(file,&count,&pitch);count=2;SetFPos(file,fsFromStart,6);FSRead(file,&count,&sizeToRead);if(!sizeToRead) ExitToShell();*bufferSize = sizeToRead;SetFPos(file,fsFromStart,8);readH = NewHandleClear(sizeToRead);HLock((Handle)readH);FSRead(file,&sizeToRead,(*readH));HUnlock((Handle)readH);return readH;}/* StartSound 3rd param:(SndCompletionProcPtr)-1L*/