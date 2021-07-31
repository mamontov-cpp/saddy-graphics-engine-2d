/*! \file sadacolortoqcolor.h
    

    Defines a converter from sad::AColor to QColor, used in conversion table
 */
#pragma once
#include <QColor>

#include <sadcolor.h>

#include <db/dbconversiontable.h>


namespace core
{

namespace typeconverters
{
    
/*! Defines a conversion from sad::AColor to QColor
 */
class SadAColorToQColor: public sad::db::AbstractTypeConverter
{
public:
    /*! Creates new converter
     */
	inline SadAColorToQColor() = default;
    /*! Converts source color to another color type
        \param[in] source a source color
        \param[out] dest a destination color
     */
    static void convert(
        const sad::AColor& source,
        QColor& dest
    );
    /*! Converts source value from another and to another type
        \param[in] source a pointer to sad::AColor value
        \param[in] dest a pointer to QColor value
     */
    virtual void convert(void* source, void* dest) override;
    /*! Can be inherited
     */
    virtual ~SadAColorToQColor() override;
};

}
    
}
