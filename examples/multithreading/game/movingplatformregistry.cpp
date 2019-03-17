#include "movingplatformregistry.h"
#include "levelstorageloader.h"
#include "../game.h"

// =================================== PUBLIC METHODS ==================================

game::MovingPlatformRegistry::MovingPlatformRegistry() : m_db(NULL), m_game(NULL)
{

}

game::MovingPlatformRegistry::MovingPlatformRegistry(const game::MovingPlatformRegistry& o) : m_db(o.m_db), m_game(o.m_game)
{
    this->copyState(o);
}

game::MovingPlatformRegistry& game::MovingPlatformRegistry::operator=(const game::MovingPlatformRegistry& o)
{
    this->destroy();
    this->copyState(o);
    return *this;
}

game::MovingPlatformRegistry::~MovingPlatformRegistry()
{
    this->destroy();
}

void game::MovingPlatformRegistry::setDatabase(Game* game, sad::db::Database* db)
{
    this->m_game= game;
    this->m_db = db;
}

bool game::MovingPlatformRegistry::add(sad::p2d::Body* platform, sad::p2d::app::Way* way)
{
    if ((platform == NULL) || (way == NULL))
    {
        return false;
    }
    for (size_t i = 0; i < m_states.size(); i++)
    {
        if (m_states[i].Platform == platform)
        {
            return false;
        }
    }
    platform->addRef();
    way->addRef();
    platform->setCurrentPosition(way->getPointInTime(0.0, 0.0));
    m_game->physicsWorld()->addBodyToGroup("platforms", platform);
    m_game->levelStorageLoader()->removeBody(platform);
    const sad::Vector<sad::Object*>& v = platform->userObjects();
    for(size_t i = 0 ; i < v.size(); i++)
    {
        sad::Sprite2D* sprite = static_cast<sad::Sprite2D*>(v[i]);
        sprite->setVisible(true);
        m_game->levelStorageLoader()->removeSprite(sprite);
    }

    game::MovingPlatformState state{platform, way, 0.0, false};
    m_states << state;
    return true;
}

bool game::MovingPlatformRegistry::add(const sad::String& platform_name, const sad::String& way_name)
{
    if (!m_db)
    {
        return false;
    }
    sad::p2d::app::Way* way = m_db->objectByName<sad::p2d::app::Way>(way_name);
    if (!way)
    {
        return false;
    }
    if (m_platforms.contains(platform_name))
    {
        return add(m_platforms[platform_name], way);
    }
    else
    {
        return false;
    }
}


void game::MovingPlatformRegistry::remove(sad::p2d::Body* platform)
{
    for (size_t i = 0; i < m_states.size(); i++)
    {
        if (m_states[i].Platform == platform)
        {
            game::MovingPlatformState& state = m_states[i];
            state.Platform->delRef();
            state.Way->delRef();
            m_states.removeAt(i);
            --i;
        }
    }
}

void game::MovingPlatformRegistry::clear()
{
    this->destroy();
}

void game::MovingPlatformRegistry::movePlatforms(double tick)
{
    for (size_t i = 0; i < m_states.size(); i++)
    {
        game::MovingPlatformState& state = m_states[i];
        double ctime = state.Time;
        if (state.Downward)
        {
            ctime -= tick;
        }
        else
        {
            ctime += tick;
        }
        // ReSharper disable once CppInitializedValueIsAlwaysRewritten
        bool valid = true;
        // Fix outbounding time, when we should change trajectory
        do
        {
            valid = true;
            if (ctime < 0)
            {
                state.Downward = false;
                ctime *= -1;
                valid = false;
            }
            if (ctime > state.Way->totalTime())
            {
                if (state.Way->closed())
                {
                    while (ctime > state.Way->totalTime())
                    {
                        ctime -= state.Way->totalTime();
                    }
                }
                else
                {
                    state.Downward = true;
                    ctime = state.Way->totalTime() - (ctime - state.Way->totalTime());
                }
                valid = false;
            }
        } while (!valid);


        sad::p2d::Vector nextPosition = state.Way->getPointInTime(ctime, 0.0); // 0.0 - because it's just a part of sum in getPointInTime
        sad::p2d::Vector p = state.Platform->position();
        sad::p2d::Vector cp = nextPosition;
        state.Platform->setCurrentTangentialVelocity((cp - p) / tick);

        state.Time = ctime;
    }
}

void game::MovingPlatformRegistry::addPlatform(const sad::String& name, sad::p2d::Body* platform)
{
    if ((platform) && (!m_platforms.contains(name)))
    {
        platform->addRef();
        m_platforms.insert(name, platform);
    }
}

// =================================== PRIVATE METHODS ==================================

void game::MovingPlatformRegistry::copyState(const game::MovingPlatformRegistry& o)
{
    for (size_t i = 0; i < o.m_states.size(); i++)
    {
        game::MovingPlatformState state = o.m_states[i];
        state.Platform->addRef();
        state.Way->addRef();
        m_states << state;
    }
    m_platforms = o.m_platforms;
    for(auto it = m_platforms.begin(); it != m_platforms.end(); ++it)
    {
        it.value()->addRef();
    }
    m_db = o.m_db;
}

void game::MovingPlatformRegistry::destroy()
{
    for (size_t i = 0; i < m_states.size(); i++)
    {
        game::MovingPlatformState& state = m_states[i];
        state.Platform->delRef();
        state.Way->delRef();
    }
    m_states.clear();
    for(auto it = m_platforms.begin(); it != m_platforms.end(); ++it)
    {
        it.value()->delRef();
    }
    m_platforms.clear();
}
