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
	/*! Returns fully rotated region for object
		\return region
	 */
	virtual sad::Rect2D renderableArea() const;
	/*! Renders a current rendered object
	 */ 
	virtual void render();
	/*! Returns size of font, used in label
		\return font size
	 */
	unsigned int fontSize() const;
	/*! Sets size of font, used in label
		\param[in] size a size of custom object
	 */
	void setFontSize(unsigned int);
	/*! Returns line spacing for, used in label
		\return line spacing for label
	 */ 
	float lineSpacing() const;
	/*! Sets line spacing, used in label
		\param[in] s a line spacing
	 */
	void setLineSpacing(float s);
	/*! Returns angle, used in object
		\return angle in object
	 */
	double angle() const;
	/*! Sets angle
		\param[in] a object rotation angle
	 */
	void setAngle(double a);
	/*! Returns non-rotated object area
		\return non-rotated object area
	 */
	sad::Rect2D area() const;
	/*! Sets non-rotated object area
		\param[in] object area
	 */
	void setArea(const sad::Rect2D & area);
	/*! Returns a string for label
		\return string data
	 */
	const sad::String& string() const;
	/*! Sets a string for a label
		\param[in] s a string data
	 */
	void setString(const sad::String & s);
	/*! Returns color for sprite and label
		\return color for sprite and label
	 */
	const sad::AColor& color() const;
	/*! Sets color for sprite and label
		\return color
	 */
	void setColor(const sad::AColor & c);
	/*! Whether sprite is flipped horizontally
		\return whether sprite is flipped horizontally
	 */
	bool flipX() const;
	/*! Whether sprite is flipped horizontally 
		\param[in] whether sprite is flipped horizontally
	 */
	void setFlipX(bool f);
	/*! Whether sprite is flipped vertically
		\return whether sprite is flipped horizontally
	 */
	bool flipY() const;
	/*! Whether sprite is flipped vertically 
	 */
	void setFlipY(bool f);
protected:
	/*! A current rendered object for custom object
	 */
	sad::SceneNode * m_current_rendered_object;
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
