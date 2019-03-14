/*! \file roomstorageloader.h
 *
 *  A room-based storage loader for items
 */
#pragma once
#include <sadhash.h>
#include <cstdlib>
#include <functional>
#include "storedobject.h"
#include <sadrect.h>

#include <sprite2d.h>
#include <p2d/body.h>

namespace game
{

/*! A room-based storage loader
 */
class RoomStorageLoader
{
public:
    /*! Makes new room storage loader
     *  \param[in] sprites a sprites for loader
     *  \param[in] room_radius a room radius, which determines how far should be ranged items
     *  \param[in] detection_radius a detection radius which determines how far polling would work
     *  \param[in] load_item an item loading callback, called when item is loaded
     *  \param[in] unload_item an item unloading callback, called when item is unloaded
     */
    RoomStorageLoader(
        const sad::Vector<sad::Sprite2D*>& sprites,
        double room_radius,
        double detection_radius,
        std::function<void(void*)>  load_item,
        std::function<void(void*)>  unload_item
    );
    /*! Makes new room storage loader
     *  \param[in] bodies a bodies for loader
     *  \param[in] room_radius a room radius, which determines how far should be ranged items
     *  \param[in] detection_radius a detection radius which determines how far polling would work
     *  \param[in] load_item an item loading callback, called when item is loaded
     *  \param[in] unload_item an item unloading callback, called when item is unloaded
     */
    RoomStorageLoader(
        const sad::Vector<sad::p2d::Body*>& bodies,
        double room_radius,
        double detection_radius,
        std::function<void(void*)> load_item,
        std::function<void(void*)> unload_item
    );
    /*! Does nothing, as all references here are week
     */
    ~RoomStorageLoader();
    /*! Removed object from loader
     */
    void removeItem(void* object);
    /*! Tries to load relevant room for loader
     */
    void tryLoadRelevantRoom(const sad::Rect2D& rect);
private:
    /*! Splits objects from items array into rooms, filling data for radius
     *  \param[in] areas a list of areas
     *  \param[in] room_radius a room radius
     */
    void splitIntoRooms(const sad::Vector<sad::Rect2D>& areas, double room_radius);
    /*! Loads specified room
     *  \param[in] index index of room
     */
    void loadRoom(int index);
    /*! Unloads specified room
     *  \param[in] index index of unloaded room
     */
    void unloadRoom(int index);
    /*! Makes simple normalized room number for loader by coordinate
     * \param[in] coord a coordinate
     * \return data
     */
    inline int normalizedRoomNumber(double coord);
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
    /*! A room detection radius for player to store items
     */
    double m_detection_radius;
    /*! A reference from items to position in m_items vector for faster removal
     */
    sad::Hash<void*, size_t> m_items_to_vector_position;
    /*! An items list for loader
     */
    sad::Vector<game::StoredObject> m_items;
    /*! A room number to vector of item indexes for loading
     */
    sad::Vector< sad::Vector<size_t> > m_room_number_to_items;
    /*! A callback for loading item for room
     */
    std::function<void(void*)> m_load_item;
    /*! A callback for unloading item for room
     */
    std::function<void(void*)> m_unload_item;
    /*! First active room
     */
    int m_active_room_1;
    /*! Second active room
     */
    int m_active_room_2;
};

}
