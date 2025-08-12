# 1Bit Dream Machine

## MIDI Folder 

* RealTimeMIDI: (todo) Will deal with MIDI in and MIDI out commands
* StandardMIDIFile: (todo) Open, parse and plays back a standard midi file

## Release History

August 11th 2025 - MIDI player.sit
Contains the state of this sandbox console app that was featured in this video https://www.youtube.com/watch?v=MToGjFoWsFU
Assumes:
a 1MHz midi interface plugged in the modem port and a MIDI sound module in that interface's out port.
or, if you have something like a Roland Sound Canvas, they have a mac serial port in the back. That config was not tested though.

Can:
* send test notes to MIDI Out. 0 = middle C, 1 = C# (one semitone above it), 2 = D, etc
* send a test C major scale
* open and parse a standard midi file
* play it back once the analysis is done
* go into a modal MIDI in mode where you can play with a piano controller, it'll send back the notes to your module in the MIDI out.
* contains a few bugs upon exiting (might system bomb)

  Future:
* cleaned up library files will be chopped up better and will be able to be integrated in other projects
