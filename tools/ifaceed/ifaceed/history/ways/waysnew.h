/*! \file waysnew.h
 *  
 *
 * Describes a simple command for adding a way
 */
#pragma once
#include "../command.h"
#include <way.h>

namespace history
{

namespace ways
{

/*! \class history::ways::New
 *   A simple command for adding a new way to list
 */
class New: public history::Command
{
public:
    /*! Creates new command
        \param[in] w way
     */
    New(sad::Way* w);
    /*! Could be inherited
     */
    virtual ~New() override;
    /*! Makes way active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Makes way inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A current selected way
     */
    sad::Way* m_way;
};

}

}
