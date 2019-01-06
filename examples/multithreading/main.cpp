/*! \mainpage Multithreading Example
    A simple example of two OpenGL windows running simultaneously
 */
/*! \file main.cpp
    
    A multithreading example is an example of two OpenGL windows in one process
    running simultaneously.
 */

#include <sadthread.h>
#include "game.h"

#ifdef WIN32
#ifdef MSVC_RELEASE
#include <Windows.h>
#endif
#endif



/* This macro manupilation is to make console window hidden in MSVC window
 */
int main(int argc, char** argv)
{
    Game main_game;
    // Here we create two waitable threads
    sad::Thread a(&main_game, &Game::runMainGameThread);
    sad::Thread b(&main_game, &Game::runInventoryThread);

    // Run them
    a.run();
    b.run();

    // And wait
    a.wait();
    b.wait();

    // Free irrklang to remove inner racing conditions with windows sound subsystems
    sad::irrklang::Engine::freeInstance();
    return 0;

}

