/*! \file load.h
	

	Defines overloads for loading data
 */
#pragma once
#include <QColor>
#include <QString>
#include <QVector>

#include <db/save.h>

#include "sadvectorsadvectoracolortoqlistqlistqcolor.h"
#include "sadrect2dtoqrectf.h"

#include "../../qstdstring.h"

namespace sad
{

namespace db
{

template<>                                                       
class Load<QColor>                                               
{                                                                
public:                                                          
static bool perform(void * ptr, const picojson::value & v)       
{                                                               
	if (!ptr)                                                    
		throw sad::db::InvalidPointer();                         
	sad::Maybe<sad::AColor>  cast = picojson::to_type<sad::AColor>(v);     
	if (cast.exists())                                           
	{                 
		const sad::AColor & clr = cast.value();
		*reinterpret_cast<QColor *>(ptr) = QColor(clr.r(), clr.g(), clr.b(), clr.a());                            
	}                                                            
	return cast.exists();                                        
}                                                                
};  

template<>                                                       
class Load<QString>                                               
{                                                                
public:                                                          
static bool perform(void * ptr, const picojson::value & v)       
{                                                               
	if (!ptr)                                                    
		throw sad::db::InvalidPointer();                         
	sad::Maybe<sad::String>  cast = picojson::to_type<sad::String>(v);     
	if (cast.exists())                                           
	{                 
		const sad::String & s = cast.value();
		*reinterpret_cast<QString *>(ptr) = STD2QSTRING(s.c_str());                            
	}                                                            
	return cast.exists();                                        
}                                                                
};


template<>                                                       
class Load<QList<QList<QColor> > >                                               
{                                                                
public:                                                          
static bool perform(void * ptr, const picojson::value & v)       
{                                                               
	if (!ptr)                                                    
		throw sad::db::InvalidPointer();                         
	sad::Maybe<sad::Vector<sad::Vector<sad::AColor> > >  cast = picojson::to_type<
		sad::Vector<sad::Vector<sad::AColor> > 
	>(v);     
	if (cast.exists())                                           
	{                 
		const sad::Vector<sad::Vector<sad::AColor> > & src = cast.value();
		QList<QList<QColor> > & dest = *reinterpret_cast<QList<QList<QColor> > *>(ptr);
		
		core
		::typeconverters
		::SadVectorSadVectorToAColorToQListQListQColor
		::convert(src, dest);
	}                                                            
	return cast.exists();                                        
}                                                                
};

template<>
class Load<QRectF >
{
public:
static bool perform(void * ptr, const picojson::value & v)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    sad::Maybe<sad::Rect2D>  cast = picojson::to_type<
        sad::Rect2D
    >(v);
    if (cast.exists())
    {
        const sad::Rect2D & src = cast.value();
        QRectF& dest = *reinterpret_cast<QRectF*>(ptr);

        core
        ::typeconverters
        ::SadRect2DToQRectF
        ::convert(src, dest);
    }
    return cast.exists();
}
};

}

}
