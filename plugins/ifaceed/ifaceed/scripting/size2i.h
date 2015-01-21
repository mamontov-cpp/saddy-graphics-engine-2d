/*! \file size2i.h
	\author HiddenSeeker

	A sad::Size2I scriptable wrapper
 */
#pragma once
#include "classwrapper.h"

#include <sadsize.h>

namespace scripting
{

/*! A sad::Size2I scriptable wrapper
 */
class Size2I: public scripting::ClassWrapper
{
 Q_OBJECT
 Q_PROPERTY(unsigned int width READ width WRITE setWidth)
 Q_PROPERTY(unsigned int height READ height WRITE setHeight)
public:
	/*! Constructs new wrapper
	 */
	Size2I();
	/*! Constructs new wrapper
		\param[in] p point
	 */
	Size2I(const sad::Size2I& p);
	/*! Could be inherited
	 */
	virtual ~Size2I();
	/*! Converts a point to original point
	 */
	const sad::Size2I& toSize() const;
	/*! Returns width
		\param[in] width
	 */
	unsigned int width() const;
	/*! Returns height
		\param[in] height
	 */
	unsigned int height() const;
	/*! Sets width
		\param[in] width width
	 */ 
	void setWidth(unsigned int width);
	/*! Sets height
		\param[in] y height
	 */
	void setHeight(unsigned int height);
	/*! Contains to string
		\return object to string
	 */
	QString toString() const;
protected:
	/*! An inner point wrapper
	 */
	sad::Size2I m_size;
};

}
