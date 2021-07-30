/*! \file stringcaster.h
    

    Defines a special caster, which converts data to stream, using std::ostringstream, if needed
    or user-defined methods
 */
#pragma once
#include "../3rdparty/format/format.h"
#include "../sadstring.h"

#include <sstream>
#include <string>

#ifdef QT_CORE_LIB
    #include<QString>
#endif

namespace sad
{

namespace log
{

/*! Converts any object to string, using std::ostringstream, for a substitution
    it to log message. 

    Instantiate a template of this class, to make output of your class possible
 */
template<typename T>
class StringCaster
{
public:
    /*! Casts object to string, for passing it to a log message
        \param[in] object an object
        \return string representation of object
     */
    static std::string cast(const T & object)
    {
        std::ostringstream s; 
        s << object;
        return s.str().c_str();
    }
};

/*! This is special type of caster, which allows writing statements, 
    like SL_CRITICAL(fmt::Print("{0}") << 5)
 */
template<>
class StringCaster<fmt::internal::ArgInserter<char> >
{
public:
    /*! This caster performs conversion from format library internal structure
        to string, allowing to pass it to a log message
        \param[in] string an inserter from inner format library
        \return string
      */
    static inline std::string cast(const fmt::internal::ArgInserter<char> & string)
    {
        // We need to perform this, since operator FormatterProxy<Char>() is non-constant
        fmt::internal::ArgInserter<char> & fmt = const_cast<fmt::internal::ArgInserter<char>&>(string); 
        return str(fmt);
    }
};

#ifdef QT_CORE_LIB

/*! Converts QString to a string, allowing to pass it to a log message
 */
template<>
class StringCaster<QString>
{
public:
    /*! Converts QString to std::string, making possible to pass it to a log
        message
        \param[in] string a string to be caster
        \return string as std::string
     */
    static inline std::string cast(const QString & string)
    {
        return string.toStdString();
    }
};

#endif

}

}

