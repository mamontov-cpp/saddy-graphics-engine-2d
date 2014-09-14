/*! \file   editorbehaviourshareddata.h
	\author HiddenSeeker

	Defines an editor's data, shared between all editor's states
 */
#pragma once
#include <stdio.h>

#include <QTimer>

#include <sadstring.h>
#include <scenenode.h>

#include <config/sprite2dconfig.h>

#include <hfsm/hfsmshared.h>

#include <log/log.h>

class AbstractScreenObject;


namespace core
{

class Editor;

/*! An editor's data. shared between all editor's state. Contains active and selected objects.
	Active object is an object, which will be placed (in case of state, of object adding)
 */
class Shared: public sad::hfsm::Shared
{
public:	 
	/*! Creates a default empty shared data
	 */
	Shared();
	/*! This class could be inherited
	 */
	virtual ~Shared();
	/*! Sets editor, which data belong to
		\param[in] e editor
	 */
	void setEditor(core::Editor* e);
	/*! Returns an editor, which data belons to
		\return editor
	 */
	core::Editor* editor() const;
	/*! Sets selected object
		\param[in] o object
	 */
	void setSelectedObject(sad::SceneNode* o);
	/*! Sets active object
		\param[in] o object
	 */
	void setActiveObject(sad::SceneNode* o);
	/*! Returns selected object
		\return selected object
	 */
	sad::SceneNode* selectedObject() const;
	/*! Returns active object
		\return active object
	 */
	sad::SceneNode* activeObject() const;
	/*! Could force hiding active border if set to false. You can disable force hiding by setting it to
		true
		\param[in] flag a flag value
	 */
	void toggleActiveBorder(bool flag);
	/*! Whether we can show active border
		\return whether we can show active border;
	 */
	bool canShowActiveBorder() const;
	

	 /*! Returns current rendered icons
	  */
	 inline sad::Sprite2DConfig * icons()
	 {
		 return m_icons;
	 }
	 /*! Starts a pending of rotation command
		 \param[in] t		timer
		 \param[in] rotatingobject an object, which is being rotated
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
		\param[in,out] rotatingobject an object, which is being rotated 
		\param[out] new_angle new angle which is set
		\param[out] old_angle old angle which is set
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
private:
	/*! A scene node, selected by user
	 */ 
	sad::SceneNode* m_selected;
	/*! An active scene node
	 */
	sad::SceneNode* m_active;
	/*! A linked editor, which shared data belongs to
	 */
	core::Editor* m_editor;


	 bool					m_show_active_border;  //!< A current active border


	 // An icons for rendering at borders
	 sad::Sprite2DConfig * m_icons;

	 // A code for handling deferred command for comitting a rotation goes here
	 QTimer * m_rotation_timer;			  //!< Returns current rotation timer
	 AbstractScreenObject * m_rotating_object;			  //!< AbstractScreenObject which rotation applied
	 bool     m_rotation_command_pending; //!< Whether on rotation command pending
	 float	  m_new_angle;                //!< Toggles new angle
	 float	  m_old_angle;                //!< Toggles old angle
};

}
