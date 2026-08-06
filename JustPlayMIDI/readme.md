# JustPlayMIDI #

## Background standard MIDI file playback ##

This allows the playback of a type 0 or type 1, but not type 2 midi file, which you integrate somewhere as part as your game loop. This uses the library files you need in a very simplistic Mac application just to showcase how to use it.

### Dependencies ###

This requires a MIDI module (hardware, external), hooked up to either the modem or printer port of a 68k Mac. Some modules have a macintosh serial ports, some only deal with DIN5 classic MIDI cable, in which case you'd need a Macintosh MIDI serial interface as well.

Just include muMidiPlay.c and the rest will follow.
```
[Your project] -> [muMidiPlay.c] --+--> [muMidiPlay.h]
                                   |
                                   +--> [muLoadMIDIData.c] -> [muLoadMIDIData.h]
                                   |
                                   +--> [muMIDISerial.c] -> [muMIDISerial.h]
                                   |
                                   +--> [muTimer.c] -> [muTimer.h]
```
## Usage ##

  Just #include "muMIDIPlay.c" in your project, and that file will include the rest
  
  Step 1a: either have a .mid file in the same folder as the executable
  
  Step 1b: or embed the .mid file in a 'smf ' raw data resource in your project's .rsrc file
  
  Step 2: Use this every time you want to load a standard MIDI file
  
     prepMIDIFileForPlay("name of file"); //from a file in the same directory
       or
     prepMIDIResForPlay(resID); //from a 'smf ' resource embedded in your application
     
  this will copy the content of the .mid into memory
  
  Step 3: Once during your loop pass, do this:
     midiLoopPass()
  returns false if it's done playing. You can also check it with isMIDIDone()
  
  Step 4: Do this to "rewind" the midi file at its beginning and start the playback over
     rewindAndPlayMIDI();
