# 1Bit Dream Machine

## cozyMIDI Folder 

* cozyMIDI:   a standard MIDI file player for classic macs. Runs fairly well on a Plus, very well on a SE/30 (so far). It needs a synth MIDI module and possibly a serial MIDI interface if your module doesn't connect directly to a Mac.

## Release History

June 19th 2026 - v0.1
presentation video -pending-

### Assumes:
a 1MHz midi interface plugged in the modem port and a MIDI sound module in that interface's out port.
or, if you have something like a Roland Sound Canvas, they have a mac serial port in the back.

### Features:

* loads and plays type 0 (1 track) and type 1 (more than 1 track) standard MIDI files

* keeps them in a pre-calculated form so the loop doesn't have to do real time divisions at the cost of slightly longer file opening delays

* animations with small graphic zones using CopyBits and 32 pixel width to keep it lean different MIDI instrument out of the 128 regular general MIDI instruments

### Known Issues:

* Refuses to work out of the printer port
