/*! \file deletetexturetask.h
	\author HiddenSeeker

	A special kind of task, which could be pushed in Renderer's pipeline to delete all textures in renderer's own thread. 
	Used in resources to prevent resource leaks
 */
#pragma once

namespace sad
{

namespace util
{

/*! \class DeleteTextureTask
	A special kind of task, which could be pushed in Renderer's pipeline to delete all textures in renderer's own thread. 
	Used in resources to prevent resource leaks
 */
class DeleteTextureTask
{
public:
	/*! Creates new task
		\param[in] id a new texture task
	 */
	DeleteTextureTask(unsigned int id);
	/*! Could be inherited
	 */
	virtual ~DeleteTextureTask();
protected:
	/*! Removes linked texture by it's id
	 */ 
	virtual void _process();
	/*! An ids of textures to be deleted
	 */
	unsigned int m_texture_id;
};

}

}
