/*! \file customobject.h
	\author HiddenSeeker

	Describes custom object with custom properties. Could be Sprite2D or Labels, depending on stuff
 */
#include "../../scenenode.h"
#include "../resource/link.h"
#include "../resource/tree.h"
#include "customschema.h"


namespace sad
{
class Sprite2D;
class Label;

namespace db
{
	
namespace custom
{

/*! Defines an extensible object as scene node, that could 
	be represented as either Sprite2D, or Label 
 */
class Object: public sad::SceneNode
{
SAD_OBJECT
public:
protected:
	/*! A text for a label
	 */
	sad::String m_label_text;
	/*! A rotation angle for object
	 */ 
	double m_angle;
	/*! An area for object
	 */
	sad::Rect2D m_area;
	/*! A color for objects
	 */
	sad::AColor m_color;
	/*! Whether sprite is flipped on x
	 */
	bool m_flipx;
	/*! Whether sprite is flipped on y
	 */
	bool m_flipy;
	/*! A sprite as defined 
	 */
	sad::Sprite2D * m_sprite2d;
	/*! A label as defined
	 */
	sad::Label * m_label;
	/*! An object's schema
	 */
	sad::db::schema::Schema * m_my_schema;
	/*! A schema for an object
		\param[in] schema for object
	 */
	sad::resource::Link<sad::db::custom::Schema> m_schema;
};


}

}

}
