/*! \mainpage Game Example
    
    Describes a simple shooting game example,
    demonstrating engine basics.
 */
/*! \file main.cpp
    

    Describes a main file for game
 */
#include "game.h"

#include <renderer.h>

/*! A main function 
    \param[in] argc count of arguments
    \param[in] argv arguments
 */
int main(int argc, char** argv)
{   
    // Create and run game	
    Game * game = new Game(sad::Renderer::ref());
    if (!game->trySetup())
    {
        delete game;
        return 1;
    }
    game->initialize();
    game->run();
    delete game;

    // If we are here, this means, that window is closed and we can exit the main loop
    return 0;
}

