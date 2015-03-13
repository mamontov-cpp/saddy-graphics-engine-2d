# Saddy 

Saddy is an open-source crossplatform 2D graphic engine, based on OpenGL, which can be used for creating simple games.

## Download

* **15.02.15** Saddy 1.5-alpha released!

A pre-built engine for Win32 Microsoft Visual C++2010 can be downloaded from here:

[saddy-1.5](https://sourceforge.net/projects/saddyengine/files/?source=navbar)

Note that you must have MSVC2010 redistributable files installed. Also you need to install Freetype library. This is just alpha release, so most of executables are not built. This is not a stable release, they will be built in next release.

* **19.10.14** Saddy 1.4 released!

You can download pre-built engine, demos and docs (Win32-MSVC version) from following pages:

[docs-1.4](https://dl.dropboxusercontent.com/u/5039908/docs-1.4.zip) 

[saddy-1.4](https://dl.dropboxusercontent.com/u/5039908/saddy-1.4.zip)

Changelog :

  * Resource loading has been fully remade to be independent from time of loading.
  * Switched to serializing to JSON for both resources and in-game screens, implemented simple serialization for all basic types
  * Marshalling has been fully remade. Old namespaces "config" and "marshal" has been replaced with "resource" and "db",
  * Resource reloading has been merged with the core.
  * Instead of just editing in-game screens in IFace Editor, now you could edit a simple database and even define objects with custom properties. Also, new version supports dialogue editing and editing ways for physic-based applications.

* **17.02.14** Saddy 1.1-BETA released!

You can download pre-built engine, demos and docs (Win32-MSVC version) from following pages:

[docs-1.1](https://dl.dropboxusercontent.com/u/5039908/docs-1.1-beta.zip) 

[saddy-1.1](https://dl.dropboxusercontent.com/u/5039908/saddy-1.1-beta.zip)

* **26.08.13** Saddy 1.0-BETA released!

## Our Goals

Our main goal is to create tiny graphic engine, which can be used to create a small native 2D games.
  * We want to provide a simple engine, which can be built with NO external dependencies at all. That's right. You don't need to download and build anything to build a source of main page
  * We do, however, use a 3rdparty libraries, like tpunit++ or format, but they are either linked statically, or moved to plugins.
  * We want to provide an easy-to-use toolchain to satisfy simple most basic needs for  creating a simple game.
  * We rely on established and well-known technologies, such as OpenGL, MSVC or GCC.
  * This is project done for fun and powered by it.   

## Roadmap

  * 1.0 - main engine structure
  * 1.1 - massive refactoring of all engine, new more simple structure introduced
  * 1.2  - replace of sad::FontManager and sad::TextureManager with sad::resource::Tree, allowing built-in reloading of assets and custom implementation of own assets. Full JSON support in core.
  * 1.3  - replace of marshal namespace entities with more awesome serialization system, which allows relationships between entities, querying by some parameters, object schemas, allowing constraints on data.
  * 1.4  - support for new serialization system in IFaceEditor.
  * 1.5(current)  - support for animations.
  * 1.5.1 - reserved for fixes of bugs and small improvements.

## Supported platforms

Currently, we support a following platforms.

  * Windows, using Microsoft Visual C++ 2010 (x86, x64)
  * Windows, using MinGW (x86/x64)
  * Ubuntu (x86/x64)
  * OpenSUSE (x86/x64)
  * ArchLinux (x86 only)  

However, we cannot provide binary builds for all of those platforms, only guarantee, that built source will work.

## Build order

First, you need to build main library, which is stored in main folder. After that you can built freetype plugin, or build game example or physics engine stress demo. After building freetype plugin, you can build IFaceEditor (plugins/ifaceed folder) or multithreading example (examples/multithreading folder).

**NOTE 1** You should check when running examples and IFaceEditor from MSVC, that your current working directory in debug is ..\..\bin

**NOTE 2** IFaceEditor and exporter require Qt 4.8+, but cannot be compiled under Qt 5+. They will be ported on Qt 5 ASAP, but currently they can't be compiled. You can support project by porting it to Qt 5.

**NOTE 3** There was some kind of issue with Ruby 2.0 and DevIL gem, required by atlasgen. You can try copy an OpenIL DLLs into atlasgen folder or install Ruby 1.9 and install gem for this version, making things work. Also, for Ruby 2.0 you could check our fork of DevIL gem: https://github.com/ahiddenseeker/devil .

## Saddy toolchain

Saddy toolchain consists of few small utilities

  * saddy - is the main engine core and has no requirements to be built.
  * saddy-freetype - is the binding of freetype fonts to saddy. Requires _freetype_.
  * exporter - a tool for exporting standard fonts into fonts with textures, which can be used with Saddy. Requires _Qt 4.8.x_.
  * atlasgen - a tool for creating texture atlases for saddy, from an xml configs and few smaller images.Requires _Ruby_ and _DevIL gem_.
  * ifaceed - a simple game screen editor. Requires _Qt 4.8.x_.

## Developers

  * Mamontov Dmitry - leader and current maintainer of project.
  * Alexander Sokolov - programmed core engine
  * Andrey Ryaskov -  programmed windows gui wrapper.
  * Victor Grigoriev - contributed to core engine development
  * Dmitry Kolesov - contributed to core engine development

## Examples

Saddy has three working examples

  * Multithreading - demonstrates basic multithreading capabilities for engine. A basic graphic capabilities is also demonstrated.
  * sad::Game - a simple shooting game


![screenshot](https://trello-attachments.s3.amazonaws.com/503608c12380a31f336bde54/521a2524ef90dbc747002f49/96ffc7621d241a1c80a97d86c2e5677e/saddy_small.png)


  * ifaceed   - yeah, this editor is also built using saddy.

![screenshot](https://trello-attachments.s3.amazonaws.com/503608c12380a31f336bde54/521a2524ef90dbc747002f49/2e670c7470f0c6be595071e66014a0ae/ifed1_small.png)  ![screenshot](https://trello-attachments.s3.amazonaws.com/503608c12380a31f336bde54/521a2524ef90dbc747002f49/8e40ac7245200f2e16eac539926a5b2a/ifed2_small.PNG)

## Run into a problem? Don't know where to start?

You can write directly to maintainer in Twitter
(https://twitter.com/ahiddenseeker) and we will reply as fast as we can.
