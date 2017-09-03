/*! \file scripting.h
    

    Describes a main class for scripting objects
 */
#pragma once
#include <QObject>
#include <QScriptEngine>
#include <QVector>
#include <QThread>
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
    /*! Returns an engine
        \return engine
     */
    QScriptEngine* engine() const;
    /*! Registers function in object
       \param name name of function
       \param v a value representation of function
     */
    void registerFunction(const QString& name, QScriptValue& v);
    /*! Registerd script class as global function
     */
    void registerScriptClass(const QString& name, QScriptClass* c);
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
    /*! Cancels execution of script
     */
    void cancelExecution();
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
    /*! Inits all bindings for scenes
        \param[out] v a global value (E)
    */
    void initDatabasePropertyBindings(QScriptValue& v);
    /*! Inits scenes bindings for scenes
        \param[out] v a global value (E)
     */
    void initSceneBindings(QScriptValue& v);
    /*! Inits scene nodes bindings
        \param[out] v a global value (E)
     */
    void initSceneNodesBindings(QScriptValue& v);
    /*! Inits layout grid bindings
        \param[out] v a global value (E)
     */
    void initLayoutGridBindings(QScriptValue& v);
    /*! Inits way bindings
        \param[out] v a global value (E)
     */
    void initWaysBindings(QScriptValue& v);
    /*! Inits dialogues bindings
        \param[out] v a global value (E)
     */
    void initDialoguesBindings(QScriptValue& v);
    /*! Inits animations bindings
        \param[out] v a global value (E)
     */
    void initAnimationsBindings(QScriptValue& v);
    /*! Inits animation instances bindings
        \param[out] v a global value (E)
     */
    void initAnimationInstanceBindings(QScriptValue& v);
    /*! Inits animation groups bindings
        \param[out] v a global value (E)
     */
    void initAnimationGroupBindings(QScriptValue& v);
    /*! Copies properties from source object to destination object
        \param[in] direction a direction in which copying  is performed
     */
    void copyProperties(scripting::Scripting::CopyPropertiesDirection direction);
    /*! An editor, where scripting object belongs to
     */
    core::Editor* m_editor;
    /*! An engine to be run
     */
    QScriptEngine* m_engine;
    /*! A value, where all bindings are stored
     */
    QScriptValue m_value;
    /*! A list of registered classes in engine
     */
    QVector<QScriptClass*> m_registered_classes;
    /*! A flags to be set as property
     */
    QScriptValue::PropertyFlags m_flags;

    /*! A context for Qt embedded parts
     */
    dukpp03::qt::Context* m_ctx;
    /*! Whether we are evaluating script at the moment
     */
    bool m_evaluating;
};

}
