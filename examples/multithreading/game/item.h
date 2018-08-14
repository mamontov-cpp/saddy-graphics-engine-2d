/*! \file item.h
 *
 *   Contains a basic in-game item
 */
#pragma once
#include <sadstring.h>

namespace game 
{

 /*! A basic in-game item
  */
class Item   // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
    /*! Constructs new item
     *  \param[in] icon an icon for item
     *  \param[in] title a title for item
     *  \param[in] description a description for item
     */
    Item(const sad::String& icon, const sad::String& title, const sad::String& description);
    /*! An item can be inherited to implement own items
     */
    virtual ~Item();
protected:
    /*! An icon for item
     */
    sad::String m_icon;
    /*! A highlight title for item
     */
    sad::String m_title;
	/*! A description for item
	 */
	sad::String m_description;
};

}