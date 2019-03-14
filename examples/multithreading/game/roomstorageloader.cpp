#include <utility>

#include <utility>

#include "roomstorageloader.h"

#include <p2d/rectangle.h>
#include <geometry2d.h>


// ====================================== PUBLIC METHODS ======================================

game::RoomStorageLoader::RoomStorageLoader(
    const sad::Vector<sad::Sprite2D*>& sprites,
    double room_radius,
    double detection_radius,
    std::function<void(void*)>  load_item,
    std::function<void(void*)>  unload_item
) : m_detection_radius(detection_radius), m_load_item(std::move(load_item)), m_unload_item(std::move(unload_item))
{
    sad::Vector<sad::Rect2D> areas;
    for(size_t i = 0; i < sprites.size(); i++)
    {
        if (sprites[i])
        {
            areas.push_back(sprites[i]->area());
            m_items.push_back(StoredObject(sprites[i]));
            m_items_to_vector_position.insert(sprites[i], i);
        }
    }
    this->splitIntoRooms(areas, room_radius);
}

game::RoomStorageLoader::RoomStorageLoader(
    const sad::Vector<sad::p2d::Body*>& bodies,
    double room_radius,
    double detection_radius,
    std::function<void(void*)>  load_item,
    std::function<void(void*)>  unload_item
) : m_detection_radius(detection_radius), m_load_item(std::move(load_item)), m_unload_item(std::move(unload_item))
{
    sad::Vector<sad::Rect2D> areas;
    for(size_t i = 0; i < bodies.size(); i++)
    {
        if (bodies[i])
        {
            areas.push_back(static_cast<sad::p2d::Rectangle*>(bodies[i]->currentShape())->rect());
            m_items.push_back(StoredObject(bodies[i]));
            m_items_to_vector_position.insert(bodies[i], i);
        }
    }
    this->splitIntoRooms(areas, room_radius);
}

void game::RoomStorageLoader::removeItem(void* object)
{
    if (m_items_to_vector_position.contains(object))
    {
        size_t pos = m_items_to_vector_position[object];
        game::StoredObject& o = m_items[pos];
        if (o.Active)
        {
            o.Active = false;
        }
    }
}


void game::RoomStorageLoader::loadRoom(int index)
{
    if (index > -1)
    {
        assert(index < m_room_number_to_items.size());
        const sad::Vector<size_t>& indexes = m_room_number_to_items[index];
        for(size_t i =  0; i < indexes.size(); i++)
        {
            if (m_items[indexes[i]].Active)
            {
                m_items[indexes[i]].Counter++;
                if (m_items[indexes[i]].Counter > 0)
                {
                    m_load_item(m_items[indexes[i]].Item);
                }
            }
        }
    }
}

void game::RoomStorageLoader::unloadRoom(int index)
{
    if (index > -1)
    {
        assert(index < m_room_number_to_items.size());
        const sad::Vector<size_t>& indexes = m_room_number_to_items[index];
        for(size_t i =  0; i < indexes.size(); i++)
        {
            if (m_items[indexes[i]].Active)
            {
                m_items[indexes[i]].Counter++;
                if (m_items[indexes[i]].Counter <= 0)
                {
                    m_unload_item(m_items[indexes[i]].Item);
                }
            }
        }
    }
}

int game::RoomStorageLoader::roomCount() const
{
    return m_room_number_to_items.size();
}

void game::RoomStorageLoader::setRoomCount(int room_count)
{
    while (m_room_number_to_items.size() < room_count)
    {
        m_room_number_to_items.push_back(sad::Vector<size_t>());
    }
}

void game::RoomStorageLoader::incrementCounterForRoom(int index)
{
    if (index > -1)
    {
        assert(index < m_room_number_to_items.size());
        const sad::Vector<size_t>& indexes = m_room_number_to_items[index];
        for(size_t i =  0; i < indexes.size(); i++)
        {
            if (m_items[indexes[i]].Active)
            {
                m_items[indexes[i]].Counter++;
            }
        }
    }
}

void game::RoomStorageLoader::unloadIfCounterIsZeroExceptFor(int min, int max)
{
    for(size_t index = 0; index < m_room_number_to_items.size(); index++)
    {
        const sad::Vector<size_t>& indexes = m_room_number_to_items[index];
        if ((index != min) && (index != max))
        {
            for(size_t i =  0; i < indexes.size(); i++)
            {
                if (m_items[indexes[i]].Active)
                {
                    if (m_items[indexes[i]].Counter <= 0)
                    {
                        m_unload_item(m_items[indexes[i]].Item);
                    }
                }
            }
        }
    }
}

// ====================================== PRIVATE METHODS ======================================

void game::RoomStorageLoader::splitIntoRooms(const sad::Vector<sad::Rect2D>& areas, double room_radius)
{
    int max_index = -1;
    for(size_t area_num = 0; area_num <= areas.size(); area_num++)
    {
        max_index = std::max(max_index, static_cast<int>(areas[area_num].p2().x() / m_detection_radius));
    }
    for(size_t i = 0; i <= max_index; i++)
    {
        m_room_number_to_items.push_back(sad::Vector<size_t>());
    }
    for(size_t room_number = 0; room_number <= m_room_number_to_items.size(); room_number++)
    {
        double min = room_number * m_detection_radius + m_detection_radius / 2.0 - room_radius / 2.0;
        double max = room_number * m_detection_radius + m_detection_radius / 2.0 + room_radius / 2.0;
        for(size_t area_num = 0; area_num <= areas.size(); area_num++)
        {
            if (sad::collides1D(min, max, areas[area_num].p0().x(), areas[area_num].p2().x()))
            {
                m_room_number_to_items[room_number].push_back(area_num);
                m_items[area_num].Rooms.push_back(room_number);
            }
        }
    }
}


