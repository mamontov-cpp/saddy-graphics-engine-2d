/*! \file gamestate.h

    A local game state for synchronizing state of thread
 */
#pragma once

namespace threads
{

/*! A global game state
 */
enum class GameState: int
{
    GS_UNITIALIZED = 0, // Renderer not fully started yet
    GS_START = 1,       // A starting screen
    GS_PLAYING = 2,     // A playing screen is presented
    GS_LOST = 3         // A player had lost a game
};

}
