/*! \file dukqtcontext.h
    
    A helper for qt context
 */
#pragma once
#include <dukqt.h>
#include <curry1.h>
#include <object.h>

#include <QString>
#include <sadstring.h>

Q_DECLARE_METATYPE(QString*)
Q_DECLARE_METATYPE(sad::db::Object*)
Q_DECLARE_METATYPE(sad::db::Object**)
Q_DECLARE_METATYPE(sad::db::Object***)
Q_DECLARE_METATYPE(sad::String)
Q_DECLARE_METATYPE(sad::String*)

/*! A typedef for currying first part
 */
typedef dukpp03::curried1<dukpp03::qt::BasicContext> curried1;


namespace dukpp03
{

/*! An instantiation for getting sad::String
 */
template<>
class GetValue<sad::String, dukpp03::qt::BasicContext>
{
public:
    /*! Performs getting value from stack
        \param[in] ctx context
        \param[in] pos index for stack
        \return a value if it exists, otherwise empty maybe
    */
    static dukpp03::Maybe<sad::String> perform(
        dukpp03::qt::BasicContext* ctx,
        duk_idx_t pos
    );
};


/*! An instantiation for pushing sad::String on stack
*/
template<>
class PushValue<sad::String, dukpp03::qt::BasicContext>
{
public:
    /*! Performs pushing value
    \param[in] ctx context
    \param[in] v value
    */
    static void perform(dukpp03::qt::BasicContext* ctx, const sad::String& v);
};


}