/*! \file sadrect2dtoqrectf.h
    

    Defines a converter from sad::Rect2D to QRectF, used in conversion table
 */
#pragma once
#include <QRectF>

#include <sadrect.h>

#include <db/dbconversiontable.h>


namespace core
{

namespace typeconverters
{

/*! Defines a conversion from sad::String to QString
 */
class SadRect2DToQRectF: public sad::db::AbstractTypeConverter
{
public:
    /*! Creates new converter
     */
	inline SadRect2DToQRectF() = default;
    /*!
     * Converts source value to another type
     * \param source a source rect
     * \param dest a destination rect
     */
    static void convert(
        const sad::Rect2D& source,
        QRectF& dest
    );
    /*! Converts source value from another and to another type
        \param[in] source a pointer to sad::String value
        \param[in] dest a pointer to QString value
     */
    virtual void convert(void * source, void * dest) override;
    /*! Can be inherited
     */
    virtual ~SadRect2DToQRectF() override;
};

}

}
