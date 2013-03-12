/*! \file   editorbehaviourshareddata.h
	\author HiddenSeeker

	Defines an editor behaviour shared data, needed  to define
	interaction with behaviours
 */
#pragma once
#include <QTimer>
#include <config/sprite2dconfig.h>
class AbstractScreenObject;

/*! A shared data for editor behaviour
 */
class EditorBehaviourSharedData
{
 private:
	 AbstractScreenObject * m_selected_object;
	 AbstractScreenObject * m_active_object;
	 bool					m_show_active_border;

	 // An icons for rendering at borders
	 Sprite2DConfig * m_icons;

	 // A code for handling deferred command for comitting a rotation goes here
	 QTimer * m_rotation_timer;			  //!< Returns current rotation timer
	 AbstractScreenObject * m_rotating_object;			  //!< AbstractScreenObject which rotation applied
	 bool     m_rotation_command_pending; //!< Whether on rotation command pending
	 float	  m_new_angle;                //!< Toggles new angle
	 float	  m_old_angle;                //!< Toggles old angle
 public:
	 /*! Sets an icons for rendering at borders 
	  */
	 inline void setIcons(Sprite2DConfig * icons)
	 {
		m_icons = icons;
	 }
	 inline Sprite2DConfig * icons() const 
	 {
		return m_icons;
	 }
	 /*! Shared data is initialized with zero selected object
	  */
	 inline EditorBehaviourSharedData() 
	 {
		 m_selected_object = NULL;
		 m_active_object = NULL;
		 m_rotation_timer = NULL;
		 m_rotation_command_pending = false;
		 m_show_active_border = true;
	 }
	 inline bool mustShowActiveBorder() { return m_show_active_border  && m_active_object != NULL;}
	 inline void toggleActiveBorder(bool flag) { m_show_active_border = flag;}
	 /*! Returns a current selected object
	  */
	 inline AbstractScreenObject * selectedObject() const
	 {
		return m_selected_object;
	 }
	 /*! Sets current selected object to a specified data
	  */
	 inline void setSelectedObject(AbstractScreenObject * o) 
	 {
		 m_selected_object = o;
	 }
	 /*! Returns a current active object
	  */
	 inline AbstractScreenObject * activeObject() const
	 {
		return m_active_object;
	 }
	 /*! Sets current active object to a specified data
	  */
	 inline void setActiveObject(AbstractScreenObject * o) 
	 {
		 m_active_object = o;
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

