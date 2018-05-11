# achtung

A C++ rewrite of [this game](https://en.wikipedia.org/wiki/Achtung,_die_Kurve!).

### Build instructions for Ubuntu/Debian based distros

1. Install CMake, git, build essentials, SDL2 and it's extensions: `sudo apt-get install cmake git build-essential libsdl2-dev libsdl2-image-dev`
2. Download sources: `git clone https://github.com/tomczykm/achtung.git`
3. Go to `bin` directory, generate Makefile and build the project: `cd bin && cmake .. && make`
4. Run `./achtung` to play the game
