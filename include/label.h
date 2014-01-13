/*! \file   label.h
    \author HiddenSeeker

	Contains a definition of simple label node, that can be added to scene
*/
#include "scene.h"
#include "font.h"
#include "sadrect.h"
#include "3rdparty/format/format.h"
#include "timer.h"
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
        /*! Renders a string of text inside of label
		*/
		virtual void render();
		/*! Returns a boundign region for a label
		 */
		virtual sad::Rect2D region() const; 
		/*! Returns a font for label
			\return font, which is label is being rendered with
		 */
		inline sad::Font * font() const
		{
			return m_font;
		}
		/*! Sets a font, that label is being rendered with			
			\param[in] font a font
		 */
		void setFont(sad::Font * font);
	    /*! Sets new font by trying to get it from a rendered
			\param[in] name a name for a font
			\param[in] r a renderer
		 */
		void setFont(const sad::String & name, sad::Renderer * r = NULL);
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
		inline float angle() const
		{
			return m_angle;
		}
		/*! Sets angle for rotation of label
			\param[in] angle a rotation angle
		 */
		inline void setAngle(float angle)
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
private:
		/*! Recomputes rotation coefficients, 
			so rotation will be placed just in place 
		 */
		void recomputeRenderingPoint();
		sad::Font  *     m_font;       //!< A font, that label is being renderd with
		sad::String      m_string;     //!< A rendered string
		/*! An upper left point, where text is rendered
		 */
		sad::Point2D     m_point;  
		/*! A counter-clockwise rotation angle for a label
		 */
		float            m_angle;
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
};

}
