#include "mainlevelloader.h"

#include <db/dbdatabase.h>

#include <functional>
#include <cassert>

game::MainLevelLoader::MainLevelLoader(sad::Renderer* renderer) : m_loaded_game_screen(false), m_first_time_loaded_game_screen(false), m_thread(NULL), m_renderer(renderer)
{
    std::function<void()> loader = [=, this]() {
        sad::db::Database* database = new sad::db::Database();
        database->setRenderer(renderer);
        database->tryLoadFrom("examples/multithreading/game_screen.json");
        database->saveSnapshot();
        renderer->addDatabase("gamescreen", database);
        this->m_loaded_game_screen = true;
        this->m_first_time_loaded_game_screen = true;
    };
    m_thread = new sad::Thread(loader);
}

game::MainLevelLoader::~MainLevelLoader()
{
    delete m_thread;
}


void game::MainLevelLoader::runLoaderThread()
{
    bool result = m_thread->run();
    assert( result );
}


void game::MainLevelLoader::loadGameScreen()
{
    if (!m_loaded_game_screen)
    {
        if (m_thread->running())
        {
            m_thread->wait();
        }
    }
    else
    {
        if (m_first_time_loaded_game_screen)
        {
            m_first_time_loaded_game_screen = false;
        }
        else
        {
            m_renderer->database("gamescreen")->restoreSnapshot();
        }
    }
}
