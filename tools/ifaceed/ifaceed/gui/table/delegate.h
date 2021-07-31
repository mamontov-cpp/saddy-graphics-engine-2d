/*! \file gui/table/delegate.h
    

    Describes gui delegate for table or item
 */
#pragma once
#include <QObject>
// ReSharper disable once CppUnusedIncludeDirective
#include <QWidget>
#include <QTableWidget>
#include <QString>

#include "../../qstdstring.h"

#include <db/dbvariant.h>
#include <db/save.h>
#include <db/load.h>
#include <db/dbdatabase.h>
#include <db/custom/customobject.h>

#include <renderer.h>
#include <refcountable.h>


namespace core
{

class Editor;

}

namespace gui
{

namespace table
{
/*! A delegate for editing row in property table
 */
class Delegate: public QObject, public sad::RefCountable
{
Q_OBJECT
public:
    /*! Constructs default, empty delegate
     */
    Delegate();
    /*! Could be inherited
     */
    virtual ~Delegate() override;
    /*! Makes delegate to be linked to widget
        \param[in] widget a widget, linked to Delegate
        \param[in] editor a linked editor
     */
    void makeLinkedTo(QTableWidget* widget,core::Editor* editor);
    /*! Links delegate to database
     */
    void linkToDatabase();
    /*! Links delegate to custom object
        \param[in] o object
     */
    void linkToCustomObject(sad::db::custom::Object* o);
    /*! Whether delegate is linked to database
        \return whether delegate is linked to database
     */
    bool isLinkedToDatabase() const;	
    /*! Whether delegate is linked to custom object
        \return whether delegate is linked to custom object
     */
    bool isLinkedToCustomObject() const;
    /*! Sets property name for delegate
        \param[in] name a name for property
     */
    void setPropertyName(const QString & name);
    /*! Constructs a property name for delegate
     */
    const QString& propertyName() const;
    /*! Adds a delegate to table
     */
    virtual void add();
    /*! Inserts a delegate to specified place of
        table
        \param[in] row a row
     */
    virtual void insert(int row);
    /*! Sets a value for a delegate. Used by commands to set value
        \param[in] v a value
     */
    virtual void set(const sad::db::Variant& v) = 0;
    /*! Disconnect common slots from delegate
     */
    virtual void disconnectSlots();
public slots:
    /*! Removes a delegate from table
     */
    virtual void remove();
    /*! Removes with command with delegate 
     */
    virtual void removeWithCommand();
protected:
    /*! A name of property (for database or custom object)
     */
    QString m_property_name;
    /*! Whether delegate belongs to custom object (otherwise it belongs to database)
     */
    bool m_custom_object;
    /*! Determines a widget, where delegate is linked
     */
    QTableWidget* m_widget;
    /*! An editor, linked to delegate
     */
    core::Editor* m_editor;
    /*! A local widget for delegate for setting value
     */
    QWidget* m_my_widget;
    /*! A real custom object, where delegate belongs to
     */
    sad::db::custom::Object* m_object;
    /*! A row, where element should be inserted
     */
    int m_row;
    /*! At this point delegate must create own editor, set it to table and connect delegate slots
        to it.
     */
    virtual void makeEditor() = 0;
    /*! Disconnects slots  for own editor here
     */ 
    virtual void disconnect() = 0;
    /*! Fetches current value of property from database and other stuff
        \return current value
     */
    template<
        typename T
    >
    T currentValue() const
    {
        if (this->isLinkedToDatabase())
        {
            return sad::Renderer::ref()
            ->database("")
            ->getProperty<T>(Q2STDSTRING(this->propertyName())).value();
        }
        else
        {
            return m_object->getProperty<T>(Q2STDSTRING(this->propertyName())).value();
        }
        // ReSharper disable once CppUnreachableCode
        return T();
    }
    /*! Sets current value for property
        \param[in] o object
     */
    template<
        typename T
    >
    void setCurrentValue(const T & o)
    {
        if (this->isLinkedToDatabase())
        {
            sad::Renderer::ref()
            ->database("")
            ->setProperty<T>(Q2STDSTRING(this->propertyName()), o);
        }
        else
        {
            m_object->setProperty<T>(Q2STDSTRING(this->propertyName()), o);
        }
    }
    /*! Finds property in table widget
        \return row index
     */
    int findPropertyInTable() const;
    /*! Inserts linked widget to table. MUST be called inside gui::table::Delegate::createWidget to make sure, that
        widget is inserted to table
     */
    void insertToTable() const;
};

}

}

Q_DECLARE_METATYPE(gui::table::Delegate*) //-V566
