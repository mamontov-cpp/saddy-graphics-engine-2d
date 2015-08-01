/*! \file   label.h
    

	Contains a definition of simple label node, that can be added to scene
*/
#include "scene.h"
#include "font.h"
#include "sadrect.h"
#include "3rdparty/format/format.h"
#include "timer.h"
#include "resource/link.h"
#pragma once

namespace sad
{
class Renderer;
/*! \class Label
    
	Class, that renders a simple multiline text on scene.
	Note, that width of text is unbounded and it grows on right direction and
	down from set rendering point
*/
class Label: public sad::SceneNode
{
 SAD_OBJECT
 public:
		/*! Creates a default broken sprite at (0,0) and no string
		 */
		Label();
		/*! Creates a simple label with font size of 20px and angle of zero.
			Default color is black.
		    \param[in] font  a font, which label is being rendered with
			\param[in] point an upper-left point, where text starts
			\param[in] string  a string, which is being rendered
		*/
		Label(
			  sad::Font *  font,
			  const sad::Point2D  & point,
			  const sad::String & string
			 );
		/*! Creates a simple label with font size of 20px and angle of zero.
			Default color is black.
		    \param[in] font  a font, which label is being rendered with
			\param[in] point an upper-left point, where text starts
			\param[in] string  a string, which is being rendered
			\param[in] tree a tree name, where font is stored
		*/
		Label(
			  const sad::String &  font,
			  const sad::Point2D  & point,
			  const sad::String & string,
			  const sad::String & tree = ""
			 );
		/*! Sets links resources to a treename and renderer
			\param[in] r renderer
			\param[in] treename a name for tree
		 */
		virtual void setTreeName(sad::Renderer* r, const sad::String& treename);
		/*! Fills vector of regions with data, that could be used for identifying bounds of item.
			As default, no regions are produced.
			\param[out] r a vector of regions
		*/
		virtual void regions(sad::Vector<sad::Rect2D> & r);
		/*! A basic schema for object
			\return a schema 
		 */
		static sad::db::schema::Schema* basicSchema();
		/*! Returns schema for an object
			\return schema
		 */
		virtual sad::db::schema::Schema* schema() const;
        /*! Renders a string of text inside of label
		*/
		virtual void render();
		/*! Called, when renderer for scene is changed
		 */
		virtual void rendererChanged();
		/*! Sets non-rotated renderable area
			\param[in] r non-rotated renderable area
		 */
		void setArea(const sad::Rect2D & r);
		/*! Returns non-rotated renderable area for a label
			\return non-rotated renderable area
		 */
		sad::Rect2D area() const;
		/*! Returns a bounding region for a label
			\return bounding region part
		 */
		virtual sad::Rect2D region() const; 
		/*! Returns a font for label
			\return font, which is label is being rendered with
		 */
		inline sad::Font * font() const
		{
			return m_font.get();
		}
		/*! Sets a font, that label is being rendered with			
			\param[in] font a font
		 */
		void setFont(sad::Font * font);
	    /*! Sets new font by trying to get it from a rendered
			\param[in] name a name for a font
			\param[in] r a renderer
			\param[in] tree a tree, where font should be located
		  */
		void setFont(const sad::String & name, sad::Renderer * r = NULL, const sad::String & tree = "");
		/*!  Returns a string for a label
			 \return a string
		 */
		inline const sad::String & string() const
		{
			return m_string;
		}
		/*! Sets a string for a label
			\param[in] string a rendered string in a label
		 */
		void setString(const sad::String & string);
		/*!  Returns an upper-left point, where label is placed
			 \return a string
		 */
		inline const sad::Point2D & point() const
		{
			return m_point;
		}
		/*! Set fonts name for a label
			\param[in] name a name of font for a label
		 */
		void setFontName(const sad::String & name);
		/*! Returns name for a font, used in label
			\return name for a used font
		 */
		inline const sad::String & fontName() const
		{
			return m_font.path();
		}
		/*! Sets upper-left point for a label
			\param[in] point a point
		 */
		void setPoint(const sad::Point2D & point);	
		/*!	Sets upper-left point for a label
			\param[in] x x coordinate
			\param[in] y y coordinate
		 */ 
		inline void setPoint(double x, double y)
		{
			setPoint(sad::Point2D(x, y));
		}
		/*! Returns counter-clockwise rotation angle for a label
			\return angle
		 */
		inline double angle() const
		{
			return m_angle;
		}
		/*! Sets angle for rotation of label
			\param[in] angle a rotation angle
		 */
		inline void setAngle(double angle)
		{
			m_angle = angle;
		}
		/*! Returns a size for a font in pixels
			\return size of label
		 */
		inline unsigned int size() const
		{
			return m_size;
		}
		/*! Returns a size for a label font in points
			\return a size for a label in points
		 */
		inline unsigned int pointSize() const
		{
			return (unsigned int)(m_size / 1.333333);
		}
		/*! Sets a size for a font in pixels
			\param[in] size a size of label in pixels
		 */
		void setSize(unsigned int size);
		/*!  Sets size for a font, used in label in pixels
			 \param[in] size size of lalbe font in pixels
		 */
		inline void setPointSize(unsigned int size)
		{
			setSize((unsigned int)(size * 1.3333333));
		}
		/*! Returns a built-in line spacing for a font
			\return built-in line spacing
		 */
		float builtinLineSpacing() const;
		/*! Returns a line spacing in pixels
			\return line spacings
		 */
		inline float lineSpacing() const
		{
			return m_linespacing_ratio * builtinLineSpacing();
		}
		/*! Sets new line spacing for a label
			\param[in] spacing a line spacing
		 */
		void setLineSpacing(float spacing);
		/*! Returns a line spacing ratio
			\return a line spacing ratio
		 */
		inline float lineSpacingRatio() const
		{
			return m_linespacing_ratio;
		}
		/*! Sets a line spacing ratio
			\param[in] ratio a line spacing ratio
		 */
		void setLineSpacingRatio(float ratio);
		/*! Returns a color for a label
			\return color
		 */
		inline const sad::AColor & color() const
		{
			return m_color;
		}
		/*! Sets a color for a label
			\param[in] color a color for a label
		 */
		inline void setColor(const sad::AColor & color)
		{
			m_color = color;
		}
		/*! Sets a color for a label
			\param[in] r red component
			\param[in] g green component
			\param[in] b blue component
			\param[in] a alpha component
		 */
		inline void setColor(sad::uchar r, sad::uchar g, sad::uchar b, sad::uchar a)
		{
			setColor(sad::AColor(r,g,b,a));
		}
		/*! Destructs it
		*/
		virtual ~Label();
		/*! When set scene and font name is defined label tries to reload
			itself from scene's renderer
			\param[in] scene a scene, which will render a node
		 */
		virtual void setScene(sad::Scene * scene);
		/*! Sets treename for a sprite
			\param[in] treename a name for a tree
		*/
		void setTreeName(const sad::String & treename);
private:
		/*! Reloads font for a label from scene
		 */
		void reloadFont();
		/*! Recomputes rotation coefficients, 
			so rotation will be placed just in place 
		 */
		void recomputeRenderingPoint();
		/*! A link to font, that label is being renderd with
		 */
		sad::resource::Link<sad::Font> m_font;
		/*! A rendered string
		 */
		sad::String      m_string; 
		/*! An upper left point, where text is rendered
		 */
		sad::Point2D     m_point;  
		/*! A counter-clockwise rotation angle for a label
		 */
		double            m_angle;
		/*! A size of font in label in pixels
		 */
		unsigned int     m_size;
		/*! A line spacing ratio, that is being set on font
		 */
		float            m_linespacing_ratio;
		/*! A color for label
		 */
		sad::AColor      m_color;
		/*! A non-rotated center of label
		 */
		sad::Point2D     m_center;
		/*! A half size with negative parts, needed to render a font
		 */
		sad::Point2D     m_halfpadding;
		/*! A cached region for label
		 */
		sad::Rect2D      m_cached_region;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::Label)
