/*! \file qrectftosadrect2d.h
    

    Defines a converter from  QRectF to sad::Rect2D, used in conversion table
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
class QRectFToSadRect2D: public sad::db::AbstractTypeConverter
{
public:
    /*! Creates new converter
     */
	inline QRectFToSadRect2D() = default;
    /*!
     * Converts source value to another type
     * \param source a source rect
     * \param dest a destination rect
     */
    static void convert(
        const QRectF& source,
        sad::Rect2D& dest
    );
    /*! Converts source value from another and to another type
        \param[in] source a pointer to sad::String value
        \param[in] dest a pointer to QString value
     */
    virtual void convert(void * source, void * dest) override;
    /*! Can be inherited
     */
    virtual ~QRectFToSadRect2D() override;
};

}

}
