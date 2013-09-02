/*! \file sceneaddingtask.h
	\author HiddenSeeker

	A task, which is performed, when scene is empty. It will populate it
	with sorted by layer property object
 */
#include <input.h>
#include "../objects/screentemplate.h"

class InterlockedScene;


/** A simple comparator for sorting layers of objects. Depends on layer property
 */
class LayerPropertyComparator 
{
public:    
	bool operator() (AbstractScreenObject * o1, AbstractScreenObject * o2); 
};

/*!
    \class SceneAddingTask
	A task, which is performed, when scene is empty. It will populate it
	with sorted by layer property object
 */
class SceneAddingTask: public sad::RepeatingTask
{
 private:
	 /*! A template object
	  */
	 ScreenTemplate * m_template;
	 /*! A scene, which objects must be added into
	  */
	 InterlockedScene * m_scene;
 public:
    /*! Constructs a new task
		\param[in] _template a resulting template
		\param[in]  scene scene object data
	 */
	SceneAddingTask(ScreenTemplate * _template, InterlockedScene * scene);
	/*! Populates scene when empty
	  */
	virtual void perform();
	/*! Task for adding scene
	 */
	~SceneAddingTask();
};

