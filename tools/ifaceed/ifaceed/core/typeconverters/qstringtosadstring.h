/*! \file qstringtosadstring.h
    

    Defines a converter from QString to sad::String, used in conversion table
 */
#pragma once
#include <QString>

#include <sadstring.h>

#include <db/dbconversiontable.h>


namespace core
{

namespace typeconverters
{

/*! Defines a conversion from  QString to sad::String
 */
class QStringToSadString: public sad::db::AbstractTypeConverter
{
public:
    /*! Creates new converter
     */
	inline QStringToSadString() = default;
    /*! Converts source value from another and to another type
        \param[in] source a pointer to sad::String value
        \param[in] dest a pointer to QString value
     */
    virtual void convert(void * source, void * dest) override;
    /*! Can be inherited
     */
    virtual ~QStringToSadString() override;
};

}
    
}
