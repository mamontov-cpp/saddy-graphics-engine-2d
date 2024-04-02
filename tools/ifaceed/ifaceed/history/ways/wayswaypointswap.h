/*! \file wayswaypointswap.h
 *  
 *
 * Describes a simple command for swapping points
 */
#pragma once
#include "../command.h"
#include <way.h>

namespace history
{

namespace ways
{

/*! \class history::ways::WayPointSwap
 *   A simple command for swapping points in way
 */
class WayPointSwap: public history::Command
{
public:
    /*! Creates new command
        \param[in] w way
        \param[in] pos1 first position
        \param[in] pos2 second position
     */
    WayPointSwap(sad::Way* w, int pos1, int pos2);
    /*! Could be inherited
     */
    virtual ~WayPointSwap() override;
    /*! Swaps way points
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Swaps way points
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A current selected way
     */
    sad::Way* m_way;
    /*! A first index for a point
     */
    int m_pos1;
    /*! A second index for a point
     */
    int m_pos2;
    /*! Tries to update ui for swapped entries
        \param[in] ob editor
     */
    void tryUpdateUI(core::Editor* ob);
};

}

}
