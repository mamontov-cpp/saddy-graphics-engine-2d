/*! \file waysremove.h
 *  \author HiddenSeeker
 *
 * Describes a simple command for removing a way
 */
#pragma once
#include "../command.h"
#include <p2d/app/way.h>

namespace history
{

namespace ways
{

/*! \class history::ways::New
 *   A simple command for adding a new way to list
 */
class Remove: public history::Command
{
public:
    /*! Creates new command
        \param[in] w way
        \param[in] pos a position of way in source list
     */
    Remove(sad::p2d::app::Way* w, int pos);
    /*! Could be inherited
     */
    virtual ~Remove();
    /*! Makes way inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL);
    /*! Makes way active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL);
protected:
    /*! A current selected way
     */
    sad::p2d::app::Way* m_way;
    /*! A position of item in list
     */
    int m_position;
};

}

}
