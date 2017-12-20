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

    sad::log::FileTarget * fl = new sad::log::FileTarget();
    bool b = fl->open(static_cast<const char *>(p));
    if (!b)
    {
        SL_LOCAL_DEBUG("Failed to open local file", r);
    }
    r.log()->addTarget( fl);

    /* Create 800x600 window in windowed mode and toggle a fixed size of window
     */
    r.init(sad::Settings(800,600, false));
    r.makeFixedSize();



    /* Bind built-ing scene to renderer
     */
    scene->setRenderer(&r);

    /*! Load resources
     */
    sad::Vector<sad::resource::Error *> errors = r.loadResources("examples/multmultithreading/multmultithreading.json");
    if (!errors.empty())
    {

        SL_LOCAL_FATAL(sad::resource::format(errors), r);
        sad::util::free(errors);
        return 1;
    }

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

    sad::log::FileTarget * fl = new sad::log::FileTarget();
    bool b = fl->open(static_cast<const char *>(p));
    if (!b)
    {
        SL_LOCAL_DEBUG("Failed to open local file", r);
    }
    r.log()->addTarget( fl);

    /* Create 800x600 window in windowed mode and toggle a fixed size of window
     */
    r.init(sad::Settings(800,600, false));
    r.makeFixedSize();



    /* Bind built-ing scene to renderer
     */
    scene->setRenderer(&r);

    /*! Load resources
     */
    sad::Vector<sad::resource::Error *> errors = r.loadResources("examples/multithreading.json");
    if (!errors.empty())
    {

        SL_LOCAL_FATAL(sad::resource::format(errors), r);
        sad::util::free(errors);
        return 1;
    }

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
