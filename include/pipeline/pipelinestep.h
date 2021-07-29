/*! \file pipelinestep.h
    

    Describes a base class, for step of pipeline, which should define a process, performed in
    main pipeline, a task which should be performed once and be removed from a pipeline.

    Also pipeline step could be marked, which gives user a power to insert steps at end or add
    them after specified item.
 */
#pragma once
#include "../maybe.h"
#include "../sadstring.h"

namespace sad
{

namespace pipeline
{

/*! Defines, where step came from - the main saddy infrastructure or user code.

    Note, that sad::Scene::render will have user as step source.
 */
enum class StepSource: int
{
    /*! A system type defines a type of step, added by sad::Renderer and other related classed
        on insertion to beginning they are being inserted to a beginning of pipeline, and on appending
        they are inserted to an end of container. User UNDER ANY CIRCUMSTANCES should not use this type
        when adding tasks
     */
    ST_SYSTEM,
    /*! A user step is inserted before rendering scene, if inserted to beginning and after last user step, when
        adding to an end to container
     */
    ST_USER
};

/*! A base class, for step of pipeline, which should define a process, performed in
    main pipeline, a task which should be performed once and be removed from a pipeline.

    Also pipeline step could be marked, which gives user a power to insert steps at end or add
    them after specified item.
 */
class Step
{
public:
    /*! Default step has no mark, and also it cannot be located.
        By default - 
     */
    inline Step() : m_enabled(true), m_source(sad::pipeline::StepSource::ST_USER)
    {
    }
    /*! You can inherit step and implement your own steps
     */
    virtual ~Step();
    /*! Marks step with specified mark
        \param[in] mark specified mark
     */
    void mark(const sad::String & mark);
    /*! Returns mark for step, if any presented
        \return mark for a step
     */
    const sad::Maybe<sad::String> mark() const;
    /*! Returns source for a pipeline step
     */
    sad::pipeline::StepSource source() const;
    /*! Sets a source for a pipeline step
        \param[in] source a pipeline source
     */
    void setSource(sad::pipeline::StepSource source);
    /*! Processes step, invoking a user actions
     */
    virtual void process();
    /*! Enables a pipeline step
     */
    virtual void enable();
    /*! Disables a pipeline step
     */
    virtual void disable();
    /*! Returns if step is enabled
     */
    virtual bool enabled() const;
    /*! Changes object for all method call. Object is casted down to method
        \param[in] o object for method call
     */
    virtual void changeObject(void * o);
    /*! Determines, whether after processing we should remove step from pipeline
        \return whether we should remove step from pipeline
     */
    virtual bool shouldBeDestroyedAfterProcessing() = 0;
protected:
    /*! Reimplement this function to create your own step actions
     */ 
    virtual void _process() = 0;
    /*! Whether we should enable or disable step invocation
     */
    bool m_enabled;
    /*! A mark, uniquely identifiying a tep
     */
    sad::Maybe<sad::String> m_mark;
    /*! Defines a source for a step
     */ 
    sad::pipeline::StepSource m_source;
};

}

}