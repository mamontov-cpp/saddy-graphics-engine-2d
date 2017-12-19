#include "game.h"

#include <renderer.h>

#include <freetype/font.h>

#include <stdexcept>

// ==================================== PUBLIC METHODS ====================================

Game::Game()  // NOLINT
{
    
}

Game::~Game()  // NOLINT
{
    
}

void Game::runMainGameThread()
{
    /* Firstly, we create our own renderer, which will do all kind of tasks
    */
    sad::Renderer r;
    /*! Create and set scene for renderer;
     */
    sad::Scene * scene = new sad::Scene();
    r.setScene(scene);
    r.tree()->factory()->registerResource<sad::freetype::Font>();
}

void Game::runInventoryThread()
{
    /* Firstly, we create our own renderer, which will do all kind of tasks
     */
    sad::Renderer r;
    /*! Create and set scene for renderer;
     */
    sad::Scene * scene = new sad::Scene();
    r.setScene(scene);
    r.tree()->factory()->registerResource<sad::freetype::Font>();
}


// ==================================== PRIVATE METHODS ====================================

Game::Game(const Game&)  // NOLINT
{
    throw std::logic_error("Not implemented");
}

Game& Game::operator=(const Game&)
{
    throw std::logic_error("Not implemented");
    // ReSharper disable once CppUnreachableCode
    return *this;
}
