# 1Bit Dream Machine

## Game programming API 

Destination platform: 68000 Macintosh, in particular the Mac Plus 
Development IDE: Symantec THINK C++ 6.0

### Goals

* Make it easier to develop arcade-like and animation heavy games
* Do the heavy lifting for the sound and music preparation of the Sound Driver (which pre-dates the Sound Manager)
* Allow MIDI output, assuming one has an Apple MIDI interface connected to a serial port, or a 3rd party clone
* Simplifies timing managers for specific delays based on seconds, on frames, on ms or microsecond scales (the latter requiring a recent enough OS, 6.0.3+ iirc)
* [Releases](https://github.com/Mu0n/1bitdreammachine/releases/) will be in the form of a single .SIT archive and a .HDA virtual volume to be used on a real Mac with a SD card storage replacement, or in emulation

### Folders in this repository 
Navigate to the following folders for more information about using the functions for each category of source code 

* [Graphics](https://github.com/Mu0n/1bitdreammachine/tree/first-big-sort/Graphics):  Offscreen bitmaps, Sprites with multiple frames and with masks, Alternate Screen Buffer (to do), Dissolve effect (to do) 
* [MIDI](https://github.com/Mu0n/1bitdreammachine/tree/first-big-sort/MIDI): Send MIDI out commands to an external device, receive MIDI in commands (to do), Standard MIDI file playback type 0 and type 1 (wip) 
* [PrepTools](https://github.com/Mu0n/1bitdreammachine/tree/first-big-sort/PrepTools): GrafPort/Windows prep functions, Ranged Random, File Load Dialog, Misc time-saving tools 
* [SoundDriver](https://github.com/Mu0n/1bitdreammachine/tree/first-big-sort/SoundDriver): Square wave synth, 4-voice synth, Freeform synth
* [StudioSessionMusic](https://github.com/Mu0n/1bitdreammachine/tree/first-big-sort/StudioSessionMusic): Studio Session file format using 6 channels mixed together (to do)
* [Timer](https://github.com/Mu0n/1bitdreammachine/tree/first-big-sort/Timer): Functions for timers based on these scales: seconds, frames, milliseconds and microseconds

#### Additional Documentation

The Inside Macintosh books are a great source of information for these oldest compact Macs, programming wise: https://vintageapple.org/inside_o/
THINK Reference 2.0 is an invaluable aid for programming and quickly navigating related topics: https://macintoshgarden.org/apps/think-reference
This popular Tinker Different forum resource page is a great starting point to get going with this particular target platform, using emulation as a development platform: https://tinkerdifferent.com/resources/think-c-development.84/
This series on YouTube for Mac programming is tied to this page, from yours truly for both! https://www.youtube.com/watch?v=Vv3w2igjiBo&list=PLwCULB5sLmz3OxPemixD2GZtJeDXmb0fx
