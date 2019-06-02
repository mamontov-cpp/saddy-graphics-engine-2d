#include "../game/game.h"

int main(int argc, char** argv)
{
    Game* game = new Game(sad::Renderer::ref());
    if (!game->trySetup())
    {
        delete game;
        return 1;
    }
    game->initialize();
    game->run();
    delete game;

    return 0;
}
