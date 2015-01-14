/*! \file tovalue.h
 *  \author HiddenSeeker
 *
 *  Contains custom conversions from QScriptValue
 */
#include <QScriptValue>
#include <maybe.h>
#include <sadcolor.h>
#include <sadrect.h>
#include <sadsize.h>
#include <sadstring.h>


namespace scripting
{
/*! A class, for performing safe conversion of script value to type
 */
template<
    typename T
>
class ToValue
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<T> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to double
 */
template<
>
class ToValue<double>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<double> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to float
 */
template<
>
class ToValue<float>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<float> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to int
 */
template<
>
class ToValue<int>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<int> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to long
 */
template<
>
class ToValue<long>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<long> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to long long
 */
template<
>
class ToValue<long long>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<long long> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to sad::AColor
 */
template<
>
class ToValue<sad::AColor>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<sad::AColor> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to sad::Color
 */
template<
>
class ToValue<sad::Color>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<sad::Color> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to sad::Point2D
 */
template<
>
class ToValue<sad::Point2D>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<sad::Point2D> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to sad::Point2I
 */
template<
>
class ToValue<sad::Point2I>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<sad::Point2I> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to sad::Size2D
 */
template<
>
class ToValue<sad::Size2D>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<sad::Size2D> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to sad::Size2I
 */
template<
>
class ToValue<sad::Size2I>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<sad::Size2I> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to sad::Size2I
 */
template<
>
class ToValue<sad::Rect2D>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<sad::Rect2D> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to sad::String
 */
template<
>
class ToValue<sad::String>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<sad::String> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to std::String
 */
template<
>
class ToValue<std::string>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<std::string> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to short
 */
template<
>
class ToValue<short>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<short> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to bool
 */
template<
>
class ToValue<bool>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<bool> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to char
 */
template<
>
class ToValue<char>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<char> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to signed char
 */
template<
>
class ToValue<signed char>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<signed char> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to unsigned char
 */
template<
>
class ToValue<unsigned char>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<unsigned char> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to unsigned int
 */
template<
>
class ToValue<unsigned int>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<unsigned int> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to unsigned long
 */
template<
>
class ToValue<unsigned long>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<unsigned long> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to unsigned long long
 */
template<
>
class ToValue<unsigned long long>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<unsigned long long> perform(const QScriptValue& v);
};

/*! A class, for performing safe conversion of script value to unsigned short
 */
template<
>
class ToValue<unsigned short>
{
public:
    /*! Tries to perform conversion to specified type
     *  \param[in] v value
     *  \return value, if it exists
     */
    static sad::Maybe<unsigned short> perform(const QScriptValue& v);
};


}
