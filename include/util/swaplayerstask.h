/*! \file swaplayerstask.h
    

    Here is defined task, used to swap layers in renderer
 */
#pragma once
#include "../pipeline/pipelinetask.h"

namespace sad
{

class Scene;
class Renderer;

namespace util
{

/*! A task, used to swap layers in renderer
 */
class SwapLayersTask: public sad::pipeline::AbstractTask
{
public:
    /*! Constructs new task
        \param[in] r a renderer
        \param[in] s1 a first scene
        \param[in] s2 a second scene
        \param[in] l1 a layer for  first scene
        \param[in] l2 a layer for  second scene
     */
    SwapLayersTask(sad::Renderer* r, sad::Scene * s1, sad::Scene * s2, int l1, int l2);
    /*! Swaps layers for a renderer
     */ 
    virtual void _process() override;
    /*! Destroys swap object
     */
    virtual ~SwapLayersTask() override;
protected:
    /*! A renderer
     */
    sad::Renderer* m_renderer;
    /*! A first scene
     */
    sad::Scene* m_scene1;
    /*! A second scene
     */
    sad::Scene* m_scene2;
    /*! A layer for first scene
     */
    int m_layer1;
    /*! A layer for second scene
     */
    int m_layer2;
};

}

}
