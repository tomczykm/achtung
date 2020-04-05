# freeachtung

A C++ rewrite of [this game](https://en.wikipedia.org/wiki/Achtung,_die_Kurve!).

### Building on Linux

1. Install CMake, git, build essentials and required development libraries. For example, on Ubuntu run: `sudo apt install cmake git build-essential libsfml-dev libsqlite3-dev`
2. [Install TGUI](https://tgui.eu/tutorials/0.8/)
3. Download sources: `git clone https://github.com/tunczyk110/freeachtung.git && cd freeachtung && git submodule update --init --recursive`
4. Go to `build` directory, generate Makefile and build the project: `mkdir build && cd build && cmake .. && make`
5. Run `./freeachtung` to play the game

You may also run `make all test` to run automated tests.

### Building on Windows

The easiest way to build on Windows seems to be with vcpkg, CMake and MSBuild. Tested with MSBuild included with Visual Studio 2019.

1. Install required libraries: `vcpkg install sfml tgui sqlite3`
2. Generate Visual Studio solution: `cmake -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake -G "Visual Studio 16 2019"`
3. Build the generated ALL_BUILD project `MSBuild ALL_BUILD.vcxproj -property:Configuration=Release`
