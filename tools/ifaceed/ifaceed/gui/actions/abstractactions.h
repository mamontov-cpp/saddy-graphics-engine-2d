/*! \file abstractactions.h
    
    Defines an abstract actions, linked to editor
 */
#pragma once
#include <QComboBox>
// ReSharper disable once CppUnusedIncludeDirective
#include <QListWidget>
#include <QListWidgetItem>
#include <QVariant>

namespace sad
{

class String;

namespace db
{
    class Object;
}

}

namespace core
{
class Editor;
}

namespace gui
{

namespace actions 
{

class AbstractActions
{
public:
    /*! An abstract actions, for an editor
     */
    AbstractActions();
    /*! Sets editor for an abstract actions
        \param[in] editor an editor
     */
    virtual void setEditor(core::Editor* editor);
    /*! Returns inner editor
        \return editor
     */
    core::Editor* editor() const; 
    /*! Can be inherited
     */
    virtual ~AbstractActions();
    /*! Tries to find an item to a list
        \param[in] w widget
        \param[in] user_data a data
        \return index
     */
    template<
        typename T
    >
    static unsigned int findInList(QListWidget* w, T user_data)
    {
        size_t pos = - 1;
        for(size_t i = 0; i < w->count(); i++)
        {
            if (w->item(i)->data(Qt::UserRole).value<T>() == user_data)
            {
                pos = i;
            }
        }
        return pos;
    }
    /*! Tries to find an item to a combo box
        \param[in] w widget
        \param[in] user_data a data
        \return index (-1 if not found)
     */
    template<
        typename T
    >
    static int findInComboBox(QComboBox* w, T user_data)
    {
        int pos = - 1;
        for(size_t i = 0; i < w->count(); i++)
        {
            if (w->itemData(i, Qt::UserRole).value<T>() == user_data)
            {
                pos = static_cast<int>(i);
            }
        }
        return pos;
    }
    /*! Tries to find an item to a combo box by major id
        \param[in] w widget
        \param[in] id id to identify item in combo box
        \return index (-1 if not found)
     */
    template<
        typename T
    >
    static int findInComboBoxByMajorId(QComboBox* w, unsigned long long id)
    {
        int pos = - 1;
        for(size_t i = 0; i < w->count(); i++)
        {
            T tmp = w->itemData(i, Qt::UserRole).value<T>(); 
            if (tmp)
            {
                if (tmp->MajorId == id)
                {
                    pos = static_cast<int>(i);
                }
            }
            else
            {
                if (id == 0)
                {
                    pos = static_cast<int>(i);
                }
            }
        }
        return pos;
    }
    /*! Fetches object name, which can be used in object list
        \param[in] o object
        \return object name
     */
    static QString viewableObjectName(sad::db::Object* o);
protected:
    /*!  A link to a local editor
     */
    core::Editor* m_editor;
};

}

}