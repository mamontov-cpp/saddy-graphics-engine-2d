/*! \file sadstringtoqstring.h
    

    Defines a converter from sad::String to QString, used in conversion table
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <QString>

// ReSharper disable once CppUnusedIncludeDirective
#include <sadstring.h>

#include <db/dbconversiontable.h>


namespace core
{

namespace typeconverters
{

/*! Defines a conversion from sad::String to QString
 */
class SadStringToQString: public sad::db::AbstractTypeConverter
{
public:
    /*! Creates new converter
     */
	inline SadStringToQString() = default;
    /*! Converts source value from another and to another type
        \param[in] source a pointer to sad::String value
        \param[in] dest a pointer to QString value
     */
    virtual void convert(void * source, void * dest) override;
    /*! Can be inherited
     */
    virtual ~SadStringToQString()  override;
};

}
    
}
