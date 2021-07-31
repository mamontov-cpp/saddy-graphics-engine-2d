/*! \file animationprocess.h
    

    Describes a simple model of animation process, which polls an animation instance after specified time
    and performs a cleanup if it needed, erasing timer
 */
#pragma once
#include <QObject>
#include <QTimer>
#include <QMutex>

#include <animations/animationsinstance.h>

namespace core
{
class Editor;
}

namespace gui
{
    
/*! Defines a simple model of process of animation playing, polling with specified frequency an instance
    of animation, whether it's finished
 */
class AnimationProcess:public QObject
{
Q_OBJECT
public:
    /*! Creates new rotation process
     */
    AnimationProcess();
    /*! Could be inherited
     */
    virtual ~AnimationProcess();
    /*! Starts new instance, adds it to animations pipeline, performs locking actions in editor
     */
    void start(sad::animations::Instance* i);
    /*! Stops an animation instance. Totally.
     */
    void stop();
    /*! Sets a editor for rotation process
        \param[in] e editor
     */
    void setEditor(core::Editor* e);
    /*! Defines a timeout for polling an animation instance, whether it finished in milliseconds
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
    /*! A mutex, for locking operations on flag and main panel
     */
    QMutex m_mutex;
    /*! A linked editor
     */
    core::Editor* m_editor;
    /*! A pending instance, launched inside of renderer
     */
    sad::animations::Instance* m_instance;
};

}
