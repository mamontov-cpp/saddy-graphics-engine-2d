/*! \file qcolortosadcolor.h
    

    Defines a converter from QColor to sad::AColor, used in conversion table
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <QColor>
// ReSharper disable once CppUnusedIncludeDirective
#include <sadcolor.h>

#include <db/dbconversiontable.h>


namespace core
{

namespace typeconverters
{

/*! Defines a conversion from QColor to sad::AColor
 */
class QColorToSadAColor: public sad::db::AbstractTypeConverter
{
public:
    /*! Creates new converter
     */
    inline QColorToSadAColor()
    {
        
    }
    /*! Converts source color to another color type
        \param[in] source a source color
        \param[out] dest a destination color
     */
    static void convert(
        const QColor & source,
        sad::AColor & dest
    );
    /*! Converts source value from another and to another type
        \param[in] source a pointer to QColor value
        \param[in] dest a pointer to sad::AColor value
     */
    virtual void convert(void * source, void * dest) override;
    /*! Can be inherited
     */
    virtual ~QColorToSadAColor() override;
};

}
    
}
