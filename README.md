# freeachtung

A C++ rewrite of [this game](https://en.wikipedia.org/wiki/Achtung,_die_Kurve!).

### Building on Linux

1. Install CMake, git, build essentials and required development libraries. For example, on Ubuntu run: `sudo apt install cmake git build-essential libsfml-dev libsqlite3-dev`
2. [Install TGUI](https://tgui.eu/tutorials/0.8/)
3. Download sources: `git clone https://github.com/tunczyk110/freeachtung.git && cd freeachtung && git submodule update --init --recursive`
4. Go to `build` directory, generate Makefile and build the project: `mkdir build && cd build && cmake .. && make`
5. Run `./freeachtung` to play the game

### Building on Windows

Building the game on Windows is likely possible as-is, but you're going to have to figure this out yourself (for now).

### Planned features

1. Feature parity with [this version of the game](http://www.cda.pl/gry-online/6431c8/Uwaga-zakret-Achtung-Die-Kurve)
2. Publishing Windows binaries
3. Recording and viewing replay files
4. Statistics for player profiles
