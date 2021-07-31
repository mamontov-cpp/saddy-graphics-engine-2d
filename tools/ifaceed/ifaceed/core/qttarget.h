/*! \file qttarget.h
    

    Describes a QtTarget as class, that whole purpose is to show message boxes
    on fatal cases
 */
#pragma once
#include <log/log.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <QString>
// ReSharper disable once CppUnusedIncludeDirective
#include <sstream>
// ReSharper disable once CppUnusedIncludeDirective
#include <QMessageBox>
#pragma once


namespace core
{
class Editor;

/*! A target, which converts a warning and critical messages to a
    user, using GUI
 */
class QtTarget:public sad::log::Target
{
 protected:
        bool m_enabled;  //!< Whether we should enable showing messages
        core::Editor * m_editor; //!< Editor information
        /*! Sends a critical message
         */
        void critical(const sad::String & m);
        /*! Sends a warning message
         */
        void warning(const sad::String & m);
 public:
        /*! Creates a new disabled target
         */
        inline QtTarget(core::Editor * ed)
        {
            m_enabled = false;
            m_editor  = ed;
        }
        /*! Enables a QtTarget
         */
        inline void enable() { m_enabled = true; }
        /*! Disables a QtTarget
         */
        inline void disable() { m_enabled = false;}
        /*! Receives a messages from targetting information
            \param[in] message taken message
         */
        virtual void receive(const sad::log::Message & message) override;
        ~QtTarget() override;
};

}
