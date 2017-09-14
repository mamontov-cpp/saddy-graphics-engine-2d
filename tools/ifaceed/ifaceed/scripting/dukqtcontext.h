/*! \file dukqtcontext.h
    
    A helper for qt context
 */
#pragma once
#include <dukqt.h>
#include <curry1.h>
#include <object.h>

#include <QString>
#include <QStringList>
#include <sadvector.h>
#include <sadstring.h>
#include <sadpoint.h>
#include <sadrect.h>
#include <sadcolor.h>

/*! Makes binding types easier by declaring  meta types for T, T* and T** respectively
 */
#define Q_DUKPP03_DECLARE_METATYPE(T) Q_DECLARE_METATYPE(T) Q_DECLARE_METATYPE(T*) Q_DECLARE_METATYPE(T**)

Q_DECLARE_METATYPE(QString*)
Q_DECLARE_METATYPE(QStringList*)
Q_DUKPP03_DECLARE_METATYPE(sad::db::Object*)
Q_DUKPP03_DECLARE_METATYPE(sad::String)
Q_DUKPP03_DECLARE_METATYPE(sad::Vector<sad::String>)
Q_DUKPP03_DECLARE_METATYPE(sad::Point2D)
Q_DUKPP03_DECLARE_METATYPE(sad::Point2I)
Q_DUKPP03_DECLARE_METATYPE(sad::Point3D)
Q_DUKPP03_DECLARE_METATYPE(sad::Point3I)
Q_DUKPP03_DECLARE_METATYPE(sad::Size2D)
Q_DUKPP03_DECLARE_METATYPE(sad::Size2I)
Q_DUKPP03_DECLARE_METATYPE(sad::Rect2D)
Q_DUKPP03_DECLARE_METATYPE(sad::Rect2I)
Q_DUKPP03_DECLARE_METATYPE(sad::Color)
Q_DUKPP03_DECLARE_METATYPE(sad::AColor)


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

/*! An instantiation for getting QStringList
*/
template<>
class GetValue<QStringList, dukpp03::qt::BasicContext>
{
public:
    /*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
    */
    static dukpp03::Maybe<QStringList> perform(
        dukpp03::qt::BasicContext* ctx,
        duk_idx_t pos
    );
};


/*! An instantiation for getting sad::Vector<sad::String>
*/
template<>
class GetValue<sad::Vector<sad::String>, dukpp03::qt::BasicContext>
{
public:
    /*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
    */
    static dukpp03::Maybe<sad::Vector<sad::String> > perform(
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

/*! An instantiation for pushing QStringList on stack
 */
template<>
class PushValue<QStringList, dukpp03::qt::BasicContext>
{
public:
    /*! Performs pushing value
        \param[in] ctx context
        \param[in] v value
    */
    static void perform(dukpp03::qt::BasicContext* ctx, const QStringList& v);
};

/*! An instantiation for pushing sad::Vector<sad::String> on stack
 */
template<>
class PushValue<sad::Vector<sad::String>, dukpp03::qt::BasicContext>
{
public:
    /*! Performs pushing value
    \param[in] ctx context
    \param[in] v value
    */
    static void perform(dukpp03::qt::BasicContext* ctx, const sad::Vector<sad::String>& v);
};

}