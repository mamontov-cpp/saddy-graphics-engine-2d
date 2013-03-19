/*! \file ifaceshareddata.h
	\author HiddenSeeker

	Creates  an editor shared data with information of shared data
 */
#include "../editorcore/editorbehaviourshareddata.h"
#include <QTimer>
#include <config/sprite2dconfig.h>
#pragma once


class IFaceEditor;
class EditorLog;
class IFaceSharedData: public EditorBehaviourSharedData
{
private:
	 // An editor log for shared data
	 IFaceEditor      * m_editor;
	 // An icons for rendering at borders
	 Sprite2DConfig * m_icons;

	 // A code for handling deferred command for comitting a rotation goes here
	 QTimer * m_rotation_timer;			  //!< Returns current rotation timer
	 AbstractScreenObject * m_rotating_object;			  //!< AbstractScreenObject which rotation applied
	 bool     m_rotation_command_pending; //!< Whether on rotation command pending
	 float	  m_new_angle;                //!< Toggles new angle
	 float	  m_old_angle;                //!< Toggles old angle
public:
	 /*! Sets a current editor
		 \param[in] e editor
	  */
	 inline void setEditor(IFaceEditor * e) { m_editor = e;}
	 /*! Returns current rendered icons
	  */
	 Sprite2DConfig * icons();
	 /*! Returns log information
		 \return log
	  */
	 EditorLog * log();
	 /*! Shared data is initialized with zero selected object
	  */
	 inline IFaceSharedData(): EditorBehaviourSharedData()
	 {
		 m_rotation_timer = NULL;
		 m_rotation_command_pending = false;
	 }
	 /*! Starts a pending of rotation command
		 \param[in] t		timer
		 \param[in] angle	angle
		 \param[in] clean_start whether rotation started clean
		 \param[in] old_angle prevous angle
	  */
	 inline void submitRotationCommand(QTimer * t, 
									   AbstractScreenObject * rotatingobject, 
									   float angle, 
									   bool clean_start, float old_angle = 0.0f)
	 {
		 if (m_rotation_timer) {
			m_rotation_timer->stop();
		 }
		 m_rotating_object = rotatingobject;
		 delete m_rotation_timer;
		 m_rotation_timer = t;
		 m_rotation_command_pending = true;
		 m_new_angle = angle;
		 if (clean_start)
			 m_old_angle = old_angle;
	 }
	 /*! Returns true, whether rotation command is pending
	  */
	 inline bool isRotationCommandPending() const 
	 {
		return m_rotation_command_pending;
	 }
	 /*! Dismisses a rotation command if needed
		\param[out] new_angle new angle which is set
		\param[out[ old_angle old angle which is set
	  */
	 inline void getAndDismissRotationCommand(AbstractScreenObject *& rotatingobject, float & new_angle, float & old_angle)
	 {
		 delete  m_rotation_timer;
		 m_rotation_timer = NULL;
		 rotatingobject = m_rotating_object;
		 m_rotation_command_pending = false;
		 new_angle = m_new_angle;
		 old_angle = m_old_angle;
	 }
};
