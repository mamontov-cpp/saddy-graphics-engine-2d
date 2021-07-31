/*! \file qcolortosadcolor.h
    

    Defines a converter from QColor to sad::Color, used in conversion table
 */
#pragma once
#include <QColor>

#include <sadcolor.h>

#include <db/dbconversiontable.h>


namespace core
{

namespace typeconverters
{

/*! Defines a conversion from QColor to sad::Color
 */
class QColorToSadColor: public sad::db::AbstractTypeConverter
{
public:
    /*! Creates new converter
     */
	inline QColorToSadColor() = default;
    /*! Converts source color to another color type
        \param[in] source a source color
        \param[out] dest a destination color
     */
    static void convert(
        const QColor & source,
        sad::Color & dest
    );
    /*! Converts source value from another and to another type
        \param[in] source a pointer to QColor value
        \param[in] dest a pointer to sad::Color value
     */
    virtual void convert(void * source, void * dest) override;
    /*! Can be inherited
     */
    virtual ~QColorToSadColor() override;
};

}
    
}
