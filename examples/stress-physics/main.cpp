/*! \mainpage Stress Test For Physics
    
    Describes a small test for physics engine in a game
 */
/*! \file main.cpp
    

    Describes a main file for game
 */
#include <renderer.h>
#include <font.h>
#include "world.h"

#include <cstdlib>
#include <ctime>

/*! A main function 
    \param[in] argc count of arguments
    \param[in] argv arguments
 */
int main(int argc, char** argv)
{
    // Init logs with target streams to file  and console
    sad::log::FileTarget * file_target = new sad::log::FileTarget();
    file_target->open("log.txt");

    // A format for console is described as {date}: [file and line] [subsystem] message
    sad::log::ConsoleTarget * console_target = new sad::log::ConsoleTarget(
        "{0}: [{1}] {3}{2}{4}", 0, true
    );
    sad::log::Log::ref()->addTarget(file_target).addTarget(console_target);

    SL_MESSAGE ("This is stress-test for physics engine.\nYou will see what capabilities does it have");

    // Initializes a renderer as non-fullscreen 800x600 window
    sad::Renderer::ref()->init(sad::Settings(800,600,false));
    SL_MESSAGE("Renderer successfully initialized!");	
    // Initializes generator for spawns and random events
    srand(static_cast<unsigned>(time(nullptr)));  // NOLINT(cert-msc51-cpp)

    //Loading resources. We re-use textures from game example. 
    const sad::Vector<sad::resource::Error *> errors = sad::Renderer::ref()->loadResources("examples/stress-physics.json");
    if (!errors.empty())
    {
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

