# freeachtung

A C++ rewrite of [this game](https://en.wikipedia.org/wiki/Achtung,_die_Kurve!).

### Building on Linux

1. Install CMake, git, build essentials and required development libraries. On distros with apt, run: `sudo apt install cmake git build-essential libsfml-dev`
2. Download sources: `git clone https://github.com/tomczykm/freeachtung.git && cd freeachtung && git submodule update --init --recursive`
3. Go to `build` directory, generate Makefile and build the project: `cd build && cmake .. && make`
4. Run `./freeachtung` to play the game

You may also build tests with `cmake --build . --target tests` and run with `make test`.

### Building on Windows

Building the game on Windows is likely possible as-is, but you're going to have to figure this out yourself (for now).

### Planned features

1. Feature parity with [this version of the game](http://www.cda.pl/gry-online/6431c8/Uwaga-zakret-Achtung-Die-Kurve)
2. Publishing Windows binaries
3. Recording and viewing replay files (with backwards compatibility, if possible)
4. Local player profiles with statistics
5. Several small QoL features (custom player names, custom colors, etc.)
