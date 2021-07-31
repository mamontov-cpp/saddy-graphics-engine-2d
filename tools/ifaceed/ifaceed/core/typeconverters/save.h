/*! \file save.h
    

    Defines overloads for saving data
 */
#pragma once
#include <QColor>
#include <QString>
// ReSharper disable once CppUnusedIncludeDirective
#include <QVector>

#include <db/save.h>

#include "qlistqlistqcolortosadvectorsadvectoracolor.h"
#include "qrectftosadrect2d.h"

#include "../qstdstring.h"

namespace sad
{

namespace db
{
    
/*! Specification for saving QColor values
 */
template<>
class Save<QColor>
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    QColor * src = static_cast<QColor *>(ptr);
    sad::AColor dest(src->red(), src->green(), src->blue(), src->alpha());
    return sad::db::Save<sad::AColor>::perform(&dest);
}

};

/*! Specification for saving QString values
 */
template<>
class Save<QString>
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    QString * src = static_cast<QString *>(ptr);
    sad::String dest(Q2STDSTRING(*src));
    return sad::db::Save<sad::String>::perform(&dest);
}

};

/*! Specification for saving QVector<QVector<QColor> > values
 */
template<>
class Save<QList<QList<QColor> > >
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    QList<QList<QColor> >  * src = static_cast<QList<QList<QColor> > *>(ptr);
    sad::Vector<sad::Vector<sad::AColor> > result;

    core
    ::typeconverters
    ::QListQListQColorToSadVectorSadVectorToAColor
    ::convert(*src, result);
    return sad::db::Save<sad::Vector<sad::Vector<sad::AColor> > >::perform(&result);
}

};


/*! Specification for saving QRectF values
 */
template<>
class Save<QRectF >
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    QRectF* src = static_cast<QRectF *>(ptr);
    sad::Rect2D result;

    core
    ::typeconverters
    ::QRectFToSadRect2D
    ::convert(*src, result);
    return sad::db::Save<QRectF>::perform(&result);
}

};

}

}
