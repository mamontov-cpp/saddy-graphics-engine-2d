/*! \file deletetexturetask.h
    

    A special kind of task, which could be pushed in Renderer's pipeline to delete all textures in renderer's own thread. 
    Used in resources to prevent resource leaks
 */
#pragma once
#include "../pipeline/pipelinetask.h"
#include "../sadvector.h"

namespace sad
{

namespace util
{

/*! \class DeleteTextureTask
    A special kind of task, which could be pushed in Renderer's pipeline to delete all textures in renderer's own thread. 
    Used in resources to prevent resource leaks
 */
class DeleteTextureTask: public sad::pipeline::AbstractTask
{
public:
    /*! Creates new task
        \param[in] id a new texture task
     */
    DeleteTextureTask(unsigned int id);
    /*! Creates new task
        \param[in] ids a list of textures
     */
    DeleteTextureTask(const sad::Vector<unsigned int> ids);
    /*! Could be inherited
     */
    virtual ~DeleteTextureTask();
protected:
    /*! Removes linked texture by it's id
     */ 
    virtual void _process() override;
    /*! An ids of textures to be deleted
     */
    sad::Vector<unsigned int> m_texture_ids;
};

}

}
