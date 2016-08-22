/*! \file qstdstring.h
    

    A converters to and from QString to std::string, locale-dependent 
 */
#pragma once
#include <QtGlobal>
#include <QString>
#include <string>

#ifdef WIN32

#if  (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    
    inline QString __std2qstring(const std::string& s)
    {
        if (s.size() == 0)
            return QString();
        return QString::fromLocal8Bit(s.c_str());
    }
    #define Q2STDSTRING(STR)    (std::string((STR).toLocal8Bit()))
    #define STD2QSTRING(STR)    (::__std2qstring(STR))
#else
    #define Q2STDSTRING(STR)    ((STR).toStdString())
    #define STD2QSTRING(STR)    (QString(std::string(STR).c_str()))
#endif

#else
    #define Q2STDSTRING(STR)    ((STR).toStdString())
    #define STD2QSTRING(STR)    (QString(std::string(STR).c_str()))
#endif
