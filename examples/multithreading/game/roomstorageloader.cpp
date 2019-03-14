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
) : m_detection_radius(detection_radius), m_load_item(std::move(load_item)), m_unload_item(std::move(unload_item)), m_active_room_1(-1), m_active_room_2(-2)
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
) : m_detection_radius(detection_radius), m_load_item(std::move(load_item)), m_unload_item(std::move(unload_item)), m_active_room_1(-1), m_active_room_2(-2)
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

void game::RoomStorageLoader::tryLoadRelevantRoom(const sad::Rect2D& rect)
{
    double min = rect.p0().x();
    double max = rect.p2().x();
    if (min > max) {
        std::swap(min, max);
    }
    int min_index =  this->normalizedRoomNumber(min);
    int max_index = this->normalizedRoomNumber(max);

    // TODO Determine what rooms are to be loaded, what are to be unloaded
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

int game::RoomStorageLoader::normalizedRoomNumber(double coord)
{
    int index =  static_cast<int>((coord / m_detection_radius));
    if (index < 0) {
        return 0;
    }
    if (index >= m_room_number_to_items.size()) {
        return m_room_number_to_items.size() - 1;
    }
    return index;
}

void game::RoomStorageLoader::tryMatch1RoomFor2Old(int min_index)
{
    if (m_active_room_1 == min_index)
    {
        // TODO: CHECK ARGUMENTS
        unloadRoom(m_active_room_2);
        m_active_room_2 = -1;
    }
    else
    {
        if (m_active_room_2 == min_index)
        {
            // TODO: CHECK ARGUMENTS
            unloadRoom(m_active_room_1);
            m_active_room_1 = min_index;
            m_active_room_2 = -1;
        }
        else
        {
            // TODO: CHECK ARGUMENTS
            unloadRoom(m_active_room_1);
            unloadRoom(m_active_room_2);
            loadRoom(min_index);
            m_active_room_1 = min_index;
            m_active_room_2 = -1;
        }
    }
}

void game::RoomStorageLoader::tryMatch2RoomsFor1Old(int min_index, int max_index)
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


void game::RoomStorageLoader::tryMatch2RoomsFor2Old(int min_index, int max_index)
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
