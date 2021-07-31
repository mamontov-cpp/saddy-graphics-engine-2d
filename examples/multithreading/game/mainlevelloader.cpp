#include "mainlevelloader.h"

#include <db/dbdatabase.h>

#include <functional>
#include <cassert>

game::MainLevelLoader::MainLevelLoader(sad::Renderer* renderer) : m_loaded_game_screen(false), m_first_time_loaded_game_screen(false), m_thread(nullptr), m_renderer(renderer)
{
    std::function<void()> loader = [=]() {
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


void game::MainLevelLoader::runLoaderThread() const
{
    bool result = m_thread->run();
    assert( result );
}


void game::MainLevelLoader::loadGameScreen()
{
    SL_LOCAL_DEBUG(str(fmt::Format("Is loaded game screen?: {0}, Is loaded game screen first time?: {1}") << m_loaded_game_screen << m_first_time_loaded_game_screen), *m_renderer);
    if (!m_loaded_game_screen)
    {
        if (m_thread->running())
        {
            m_thread->wait();
        }
        m_first_time_loaded_game_screen = false;
    }
    else
    {
        if (!m_first_time_loaded_game_screen)
        { 
            m_renderer->database("gamescreen")->restoreSnapshot();
        }
        else
        {
            m_first_time_loaded_game_screen = false;
        }
    }
}
