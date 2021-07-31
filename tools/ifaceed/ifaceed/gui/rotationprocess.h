/*! \file rotationprocess.h
    

    Describes a simple model of rotation process, which delays adding of angle change
    for scene node to history
 */
#pragma once
#include <QObject>
#include <QTimer>

#include <scenenode.h>

namespace core
{
class Editor;
}

namespace gui
{
    
/*! Defines a simple model of process of rotation, which delays adding of angle change
    for scene node to history
 */
class RotationProcess:public QObject
{
Q_OBJECT
public:
    /*! Creates new rotation process
     */
    RotationProcess();
    /*! Could be inherited
     */
    virtual ~RotationProcess();
    /*! Starts new rotation process
        \param[in] node a node
        \param[in] old_value an old value for process
        \param[in] new_value a new value for process
     */
    void start(sad::SceneNode* node, float old_value, float new_value);
    /*! Sets a editor for rotation process
        \param[in] e editor
     */
    void setEditor(core::Editor* e);
    /*! Defines a timeout for rotation process in milliseconds
     */
    static const int TIMEOUT; 
public slots:
    /*! Called, when timer is expired, makes process add angle change for
        scene node to history
     */
    void timerExpired();
protected:
    /*! A timer, which determines, whether rotation process is pending
     */
    QTimer m_timer;
    /*! Whether rotation process is pending
     */
    bool m_pending;
    /*! A linked editor
     */
    core::Editor* m_editor;
    /*! A pending node
     */
    sad::SceneNode* m_node;
    /*! An old value for angle
     */
    float m_old_value;
    /*! A new value for angle
     */
    float m_new_value;

    /*!
     * Restarts timer
     */
    void restart();
    /*!
     * Sets timeout and starts timer
     */
    void setTimeoutAndStart();
    /*!
     * Start local timer
     */
    void startLocalTimer();
    /*!
     * Starts local timer. Must be called from main thread
     */
    void startLocalTimerNow();
    /*!
     * Stops local timer
     */
    void stopLocalTimer();
};

}
