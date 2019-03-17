#include "levelstorageloader.h"

const int game::LevelStorageLoader::DETECTION_RADIUS = 800;
const int game::LevelStorageLoader::PHYSICS_ROOM_RADIUS = 1200;
const int game::LevelStorageLoader::SPRITE_ROOM_RADIUS = 2400;

// ======================================= PUBLIC METHODS =======================================
game::LevelStorageLoader::LevelStorageLoader(game::StaticObjectContainer* container, sad::p2d::World* world, const sad::Rect2D& r) : m_active_room_1(-1), m_active_room_2(-1)
{
    m_platform_sprite_loader = new  game::RoomStorageLoader(
        container->PlatformSprites,
        game::LevelStorageLoader::SPRITE_ROOM_RADIUS,
        game::LevelStorageLoader::DETECTION_RADIUS,
        [](void* o) { static_cast<sad::Sprite2D*>(o)->setVisible(true); },
        [](void* o) { static_cast<sad::Sprite2D*>(o)->setVisible(false); }
    );
    m_platform_body_loader = new  game::RoomStorageLoader(
        container->PlatformBodies,
        game::LevelStorageLoader::PHYSICS_ROOM_RADIUS,
        game::LevelStorageLoader::DETECTION_RADIUS,
        [=](void* o) { world->addBodyToGroup("platforms", static_cast<sad::p2d::Body*>(o)); },
        [=](void* o) { world->removeBodyFromGroup("platforms", static_cast<sad::p2d::Body*>(o)); }
    );
    m_coin_sprite_loader = new  game::RoomStorageLoader(
        container->CoinSprites,
        game::LevelStorageLoader::SPRITE_ROOM_RADIUS,
        game::LevelStorageLoader::DETECTION_RADIUS,
        [](void* o) { static_cast<sad::Sprite2D*>(o)->setVisible(true); },
        [](void* o) { static_cast<sad::Sprite2D*>(o)->setVisible(false); }
    );
    m_coin_body_loader = new  game::RoomStorageLoader(
        container->CoinBodies,
        game::LevelStorageLoader::PHYSICS_ROOM_RADIUS,
        game::LevelStorageLoader::DETECTION_RADIUS,
        [=](void* o) { world->addBodyToGroup("coins", static_cast<sad::p2d::Body*>(o)); },
        [=](void* o) { world->removeBodyFromGroup("coins", static_cast<sad::p2d::Body*>(o)); }
    );
    int room_count1 = std::max(m_platform_sprite_loader->roomCount(), m_platform_body_loader->roomCount());
    int room_count2 = std::max(m_coin_sprite_loader->roomCount(), m_coin_body_loader->roomCount());
    m_room_count = std::max(room_count1, room_count2);
    m_platform_sprite_loader->setRoomCount(m_room_count);
    m_platform_body_loader->setRoomCount(m_room_count);
    m_coin_sprite_loader->setRoomCount(m_room_count);
    m_coin_body_loader->setRoomCount(m_room_count);

    // Compute current plater room
    double min = r.p0().x();
    double max = r.p2().x();
    if (min > max) {
        std::swap(min, max);
    }
    int min_index =  this->normalizedRoomNumber(min);
    int max_index = this->normalizedRoomNumber(max);

    m_coin_body_loader->loadRoom(min_index);
    m_platform_body_loader->loadRoom(min_index);

    m_platform_sprite_loader->incrementCounterForRoom(min_index);
    m_coin_sprite_loader->incrementCounterForRoom(min_index);


    if (min_index == max_index)
    {
        m_active_room_1 = min_index;
        m_active_room_2 = -1;
    }
    else
    {
        m_coin_body_loader->loadRoom(max_index);
        m_platform_body_loader->loadRoom(max_index);

        m_platform_sprite_loader->incrementCounterForRoom(max_index);
        m_coin_sprite_loader->incrementCounterForRoom(max_index);

        m_active_room_1 = min_index;
        m_active_room_2 = max_index;
    }

    m_coin_sprite_loader->unloadIfCounterIsZeroExceptFor(min_index, max_index);
    m_platform_sprite_loader->unloadIfCounterIsZeroExceptFor(min_index, max_index);

}

game::LevelStorageLoader::~LevelStorageLoader()
{
    delete m_platform_sprite_loader;
    delete m_platform_body_loader;
    delete m_coin_sprite_loader;
    delete m_coin_body_loader;
}

void game::LevelStorageLoader::removeSprite(sad::Sprite2D* s) const
{
    m_coin_sprite_loader->removeItem(s);
    m_platform_sprite_loader->removeItem(s);
}

void game::LevelStorageLoader::removeBody(sad::p2d::Body* b) const
{
    m_coin_body_loader->removeItem(b);
    m_platform_body_loader->removeItem(b);
}


void game::LevelStorageLoader::tryLoadRelevantRoom(const sad::Rect2D& rect)
{
    double min = rect.p0().x();
    double max = rect.p2().x();
    if (min > max) {
        std::swap(min, max);
    }
    int min_index =  this->normalizedRoomNumber(min);
    int max_index = this->normalizedRoomNumber(max);

    if (m_active_room_1 == m_active_room_2)
    {
        m_active_room_2 = -1;
    }
    else
    {
        if (m_active_room_1 == -1)
        {
            m_active_room_1 = m_active_room_2;
            m_active_room_2 = -1;
        }
    }
    if (min_index == max_index)
    {
        this->tryMatch1RoomFor2Old(min_index);
    }
    else
    {
        if (m_active_room_2 == -1)
        {
            this->tryMatch2RoomsFor1Old(min_index, max_index);
        }
        else
        {
            this->tryMatch2RoomsFor2Old(min_index, max_index);
        }
    }
    // Swap loader
    if (m_active_room_1 > m_active_room_2)
    {
        std::swap(m_active_room_1, m_active_room_2);
    }
}


// ======================================= PRIVATE METHODS =======================================


void  game::LevelStorageLoader::loadRoom(int index) const
{
    m_coin_sprite_loader->loadRoom(index);
    m_coin_body_loader->loadRoom(index);

    m_platform_body_loader->loadRoom(index);
    m_platform_sprite_loader->loadRoom(index);
}

void game::LevelStorageLoader::unloadRoom(int index) const
{
    m_coin_sprite_loader->unloadRoom(index);
    m_coin_body_loader->unloadRoom(index);

    m_platform_body_loader->unloadRoom(index);
    m_platform_sprite_loader->unloadRoom(index);
}


int game::LevelStorageLoader::normalizedRoomNumber(double coord) const
{
    int index =  static_cast<int>((coord / game::LevelStorageLoader::DETECTION_RADIUS));
    if (index < 0) {
        return 0;
    }
    if (index >= m_room_count) {
        return m_room_count - 1;
    }
    return index;
}

void game::LevelStorageLoader::tryMatch1RoomFor2Old(int min_index)
{
    if (m_active_room_1 == min_index)
    {
        // CHECK ARGUMENTS
        unloadRoom(m_active_room_2);
        m_active_room_2 = -1;
    }
    else
    {
        if (m_active_room_2 == min_index)
        {
            // CHECK ARGUMENTS
            unloadRoom(m_active_room_1);
            m_active_room_1 = min_index;
            m_active_room_2 = -1;
        }
        else
        {
            //  CHECK ARGUMENTS
            unloadRoom(m_active_room_1);
            unloadRoom(m_active_room_2);
            loadRoom(min_index);
            m_active_room_1 = min_index;
            m_active_room_2 = -1;
        }
    }
}

void game::LevelStorageLoader::tryMatch2RoomsFor1Old(int min_index, int max_index)
{
    if (min_index == m_active_room_1)
    {
        loadRoom(max_index);
        m_active_room_2 = max_index;
    }
    else
    {
        if (max_index == m_active_room_1)
        {
            loadRoom(min_index);
            m_active_room_1 = min_index;
            m_active_room_2 = max_index;
        }
        else
        {
            unloadRoom(m_active_room_1);
            loadRoom(min_index);
            loadRoom(max_index);

            m_active_room_1 = min_index;
            m_active_room_2 = max_index;
        }
    }
}


void game::LevelStorageLoader::tryMatch2RoomsFor2Old(int min_index, int max_index)
{
    if (m_active_room_1 == min_index)
    {
        if (m_active_room_2 != max_index)
        {
            // UNLOAD m_active_room_2, load max_index
            unloadRoom(m_active_room_2);
            loadRoom(max_index);
            m_active_room_2 = max_index;
        }
    }
    else
    {
        if (m_active_room_2 == max_index)
        {
            if (m_active_room_1 != min_index)
            {
                 // UNLOAD m_active_room_1, load min_index
                unloadRoom(m_active_room_1);
                loadRoom(min_index);
                m_active_room_1 = max_index;
            }
        }
        else
        {
            // UNLOAD old, load all
            unloadRoom(m_active_room_1);
            unloadRoom(m_active_room_2);
            loadRoom(min_index);
            loadRoom(max_index);
            m_active_room_1 = min_index;
            m_active_room_2 = max_index;
        }
    }
}
