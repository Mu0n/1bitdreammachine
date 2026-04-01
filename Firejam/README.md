# 1Bit Dream Machine

## FireJam Folder 

* FireJam:  Lets your compact mac be at the center of music production. It can take in mouse clicks, typing keyboard playing and MIDI in keyboard playing. It can output sound in the old Mac Sound Driver, or to a MIDI out connected external module. 

## Release History

March 31st 2026 - presentation video https://youtu.be/gliJVwzRifA

### Assumes:
a 1MHz midi interface plugged in the modem port and a MIDI sound module in that interface's out port.
or, if you have something like a Roland Sound Canvas, they have a mac serial port in the back.

### Features:

* Window control in the shape of an 88-keys piano, defined as a CDEF resource
* Responds to these inputs: mouse clicks, typing keyboard keys and MIDI in note presses
* Selects 4 different output modes: monophonic square wave, polyphonic 4-tones, monophonic freeform samples and polyphonic MIDI out
* Can load a new freeform sample from a 'snd ' file
* Can select a different MIDI instrument out of the 128 regular general MIDI instruments

### Known Issues:

* Will not work under the Sound Manager of System 7 or above, although it is theoretically possible to only use MIDI in and out in that context.
* The note highlighting in and out becomes sluggish once the event queue is getting filled or has been busy
* Crashes under certain conditions
