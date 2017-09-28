/*! \file scripting.h
    

    Describes a main class for scripting objects
 */
#pragma once
#include <QObject>
#include <QSet>
#include "dukqtcontext.h"

namespace core
{
class Editor;
}

namespace scripting
{
    
/*! A group of actions, linked to animations
 */	
class Scripting: public QObject
{
Q_OBJECT
public:
/*! An enum for copying objects
 */
enum CopyPropertiesDirection
{
    SSC_CPD_FROM_HEAP_TO_GLOBAL = 0, // From stashed heap to global object
    SSC_CPD_FROM_GLOBAL_TO_HEAP = 1  // From global object to heap
};
    /*! Creates new label actions
        \param[in] parent a parent object
     */
    Scripting(QObject* parent = NULL);
    /*! This class could be inherited
     */
    virtual ~Scripting();
    /*! Returns common properties, which all classes could have
        \return common properties
     */
    QSet<QString> commonProperties();
    /*! Returns all functions and properties, defined in system
        \param[out] properties list of properties
        \param[out] functions list of functions
     */
    void propertiesAndFunctions(
        QStringList& properties,
        QStringList& functions
    );
    /*! Returns all functions and properties
        \param[out] properties list of properties
        \param[out] functions list of functions
        \param[in] get_global whether we should get global object props
     */
    void propertiesAndFunctions(
        QSet<QString> &properties,
        QSet<QString> &functions,
        bool get_global = true
     );
    /*! Returns screen width. Used, when scripting
        \return screen width
     */
    static int screenWidth();
    /*! Returns screen height. Used, when scripting
        \return screen height
     */
    static int screenHeight();
    /*! Sets an editor, where scripting object belongs to
        \param[in] e editor
     */
    void setEditor(core::Editor* e);
    /*! Returns linked editor to scripting
        \return an editor
     */
    core::Editor* editor() const;
    /*! Returns context from scripting
        \return context
     */
    dukpp03::qt::Context* context() const;
public slots:
    /*! Run script in console
     */
    void runScript();
    /*! Shows help
     */
    void showHelp();
    /*! Saves script to a file
     */
    void saveScript();
    /*! Loads script from a file
     */
    void loadScript();
protected:
    /*! Inits inner script with constructors for common types
     */
    void initSadTypeConstructors();
    /*! Copies properties from source object to destination object
        \param[in] direction a direction in which copying  is performed
     */
    void copyProperties(scripting::Scripting::CopyPropertiesDirection direction);
    /*! Copies properties from source object to destination object recursive
        \param[in] source_id position of source object on stack
        \param[in] dest_id position of destination object on stack
     */
    void copyObjectsOnStackRecursive(duk_idx_t source_id, duk_idx_t dest_id);
    /*! An editor, where scripting object belongs to
     */
    core::Editor* m_editor;
    /*! A context for Qt embedded parts
     */
    dukpp03::qt::Context* m_ctx;
    /*! A global value for editing global "E" property
     */
    dukpp03::qt::JSObject* m_global_value;
    /*! A global value for editing global "E.animations" property
     */
    dukpp03::qt::JSObject* m_animations_value;
    /*! Whether we are evaluating script at the moment
     */
    bool m_evaluating;
};

}
