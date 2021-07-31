/*! \mainpage Stress Test For Physics
    
    Describes a small test for physics engine in a game
 */
/*! \file main.cpp
    

    Describes a main file for game
 */
#include <renderer.h>
#include <font.h>
#include <texturemappedfont.h>
#include "world.h"

#include <math.h>
#include <time.h>

/*! A main function 
    \param[in] argc count of arguments
    \param[in] argv arguments
 */
int main(int argc, char** argv)
{
    // Init logs with target streams to file  and console
    sad::log::FileTarget * filetarget = new sad::log::FileTarget();
    filetarget->open("log.txt");

    // A format for console is described as {date}: [file and line] [subsystem] message
    sad::log::ConsoleTarget * consoletarget = new sad::log::ConsoleTarget(
        "{0}: [{1}] {3}{2}{4}", 0, true
    );
    sad::log::Log::ref()->addTarget(filetarget).addTarget(consoletarget);

    SL_MESSAGE ("This is stress-test for physics engine.\nYou will see what capabilities does it have");

    // Inits a renderer as non-fullscreen 800x600 window
    sad::Renderer::ref()->init(sad::Settings(800,600,false));
    SL_MESSAGE("Renderer successfully initialized!");	
    // Inits generator for spawns and random raings
    srand((unsigned int)time(nullptr));

    //Loading resources. We re-use textures from game example. 
    bool res=true; 
    sad::Vector<sad::resource::Error *> errors = sad::Renderer::ref()->loadResources("examples/stress-physics.json");
    sad::String errortext;
    if (errors.size() != 0)
    {
        res = false;
        SL_FATAL(sad::resource::format(errors));
        sad::util::free(errors);
        return 1;
    } 
    SL_MESSAGE(sad::String("Resources successfully loaded"));
    
    // Create and run game
    World * world = new World();
    world->run();
    delete world;


    // If we are here, this means, that window is closed and we can exit the main loop
    return 0;
}

