#include "../game/game.h"

#include <renderer.h>
#include <opengl.h>
#include <os/extensionfunctions.h>
#include <pipeline/pipeline.h>
#include <pipeline/pipelinetask.h>

int main(int argc, char** argv)
{
    sad::Renderer* r = sad::Renderer::ref();
    r->log()->addTarget(new sad::log::ConsoleTarget());
    r->init(sad::Settings(640, 480, false));
    r->pipeline()->appendTask([=] {
        r->opengl()->extensionFunctions()->tryInit();
    });
    r->run();
    /*
    Game* game = new Game(sad::Renderer::ref());
    if (!game->trySetup())
    {
        delete game;
        return 1;
    }
    game->initialize();
    game->run();
    delete game;
    */

    return 0;
}
