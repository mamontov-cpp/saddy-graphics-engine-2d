/*! \file animations/animationstexturecoordinateslist.h
	\author HiddenSeeker

	An animation of changing texture coordinates, defined by their links in resource tree
	is stored here.
 */
#pragma once

#include "../fuzzyequal.h"
#include "../sadvector.h"
#include "../sadstring.h"
#include "../sadhash.h"

#include "animationsanimation.h"


namespace sad
{
	
namespace animations
{

/*! An animation, which changes texture coordinates of sprite over time
 */
class TextureCoordinatesList: public sad::animations::Animation
{
SAD_OBJECT
public:
	/*! Creates new empty animation
	 */
	TextureCoordinatesList();
	/*! Can be inherited
	 */
	virtual ~TextureCoordinatesList();
	/*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
	/*! Called, when loading an object. Here, object must make all resource path links depend on specified tree.
		By default, does nothing
		\param[in] renderer a renderer
		\param[in] treename a tree name
	 */
	virtual void setTreeName(
		sad::Renderer* renderer,
		const sad::String& treename
	);
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v);
	/*! Sets list of options, which defines texture coordinate sources
		\param[in] fonts a font list, used in animation
	 */
	void setList(const sad::Vector<sad::String>& list);
	/*! Returns options list
		\return options list
	 */
	const sad::Vector<sad::String> & list() const;
	/*! Sets state of object from animation
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time);
    /*! Creates a state command for an object
        \param[in] o object
        \return state command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* stateCommand(sad::db::Object* o);
    /*! Checks, whether animation is applicable to an object
        \param[in] o object
        \return whether animation is applicable to that object
     */
    virtual bool applicableTo(sad::db::Object* o);
protected:
	/*! Tries to fetch coordinates from cache
		\param[in] c coordinates
		\return NULL if not found, otherwise rectangle
	 */ 
	sad::Rect2D* coordinates(const sad::String& c);
	/*! A texture coordinates cache
	 */
	sad::Hash<sad::String, sad::Rect2D> m_cache;
	/*! A cached root folder for faster finding options with texture coordinates
	 */
	sad::resource::Folder* m_cache_root_folder;
	/*! A folder, for which cache is being created
	 */
	sad::resource::Folder* m_cache_folder;
	/*! A list of texture coordinates to be set
	 */
	sad::Vector<sad::String> m_list;
	/*! Sets renderer for list
	 */ 
	sad::Renderer* m_renderer;
	/*! Sets tree name for list
	 */
	sad::String m_tree_name;
};

}

}
