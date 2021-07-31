/*! \file sadvectorsadvectoracolortoqlistqlistqcolor.h
    

    Defines conversion from sad::Vector<sad::Vector<sad::AColor> > to QList<QList<QColor> >.
 */
#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <QVector>
#include <QColor>

#include <sadvector.h>
#include <sadcolor.h>

#include <db/dbconversiontable.h>

namespace core
{

namespace typeconverters
{

/*! Defines a conversion from sad::Vector<sad::Vector<sad::AColor> > to QList<QList<QColor> >.
 */
class SadVectorSadVectorToAColorToQListQListQColor: public sad::db::AbstractTypeConverter
{
public:
    /*! Creates new converter
     */
	inline SadVectorSadVectorToAColorToQListQListQColor() = default;
    /*! Converts source value to dest value
        \param[in] src source value
        \param[in] dest destination value
     */
    static void convert(
        const sad::Vector<sad::Vector<sad::AColor> > & src,
        QList<QList<QColor> > & dest
    );
    /*! Converts source value from another and to another type
        \param[in] source a pointer to sad::Vector<sad::Vector<sad::AColor> > value
        \param[in] dest a pointer to QList<QList<QColor> > value
     */
    virtual void convert(void * source, void * dest) override;
    /*! Can be inherited
     */
    virtual ~SadVectorSadVectorToAColorToQListQListQColor() override;
};

}
    
}
