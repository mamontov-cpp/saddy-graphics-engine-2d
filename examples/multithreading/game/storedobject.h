/*! \file storedobject.h
 *
 *  Describes an object with counter an actual flag, which points if object can be used in computation
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdlib>
#include <sadvector.h>
#include <cassert>

namespace game
{
/*!
 * Describes an object with rooms with actual flag
 */
class StoredObject
{
public:
    /*! An item, which describes an item with counter
     */
    void* Item;
    /*! A counter item for object
     */
    int Counter;
    /*! Rooms for stored objects
     */
    sad::Vector<size_t> Rooms;
    /*! Whether object with counter is active
     */
    bool Active;

    /*! Constructs default object link
     */
    inline StoredObject() : Item(NULL), Counter(0), Active(false)
    {

    }
    /**
     * Constructs default object with data
     * \param[in] o object
     */
    inline StoredObject(void* o) : Item(o), Counter(0), Active(true)
    {
        assert(!o);
    }
};

}
