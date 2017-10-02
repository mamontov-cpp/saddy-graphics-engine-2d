[![AppVeyor](https://ci.appveyor.com/api/projects/status/u99u4tsb9dv6qwx3?svg=true&branch=master)](https://ci.appveyor.com/project/mamontov-cpp/saddy-graphics-engine-2d) [![Travis](https://travis-ci.org/mamontov-cpp/saddy-graphics-engine-2d.svg?branch=master)](https://travis-ci.org/mamontov-cpp/saddy-graphics-engine-2d) [![Join the chat at https://gitter.im/saddy-graphics-engine-2d/Lobby](https://badges.gitter.im/saddy-graphics-engine-2d/Lobby.svg)](https://gitter.im/saddy-graphics-engine-2d/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

# Saddy 

Saddy is an open-source crossplatform 2D graphic engine, based on OpenGL, which can be used to create simple games.

## Build status

| Platform      | Debug                                                                                                          | Release                                                                                                           |
| ------------- |----------------------------------------------------------------------------------------------------------------| ------------------------------------------------------------------------------------------------------------------|
| MSVC x32      | ![1](https://appveyor-matrix-badges.herokuapp.com/repos/mamontov-cpp/saddy-graphics-engine-2d/branch/master/1) | ![2](https://appveyor-matrix-badges.herokuapp.com/repos/mamontov-cpp/saddy-graphics-engine-2d/branch/master/2)    |
| MSVC x64      | ![1](https://appveyor-matrix-badges.herokuapp.com/repos/mamontov-cpp/saddy-graphics-engine-2d/branch/master/3) | ![2](https://appveyor-matrix-badges.herokuapp.com/repos/mamontov-cpp/saddy-graphics-engine-2d/branch/master/4)    |
| MinGW         | ![1](https://appveyor-matrix-badges.herokuapp.com/repos/mamontov-cpp/saddy-graphics-engine-2d/branch/master/5) | ![2](https://appveyor-matrix-badges.herokuapp.com/repos/mamontov-cpp/saddy-graphics-engine-2d/branch/master/6)    |


| Platform      | Debug                                                                                                          | Release                                                                                                           |
| ------------- |----------------------------------------------------------------------------------------------------------------| ------------------------------------------------------------------------------------------------------------------|
| Ubuntu        | ![1](https://travis-matrix-badges.herokuapp.com/repos/mamontov-cpp/saddy-graphics-engine-2d/branches/master/1) | ![2](https://travis-matrix-badges.herokuapp.com/repos/mamontov-cpp/saddy-graphics-engine-2d/branches/master/2)    |



## News

Note, that starting from 1.7.0, Saddy will only support compilers, that support C++14.

* **11.03.17** Saddy [1.6.1](https://github.com/mamontov-cpp/saddy/releases/tag/1.6.1) released!

* **22.08.16** Saddy [1.6.0](https://github.com/mamontov-cpp/saddy/releases/tag/1.6.0) released!

* **23.01.16** Saddy [1.5.4](https://github.com/mamontov-cpp/saddy/releases/tag/1.5.4) released!

* **18.01.16** Saddy [1.5.3](https://github.com/mamontov-cpp/saddy/releases/tag/1.5.3) released!

* **29.11.15** Saddy [1.5.2.2](https://github.com/mamontov-cpp/saddy/releases/tag/1.5.2.2) released!

* **23.09.15** Saddy [1.5.2.1](https://github.com/mamontov-cpp/saddy/releases/tag/1.5.2.1) released!

* **15.09.15** Saddy [1.5.2](https://github.com/mamontov-cpp/saddy/releases/tag/1.5.2) released!

* **5.08.15** Saddy [1.5.1](https://github.com/mamontov-cpp/saddy/releases/tag/1.5.1) released!

* **23.05.15** Saddy [1.5](https://github.com/mamontov-cpp/saddy/releases/tag/1.5) released!

* **15.02.15** Saddy [1.5-alpha](https://github.com/mamontov-cpp/saddy/releases/tag/1.5-alpha) released!

* **19.10.14** Saddy [1.4](https://github.com/mamontov-cpp/saddy/releases/tag/1.4) released!

* **17.02.14** Saddy 1.1-BETA released!

* **26.08.13** Saddy 1.0-BETA released!

## Our Goals

Our main goal is to create small graphics engine, which can be used to create a small 2D games.
  * We want to provide a simple engine, which can be built with NO external dependencies at all and work on old platforms, that don't support shaders or integrated videocards, so the core project can be built without installing any libraries (except for those, required by OpenGL and CMake, starting from 1.6.0+). The core project now uses old OpenGL calls, but nevertheless, tries to create OpenGL3+ context, so you can also use newer API.
  * We do, however, use a 3rdparty libraries, like [tpunit++](https://github.com/tpounds/tpunitpp) or [format](https://github.com/cppformat/cppformat), but they are either embedded, or moved to plugins. A complete list of dependencies could be found [here](https://github.com/mamontov-cpp/saddy-graphics-engine-2d/wiki/Dependencies)
  * We want to provide an easy-to-use toolchain to satisfy simple most basic needs for  creating a simple game.

## How to download project and get sources ready to compile

```
git clone https://github.com/mamontov-cpp/saddy-graphics-engine-2d.git
cd saddy-graphics-engine-2d
git submodule init
git submodule update
git submodule status
```

This will download whole project with dependencies

## Supported platforms

We provide support  for following platforms at the moment:

For Saddy v1.7.0 and after:

  * Windows, using Microsoft Visual C++ 2017+ (x86, x64)
  * Windows, using CMake/MinGW-w64 6.2.0+ (x86/x64)
  * Linux, using CMake/gcc 6.2.0 (x86/x64)

For Saddy v1.6.1 and before:

  * Windows, using Microsoft Visual C++ 2010+ (x86, x64)
  * Windows, using CMake/MinGW-w64 5.2.0+ (x86/x64)
  * Linux, using CMake/gcc (x86/x64)
  
## Example screenshots

![Game Example (screenshot 1)](http://s4.postimg.org/yk1zz3mu5/screen1.png)   ![Game Example (screnshot 2)](http://s9.postimg.org/ypqdxqpan/screen2.png)

![Physics (screenshot 1)](http://s27.postimg.org/44xxl1yeb/screen3.png) ![Physics  (screenshot 2)](http://s15.postimg.org/4u7mh2e8b/screen4.png)

![Instrumented game](https://s13.postimg.org/hplvp4i3b/gameinstrumented.png)

## Ran into a problem? Don't know where to start?

Check our [wiki](https://github.com/mamontov-cpp/saddy-graphics-engine-2d/wiki) and [examples](https://github.com/mamontov-cpp/saddy-graphics-engine-2d/tree/master/examples), or a simple [game example](https://github.com/mamontov-cpp/saddy-graphics-engine-2d/tree/master/examples/game). Build instructions can be found [here](https://github.com/mamontov-cpp/saddy-graphics-engine-2d/wiki/Build-instructions). 

You can write directly to maintainer in Twitter
(https://twitter.com/ahiddenseeker) and we will reply as fast as we can.
