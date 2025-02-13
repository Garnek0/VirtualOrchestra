# Virtual Orchestra

Virtual Orchestra is a musical instrument simulator made as a hobby project for fun.

The basic idea is that this program lets you see how different musical instruments are played by highlighting their "active" parts (and perhaps doing some other stuff, depending on the instrument). Of course, audio playback is supported too.

NOTE: This project is in very early development (pre-alpha). There are probably a ton of bugs, design issues and misconceptions about how the instruments work.
If you find any bugs or notice something is not as it should be, please feel free to open an issue or contribute via PR.

## Building and Running

### Dependencies
- `libfluidsynth`
- `libsmf`

### Linux

In the root directory, run `make build` to build the project or `make run` to build and run the program with the sample `res/arpeggio.mid` MIDI file.
If you want to use another MIDI file, you have to run the Virtual Orchestra binary FROM THE ROOT DIRECTORY otherwise it will not work. You need
to pass your MIDI file as an argument to the program (like this: `./build/vo "path/to/midi/file.mid`).

### Windows

Use a Linux environment.

### MacOS

I dont know how MacOS works, but I *think* the process is the same as on Linux.

## Instructions

Once you open VO you will be greeted with a piano keyboard. As of now thats the only instrument available (again, this is pre-alpha after all). 
To start playing/pause the music, press Space. To stop and rewind to the beginning, press S.
You can move the camera around using the arrow keys or by dragging the stage while holding down the middle mouse button, though there isn't much to see.
You can also zoom in/out with the scroll wheel.

## Acknowledgements

MuseScore team - MSBasic soundfont (see MSBASIC_LICENSE)
