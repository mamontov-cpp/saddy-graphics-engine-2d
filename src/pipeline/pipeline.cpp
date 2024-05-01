#include "pipeline/pipeline.h"
#include "db/dbtypename.h"
#include <cassert>
#include "opticksupport.h"

sad::pipeline::Pipeline::Pipeline()
{
    PROFILER_EVENT;

}

bool sad::pipeline::Pipeline::contains(const sad::String & mark)
{
    PROFILER_EVENT;
    return findByMark(mark).p1() != nullptr;
}

void sad::pipeline::Pipeline::enableByMark(const sad::String& mark)
{
    PROFILER_EVENT;
    StepListPosition po = findByMark(mark);
    if (po.p1() != nullptr) 
    {
        (*po.p1())[po.p2()]->enable();
    }
}

void sad::pipeline::Pipeline::disableByMark(const sad::String& mark)
{
    PROFILER_EVENT;
    StepListPosition po = findByMark(mark);
    if (po.p1() != nullptr) 
    {
        (*po.p1())[po.p2()]->disable();
    }
}

bool sad::pipeline::Pipeline::isStepEnabled(const sad::String& mark)
{
    PROFILER_EVENT;
    StepListPosition po = findByMark(mark);
    if (po.p1() != nullptr) 
    {
        return (*po.p1())[po.p2()]->enabled();
    } 
    return false;
}

void sad::pipeline::Pipeline::removeByMarkWith(const sad::String& mark, bool clean_memory)
{
    PROFILER_EVENT;
    StepListPosition po = findByMark(mark);
    if (po.p1() != nullptr) 
    {
        if (clean_memory)
        {
            m_queue_for_memory_cleaning_removal << (*po.p1())[po.p2()];
        }
        else
        {
           remove((*po.p1())[po.p2()]);
        }
    }
}

void sad::pipeline::Pipeline::removeByMarkWith(const sad::String& mark)
{
    PROFILER_EVENT;
    removeByMarkWith(mark, true);
}

sad::pipeline::Step * sad::pipeline::Pipeline::insertStep(
        sad::pipeline::PipelineInsertionType type, 
        sad::pipeline::Step * step
    )
{
    PROFILER_EVENT;
    assert(
        type == sad::pipeline::PipelineInsertionType::PIT_BEGIN 
     || type == sad::pipeline::PipelineInsertionType::PIT_END
     || type == sad::pipeline::PipelineInsertionType::PIT_SYSTEM_BEFORE_FIRST_USER_ACTION
     || type == sad::pipeline::PipelineInsertionType::PIT_SYSTEM_AFTER_LAST_USER_ACTION 
    );
    sad::pipeline::PipelineInsertionData data;
    data.set1(type);
    data.set3(step);
    add(data);
    return step;
}

sad::pipeline::Step *  sad::pipeline::Pipeline::insertStep(
        sad::pipeline::PipelineInsertionType type, 
        sad::String mark,
        sad::pipeline::Step * step
    )
{
    PROFILER_EVENT;
    sad::pipeline::PipelineInsertionData data;
    data.set1(type);
    data._2().setValue(mark);
    data.set3(step);
    add(data);
    return step;
}


void sad::pipeline::Pipeline::run()
{
    PROFILER_EVENT;
    this->performQueuedActions();
    this->lockChanges();

    invokeSteps(m_system_steps_before_user);
    invokeSteps(m_user_steps);
    invokeSteps(m_system_steps_after_user);

    this->unlockChanges();
    this->performQueuedActions();
}

sad::pipeline::Step * sad::pipeline::Pipeline::append(sad::pipeline::Step * step)
{
    PROFILER_EVENT;
    step->setSource(sad::pipeline::StepSource::ST_USER);
    this->insertStep(sad::pipeline::PipelineInsertionType::PIT_END, step);
    return step;
}

sad::pipeline::Step * sad::pipeline::Pipeline::prepend(sad::pipeline::Step * step)
{
    PROFILER_EVENT;
    step->setSource(sad::pipeline::StepSource::ST_USER);
    this->insertStep(sad::pipeline::PipelineInsertionType::PIT_BEGIN, step);
    return step;	
}

sad::pipeline::Pipeline::~Pipeline()
{
    PROFILER_EVENT;
    sad::pipeline::Pipeline::StepsList * lists[3] = {
        &m_system_steps_before_user,
        &m_user_steps,
        &m_system_steps_after_user
    };
    for(int i = 0; i < 3; i++)
    {
        sad::pipeline::Pipeline::clearSteps(lists[i]);
    }
}

void sad::pipeline::Pipeline::add(const sad::pipeline::PipelineInsertionData & o)
{
    PROFILER_EVENT;
    if (o.p1() == sad::pipeline::PipelineInsertionType::PIT_END && o.p3()->source() == sad::pipeline::StepSource::ST_USER)
    {
        addNow(o);
    }
    else
    {
        this->sad::TemporarilyImmutableContainerWithHeterogeneousCommands<
            sad::pipeline::PipelineInsertionData, 
            sad::pipeline::Step *
        >::add(o);
    }
}

void sad::pipeline::Pipeline::invokeSteps(sad::pipeline::Pipeline::StepsList & steps)
{
    PROFILER_EVENT;
    for(unsigned int i = 0; i < steps.size(); i++)
    {
        steps[i]->process();
        if (steps[i]->shouldBeDestroyedAfterProcessing()) 
        {
            delete steps[i];
            steps.removeAt(i);
            --i;
        }
    }
}


sad::pipeline::Pipeline::StepListPosition 
sad::pipeline::Pipeline::findByMark(StepsList* steps, const sad::String & mark)
{
    PROFILER_EVENT;
    for(unsigned int i = 0; i < steps->size(); i++) 
    {
        sad::pipeline::Step* step= (*steps)[i];
        if (step->mark().exists())
        {
            if (step->mark().value() == mark)
            {
                return sad::pipeline::Pipeline::StepListPosition(steps, i);
            }
        }
    }
    return sad::pipeline::Pipeline::StepListPosition(nullptr, 0);
}


sad::pipeline::Pipeline::StepListPosition 
sad::pipeline::Pipeline::findByStep(StepsList* steps, sad::pipeline::Step* step)
{
    PROFILER_EVENT;
    for(unsigned int i = 0; i < steps->size(); i++) 
    {
        sad::pipeline::Step* pstep= (*steps)[i];
        if (step == pstep)
        {
            return sad::pipeline::Pipeline::StepListPosition(steps, i);
        }
    }
    return sad::pipeline::Pipeline::StepListPosition(nullptr, 0);
}

sad::pipeline::Pipeline::StepListPosition 
sad::pipeline::Pipeline::findByMark(const sad::String & mark)
{
    PROFILER_EVENT;
    sad::pipeline::Pipeline::StepsList * lists[3] = {
        &m_system_steps_before_user,
        &m_user_steps,
        &m_system_steps_after_user
    };
    sad::pipeline::Pipeline::StepListPosition pos(nullptr, 0);
    for(int i = 0; i < 3 && pos.p1() == nullptr; i++)
    {
        pos = sad::pipeline::Pipeline::findByMark(lists[i], mark);
    }
    return pos;
}


sad::pipeline::Pipeline::StepListPosition 
sad::pipeline::Pipeline::findByStep(sad::pipeline::Step* step)
{
    PROFILER_EVENT;
    sad::pipeline::Pipeline::StepsList * lists[3] = {
        &m_system_steps_before_user,
        &m_user_steps,
        &m_system_steps_after_user
    };
    sad::pipeline::Pipeline::StepListPosition pos(nullptr, 0);
    for(int i = 0; i < 3 && pos.p1() == nullptr; i++)
    {
        pos = sad::pipeline::Pipeline::findByStep(lists[i], step);
    }
    return pos;
}

void sad::pipeline::Pipeline::clearSteps(StepsList* steps)
{
    PROFILER_EVENT;
    for(unsigned int i = 0; i < steps->size(); i++)
    {
        delete (*steps)[i];
    }
    steps->clear();
}

void sad::pipeline::Pipeline::addNow(PipelineInsertionData o)
{
    PROFILER_EVENT;
    switch(o.p1())
    {
        case sad::pipeline::PipelineInsertionType::PIT_BEGIN: 
        {
            if (o.p3()->source() == sad::pipeline::StepSource::ST_SYSTEM)
                m_system_steps_before_user.insert(o.p3(), 0);
            else
                m_user_steps.insert(o.p3(), 0);
            break;
        }
        case sad::pipeline::PipelineInsertionType::PIT_END:
        {
            if (o.p3()->source() == sad::pipeline::StepSource::ST_SYSTEM)
                m_system_steps_after_user << o.p3();
            else
                m_user_steps << o.p3();
            break;
        }
        case sad::pipeline::PipelineInsertionType::PIT_SYSTEM_BEFORE_FIRST_USER_ACTION:
        {
            m_system_steps_before_user << o.p3();
            break;
        }
        case sad::pipeline::PipelineInsertionType::PIT_SYSTEM_AFTER_LAST_USER_ACTION:
        {
            m_system_steps_after_user << o.p3();
            break;
        }
        case sad::pipeline::PipelineInsertionType::PIT_BEFORE:
        {
            StepListPosition pos = findByMark(o.p2().value());
            StepsList * list = pos.p1();
            size_t position = pos.p2();
            if (list != nullptr)
            {
                list->insert(o.p3(), position);
            }
            break;
        }
        case sad::pipeline::PipelineInsertionType::PIT_AFTER:
        {
            StepListPosition pos = findByMark(o.p2().value());
            StepsList * list = pos.p1();
            size_t position = pos.p2();
            if (list != nullptr)
            {
                if (list->size() - 1 == position)
                {
                    (*list) << o.p3();
                } 
                else
                {
                    list->insert(o.p3(), position + 1);
                }
            }
            break;
        }
    };
}

void sad::pipeline::Pipeline::removeNow(sad::pipeline::Step * o)
{
    PROFILER_EVENT;
    removeFromPipeline(o, false);
}

void sad::pipeline::Pipeline::removeFromPipeline(sad::pipeline::Step * o, bool clean_memory)
{
    PROFILER_EVENT;
    StepListPosition pos = findByStep(o);
    StepsList * list = pos.p1();
    size_t position = pos.p2();
    if (list != nullptr)
    {
        list->removeAt(position);
        if (clean_memory)
        {
            delete o;
        }
    }
}

void sad::pipeline::Pipeline::clearNow()
{
    PROFILER_EVENT;
    sad::pipeline::Pipeline::clearSteps(&m_user_steps);
}

void sad::pipeline::Pipeline::performQueuedActions()
{
    PROFILER_EVENT;
    m_command_queue_lock.lock();
    if (m_queue_for_memory_cleaning_removal.size())
    {
        for(size_t i = 0; i < m_queue_for_memory_cleaning_removal.size(); i++)
        {
            removeFromPipeline(m_queue_for_memory_cleaning_removal[i], true);
        }
        m_queue_for_memory_cleaning_removal.clear();
    }
    m_command_queue_lock.unlock();
    this->sad::TemporarilyImmutableContainerWithHeterogeneousCommands<
        sad::pipeline::PipelineInsertionData, 
        sad::pipeline::Step *
    >::performQueuedActions();
}


void sad::pipeline::Pipeline::appendStateTransition(
    sad::hfsm::Machine * machine, 
    const sad::String & state
)
{
    PROFILER_EVENT;
    append(new sad::hfsm::MachineStateChangeTask(machine, state) );
}

DECLARE_COMMON_TYPE(sad::pipeline::Pipeline);
