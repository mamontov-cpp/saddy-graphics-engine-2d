/*!  \file levelstorageloader.h
 *
 *   Defines a level storage loader for game
 */
#pragma once
#include "roomstorageloader.h"
#include "staticobjectcontainer.h"

#include <p2d/world.h>

namespace game
{

/*! A level storage loader for loading coins and platforms, when loading
 */
class LevelStorageLoader
{
public:
    /* Default detection radius
     */
    static const int DETECTION_RADIUS;
    /*! A radius for physics room
     */
    static const int PHYSICS_ROOM_RADIUS;
    /*! A sprite room radius
     */
    static const int SPRITE_ROOM_RADIUS;
    /*! Makes new loader
     *  \param[in] container a container
     *  \param[in] world a new world data
     *  \param[in] r rectangle a rectangle for player to pool
     */
    LevelStorageLoader(game::StaticObjectContainer* container, sad::p2d::World* world, const sad::Rect2D& r);
    /*! Frees memory from other loaders
     */
    ~LevelStorageLoader();
    /*! Removes sprite from loader
     *  \param[in] s sprite
     */
    void removeSprite(sad::Sprite2D* s) const;
    /*! Removes body from loader
     *  \param[in] b body from loader
     */
    void removeBody(sad::p2d::Body* b) const;
    /*! Tries to load relevant room for loader
     *  \param[in] rect a rectangle for player
     */
    void tryLoadRelevantRoom(const sad::Rect2D& rect);
private:
    /*! Loads specified room
     *  \param[in] index index of room
     */
    void loadRoom(int index) const;
    /*! Unloads specified room
     *  \param[in] index index of unloaded room
     */
    void unloadRoom(int index)  const;
    /*! Makes simple normalized room number for loader by coordinate
     * \param[in] coordinate a coordinate
     * \return data
     */
    inline int normalizedRoomNumber(double coordinate)  const;
    /*! Tries to match 1 room to 2 old
     *  \param[in] min_index a minimal room index
     */
    void tryMatch1RoomFor2Old(int min_index);
    /*! Tries to match 2 room to 1 old
     *  \param[in] min_index a minimal room index
     *  \param[in] max_index a maximal room index
     */
    void tryMatch2RoomsFor1Old(int min_index, int max_index);
    /*! Tries to match 2 room to 1 old
     *  \param[in] min_index a minimal room index
     *  \param[in] max_index a maximal room index
     */
    void tryMatch2RoomsFor2Old(int min_index, int max_index);
    /*! A platform sprite loader
     */
    game::RoomStorageLoader* m_platform_sprite_loader;
    /*! A platform body loader
     */
    game::RoomStorageLoader* m_platform_body_loader;
    /*! A coin sprite loader
     */
    game::RoomStorageLoader* m_coin_sprite_loader;
    /*! A coin body loader
     */
    game::RoomStorageLoader* m_coin_body_loader;
    /*! First active room
     */
    int m_active_room_1;
    /*! Second active room
     */
    int m_active_room_2;
    /*! A count of rooms in loader
     */
    int m_room_count;
};

}
