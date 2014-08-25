/*! \file load.h
	\author HiddenSeeker

	Defines overloads for loading data
 */
#pragma once
#include <QColor>
#include <QString>
#include <QVector>

#include <db/save.h>

#include "sadvectorsadvectoracolortoqvectorqvectorqcolor.h"


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
		*reinterpret_cast<QString *>(ptr) = QString(s.c_str());                            
	}                                                            
	return cast.exists();                                        
}                                                                
};


template<>                                                       
class Load<QVector<QVector<QColor> > >                                               
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
		QVector<QVector<QColor> > & dest = *reinterpret_cast<QVector<QVector<QColor> > *>(ptr);
		
		core
		::typeconverters
		::SadVectorSadVectorToAColorToQVectorQVectorQColor
		::convert(src, dest);
	}                                                            
	return cast.exists();                                        
}                                                                
};

}

}
