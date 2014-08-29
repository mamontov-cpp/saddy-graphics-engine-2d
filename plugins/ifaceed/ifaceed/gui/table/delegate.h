/*! \file gui/table/delegate.h
	\author HiddenSeeker

	Describes gui delegate for table or item
 */
#pragma once
#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QString>

namespace core
{

class Editor;

}

namespace gui
{

namespace table
{
	
class Delegate: public QObject
{
Q_OBJECT
public:
	/*! Constructs default, empty delegate
	 */
	Delegate();
	/*! Could be inherited
	 */
	virtual ~Delegate();
	/*! Maked delegate linked to widget
		\param[in] widget a widget, linked to Delegate
		\param[in] editor a linked editor
	 */
	void makeLinkedTo(QTableWidget* widget,core::Editor* editor);
	/*! Links delegate to database
	 */
	void linkToDatabase();
	/*! Links delegate to custom object
	 */
	void linkToCustomObject();
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
	/*! Make editor for delegate
		\return editor
	 */
	virtual QWidget* editor() = 0;
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
};

}

}