#include "p2d/world.h"

#include <algorithm>

DECLARE_SOBJ(sad::p2d::World);

// =============================== sad::p2d::World::GlobalBodyContainer METHODS ===============================

void sad::p2d::World::GlobalBodyContainer::performAction(const std::function<void(sad::p2d::Body*)>& f)
{
    size_t size = this->AllBodies.size();
    if (size)
    {
        sad::p2d::World::BodyWithActivityFlag* p = &(this->AllBodies[0]);
        for(size_t i = 0; i < size; i++)
        {
            if (p->Active)
            {
                sad::p2d::Body* body = p->Body;
                f(body);
            }
            p++;
        }
    }
}

void sad::p2d::World::GlobalBodyContainer::performActionWithTimeStep(
    void (sad::p2d::Body::*action)(double),
    double time_step
)
{
    this->performAction([action, time_step](sad::p2d::Body* body) {
        (body->*action)(time_step);
    });
}

void sad::p2d::World::GlobalBodyContainer::setSamplingCount(int sample_count)
{
    this->performAction([sample_count](sad::p2d::Body* body) {
         body->setSamplingCount(sample_count);
    });
}

void sad::p2d::World::GlobalBodyContainer::trySetTransformer()
{
    this->performAction([](sad::p2d::Body* body) {
        body->trySetTransformer();
    });
}

void sad::p2d::World::GlobalBodyContainer::buildBodyCaches(double time_step)
{
   void (sad::p2d::Body::*action)(double) = &sad::p2d::Body::buildCaches;
   this->performActionWithTimeStep(action, time_step);
}

void sad::p2d::World::GlobalBodyContainer::stepDiscreteChangingValues(double time_step)
{
    this->performActionWithTimeStep(
        &sad::p2d::Body::stepDiscreteChangingValues,
        time_step
    );
}

void sad::p2d::World::GlobalBodyContainer::stepPositionsAndVelocities(double time_step)
{
    this->performActionWithTimeStep(
        &sad::p2d::Body::stepPositionsAndVelocities,
        time_step
    );
}

sad::p2d::World::BodyLocation& sad::p2d::World::GlobalBodyContainer::add(sad::p2d::Body* b)
{
    if (BodyToLocation.contains(b))
    {
        return BodyToLocation[b];
    }


    size_t position = 0;
    if (FreePositions.size() != 0)
    {
        size_t last_free_position = FreePositions.size() - 1;
        size_t position = FreePositions[last_free_position];
        FreePositions.erase(FreePositions.begin() + last_free_position);
        b->addRef();

        AllBodies[position].Body = b;
        AllBodies[position].Active = true;
    }
    else
    {
        position = AllBodies.size();
        AllBodies.push_back(sad::p2d::World::BodyWithActivityFlag(b));
    }

    BodyLocation bl;
    bl.OffsetInAllBodies = position;
    BodyToLocation.insert(b, bl);
    return BodyToLocation[b];
}

void sad::p2d::World::GlobalBodyContainer::remove(sad::p2d::Body* b)
{
    if (BodyToLocation.contains(b))
    {
        BodyLocation& bl = BodyToLocation[b];
        FreePositions.push_back(bl.OffsetInAllBodies);
        if (AllBodies[bl.OffsetInAllBodies].Active)
        {
            b->delRef();
        }
        AllBodies[bl.OffsetInAllBodies].markAsInactive();

        BodyToLocation.remove(b);
    }
}

static sad::Vector<size_t> __empty_vector;

const sad::Vector<size_t>& sad::p2d::World::GlobalBodyContainer::getGroupLocations(sad::p2d::Body* b)
{
    if (BodyToLocation.contains(b))
    {
        BodyLocation& bl = BodyToLocation[b];
        return bl.PositionInGroups;
    }
    return __empty_vector;
}

void sad::p2d::World::GlobalBodyContainer::clear()
{
    size_t size = this->AllBodies.size();
    if (size)
    {
        sad::p2d::World::BodyWithActivityFlag* p = &(this->AllBodies[0]);
        for(size_t i = 0; i < size; i++)
        {
            if (p->Active)
            {
                sad::p2d::Body* body = p->Body;
                body->delRef();
            }
            p++;
        }
    }
    this->BodyToLocation.clear();
    this->AllBodies.clear();
    this->FreePositions.clear();
}

// =============================== sad::p2d::World::Group METHODS ===============================

size_t sad::p2d::World::Group::add(sad::p2d::Body* b)
{
    if (BodyToLocation.contains(b))
    {
        return BodyToLocation[b];
    }

    size_t position = 0;
    if (FreePositions.size() != 0)
    {
        size_t last_free_position = FreePositions.size() - 1;
        size_t position = FreePositions[last_free_position];
        FreePositions.erase(FreePositions.begin() + last_free_position);
        b->addRef();

        Bodies[position].Body = b;
        Bodies[position].Active = true;
    }
    else
    {
        position = Bodies.size();
        Bodies.push_back(sad::p2d::World::BodyWithActivityFlag(b));
    }

    BodyToLocation.insert(b, position);
    return position;
}



void sad::p2d::World::Group::remove(sad::p2d::Body* b)
{
    if (BodyToLocation.contains(b))
    {
        size_t bl = BodyToLocation[b];
        FreePositions.push_back(bl);
        if (Bodies[bl].Active)
        {
            b->delRef();
        }
        Bodies[bl].markAsInactive();

        BodyToLocation.remove(b);
    }
}


const sad::Vector<sad::p2d::World::BodyWithActivityFlag>& sad::p2d::World::Group::bodies() const
{
    return this->Bodies;
}

sad::Maybe<size_t> sad::p2d::World::Group::getLocation(sad::p2d::Body* b) const
{
    sad::Maybe<size_t> location;
    if (BodyToLocation.contains(b))
    {
        location.setValue(BodyToLocation[b]);
    }
    return location;
}

void sad::p2d::World::Group::clear()
{
    size_t size = this->Bodies.size();
    if (size)
    {
        sad::p2d::World::BodyWithActivityFlag* p = &(this->Bodies[0]);
        for(size_t i = 0; i < size; i++)
        {
            if (p->Active)
            {
                sad::p2d::Body* body = p->Body;
                body->delRef();
            }
            p++;
        }
    }
    this->BodyToLocation.clear();
    this->Bodies.clear();
    this->FreePositions.clear();
}

// =============================== sad::p2d::World::GroupContainer METHODS ===============================

size_t sad::p2d::World::GroupContainer::add(const sad::String& name)
{
    if (GroupToLocation.contains(name))
    {
        return GroupToLocation[name];
    }

    size_t position = 0;
    if (FreePositions.size() != 0)
    {
        size_t last_free_position = FreePositions.size() - 1;
        size_t position = FreePositions[last_free_position];
        FreePositions.erase(FreePositions.begin() + last_free_position);

        Groups[position].Group = sad::p2d::World::Group();
        Groups[position].Active = true;
    }
    else
    {
        position = Groups.size();
        Groups.push_back(sad::p2d::World::GroupWithActivityFlag());
    }

    GroupToLocation.insert(name, position);
    return position;
}



void sad::p2d::World::GroupContainer::remove(const sad::String& name)
{
    if (GroupToLocation.contains(name))
    {
        size_t bl = GroupToLocation[name];
        FreePositions.push_back(bl);
        if (Groups[bl].Active)
        {
            Groups[bl].Group.clear();
        }
        Groups[bl].markAsInactive();

        GroupToLocation.remove(name);
    }
}

void sad::p2d::World::GroupContainer::clear()
{
    size_t size = this->Groups.size();
    if (size)
    {
        sad::p2d::World::GroupWithActivityFlag* p = &(this->Groups[0]);
        for(size_t i = 0; i < size; i++)
        {
            if (p->Active)
            {
                p->Group.clear();
            }
            p++;
        }
    }
}

sad::Maybe<size_t> sad::p2d::World::GroupContainer::getLocation(const sad::String& name) const
{
    sad::Maybe<size_t> location;
    if (GroupToLocation.contains(name))
    {
        location.setValue(GroupToLocation[name]);
    }
    return location;
}

// =============================== sad::p2d::World::GlobalHandlerList METHODS ===============================

void sad::p2d::World::GlobalHandlerList::add(size_t i1, size_t i2, sad::p2d::BasicCollisionHandler* h)
{
    for(size_t i = 0; i < List.size(); i++)
    {
        if ((List[i].TypeIndex1) == i1 && (List[i].TypeIndex2 == i2))
        {
            sad::Vector<sad::p2d::BasicCollisionHandler*>& lst = List[i].List;
            if (std::find(lst.begin(), lst.end(), h) == lst.end())
            {
                lst.push_back(h);
                h->addRef();
            }
            return;
        }
    }
    sad::p2d::World::HandlerList t;
    t.TypeIndex1 = i1;
    t.TypeIndex2 = i2;
    t.List.push_back(h);

    List.push_back(t);
    h->addRef();
}

void sad::p2d::World::GlobalHandlerList::remove(sad::p2d::BasicCollisionHandler* h)
{
    for(size_t i = 0; i < List.size(); i++)
    {
        sad::Vector<sad::p2d::BasicCollisionHandler*>& lst = List[i].List;
        sad::Vector<sad::p2d::BasicCollisionHandler*>::iterator it = std::find(lst.begin(), lst.end(), h);
        if (it != lst.end())
        {
            lst.erase(it);
            h->delRef();
        }
    }
}

void sad::p2d::World::GlobalHandlerList::clear()
{
    for(size_t i = 0; i < List.size(); i++)
    {
        const sad::Vector<sad::p2d::BasicCollisionHandler*>& lst = List[i].List;
        for(size_t j = 0; j < lst.size(); j++)
        {
            lst[j]->delRef();
        }
    }
    List.clear();
}

// =============================== sad::p2d::World PUBLIC METHODS ===============================

sad::p2d::World::World() : m_time_step(1), m_is_locked(false)
{
    m_transformer = new p2d::CircleToHullTransformer(*(p2d::CircleToHullTransformer::ref()));
    m_detector = new p2d::SimpleCollisionDetector();
    m_detector->addRef();

    m_command_queue = new sad::Vector<sad::p2d::World::QueuedCommand>();
}


sad::p2d::World::~World()
{
    delete m_transformer;
    m_detector->delRef();
    m_group_container.clear();
    m_global_handler_list.clear();
    m_global_body_container.clear();

    // Clean up command queue
    for(size_t i = 0; i < m_command_queue->size(); i++)
    {
        sad::p2d::World::QueuedCommand& cmd = (*m_command_queue)[i];
        switch(cmd.Type)
        {
            case sad::p2d::World::P2D_WORLD_QCT_ADD_BODY:
            case sad::p2d::World::P2D_WORLD_QCT_REMOVE_BODY:
            case sad::p2d::World::P2D_WORLD_QCT_ADD_BODY_TO_GROUP:
            case sad::p2d::World::P2D_WORLD_QCT_REMOVE_BODY_FROM_GROUP:
            {
                cmd.Body->delRef();
                break;
            }
            case sad::p2d::World::P2D_WORLD_QCT_ADD_HANDLER:
            case sad::p2d::World::P2D_WORLD_QCT_REMOVE_HANDLER:
            {
                cmd.Handler->delRef();
                break;
            }
        }
    }
    delete m_command_queue;
}

sad::p2d::CircleToHullTransformer * sad::p2d::World::transformer()
{
    return m_transformer;
}


void sad::p2d::World::setDetector(sad::p2d::CollisionDetector * d)
{
    m_detector->delRef();
    m_detector = d;
    m_detector->addRef();
    m_global_body_container.setSamplingCount(m_detector->sampleCount());
}

double sad::p2d::World::timeStep() const
{
    return m_time_step;
}


void sad::p2d::World::setTransformer(sad::p2d::CircleToHullTransformer * t)
{
    delete m_transformer;
    m_transformer = t;
    m_global_body_container.trySetTransformer();
}

void sad::p2d::World::add(sad::p2d::Body* b)
{
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::P2D_WORLD_QCT_ADD_BODY;
        cmd.Body = b;
        b->addRef();
        addCommand(b);
    }
    else
    {
        this->addNow(b);
    }
}


// =============================== sad::p2d::World PRIVATE METHODS ===============================

bool sad::p2d::World::isLockedForChanges()
{
    m_is_locked_lock.lock();
    bool result = m_is_locked;
    m_is_locked_lock.unlock();

    return result;
}

void sad::p2d::World::setIsLockedFlag(bool is_locked)
{
    m_is_locked_lock.lock();
    m_is_locked = is_locked;
    m_is_locked_lock.unlock();
}

void sad::p2d::World::addCommand(const sad::p2d::World::QueuedCommand& c)
{
    m_command_queue_lock.lock();
    *m_command_queue << c;
    m_command_queue_lock.unlock();
}

void sad::p2d::World::performQueuedCommands()
{
    m_command_queue_lock.lock();

    if (m_command_queue->size())
    {
        sad::Vector<sad::p2d::World::QueuedCommand>* queue = m_command_queue;
        m_command_queue = new sad::Vector<sad::p2d::World::QueuedCommand>();
        for(size_t i = 0; i < queue->size(); i++)
        {
            sad::p2d::World::QueuedCommand& cmd = (*queue)[i];
            switch(cmd.Type)
            {
                case sad::p2d::World::P2D_WORLD_QCT_ADD_BODY:
                {
                    addNow(cmd.Body);
                    cmd.Body->delRef();
                    break;
                }
                case sad::p2d::World::P2D_WORLD_QCT_REMOVE_BODY:
                {
                    removeNow(cmd.Body);
                    cmd.Body->delRef();
                    break;
                }
                case sad::p2d::World::P2D_WORLD_QCT_CLEAR_BODIES:
                {
                    clearBodiesNow();
                    break;
                }
                case sad::p2d::World::P2D_WORLD_QCT_ADD_BODY_TO_GROUP:
                {
                    addBodyToGroupNow(cmd.GroupName, cmd.Body);
                    cmd.Body->delRef();
                    break;
                }
                case sad::p2d::World::P2D_WORLD_QCT_REMOVE_BODY_FROM_GROUP:
                {
                    removeBodyFromGroupNow(cmd.GroupName, cmd.Body);
                    cmd.Body->delRef();
                    break;
                }
                case sad::p2d::World::P2D_WORLD_QCT_ADD_GROUP:
                {
                    addGroupNow(cmd.GroupName);
                    break;
                }
                case sad::p2d::World::P2D_WORLD_QCT_REMOVE_GROUP:
                {
                    removeGroupNow(cmd.GroupName);
                    break;
                }
                case sad::p2d::World::P2D_WORLD_QCT_CLEAR_GROUP:
                {
                    clearGroupNow(cmd.GroupName);
                    break;
                }
                case sad::p2d::World::P2D_WORLD_QCT_CLEAR_GROUPS:
                {
                    clearGroupsNow();
                    break;
                }
                case sad::p2d::World::P2D_WORLD_QCT_ADD_HANDLER:
                {
                    addHandlerNow(cmd.GroupName, cmd.SecondGroupName, cmd.Handler);
                    cmd.Handler->delRef();
                    break;
                }
                case sad::p2d::World::P2D_WORLD_QCT_REMOVE_HANDLER:
                {
                    removeHandlerNow(cmd.Handler);
                    cmd.Handler->delRef();
                    break;
                }
                case sad::p2d::World::P2D_WORLD_QCT_CLEAR_HANDLERS:
                {
                    clearHandlersNow();
                    break;
                }
                case sad::p2d::World::P2D_WORLD_QCT_CLEAR:
                {
                    clearNow();
                    break;
                }
                case sad::p2d::World::P2D_WORLD_QCT_STEP:
                {
                    stepNow(cmd.StepValue);
                    break;
                }
            }
        }
        delete queue;
    }

    m_command_queue_lock.unlock();
}

/*
void sad::p2d::World::removeHandler(sad::p2d::BasicCollisionHandler * h)
{
    for(size_t i = 0;  i < m_callbacks.count(); i++)
    {
        if (m_callbacks[i].p2() == h)
        {
            m_callbacks.removeAt(i);
        }
    }
    delete h;
}
void sad::p2d::World::addNow(p2d::Body * b)
{
    sad::Vector<sad::String> groups;
    for(sad::Hash<sad::String, sad::Vector<p2d::Body*> > ::iterator it = m_groups.begin();
        it != m_groups.end();
        it++)
    {
        if (it.key() == "p2d::Body")
        {
            groups << it.key();
        }
        if (b->userObject() != NULL)
        {
            if (b->userObject()->metaData()->canBeCastedTo(it.key()))
            {
                groups << it.key();
            }
        }
    }
    b->addRef();
    m_allbodies.insert(b, groups);
    for(size_t i = 0; i < groups.size(); i++)
    {
        m_groups[groups[i]] << b;
    }
    b->setSamplingCount(m_detector->sampleCount());
    b->setWorld(this);
}

void sad::p2d::World::removeNow(p2d::Body * b)
{
    if (m_allbodies.contains(b))
    {
        sad::Vector<sad::String> groups = m_allbodies[b];
        for(size_t i = 0; i < groups.size(); i++)
        {
            m_groups[groups[i]].removeAll(b);
        }
        m_allbodies.remove(b);
        b->delRef();
    }
}

void sad::p2d::World::splitTimeStepAt(double time)
{
    m_splitted_time_step.setValue(time);
}

void sad::p2d::World::step(double time)
{
    performQueuedActions();
    lockChanges();
    m_time_step = time;
    while ( sad::non_fuzzy_zero(m_time_step) )
    {
        m_splitted_time_step.clear();
        m_global_body_container.buildBodyCaches(m_time_step);
        findAndExecuteCollisionCallbacks();
        if (m_splitted_time_step.exists())
        {
            m_global_body_container.stepPositionsAndVelocities(m_splitted_time_step.value());
            m_time_step -= m_splitted_time_step.value();
        }
        else
        {
            m_global_body_container.stepPositionsAndVelocities(m_time_step);
            m_time_step = 0;
        }
    }
    // Set time step to something to not bug with some interstep calls
    m_time_step = 1;
    // Step forces and body options
    m_global_body_container.stepDiscreteChangingValues(time);
    unlockChanges();
    performQueuedActions();
}

void sad::p2d::World::executeCallbacks(reactions_t & reactions)
{
    for (size_t i = 0; i < reactions.count(); ++i)
    {
        reactions[i].p2()->invoke(reactions[i].p1());
        // If time splitted, we should erase all callbacks
        if (m_splitted_time_step.exists())
        {
            reactions.clear();
        }
    }
}

void sad::p2d::World::sortCallbacks(reactions_t & reactions)
{
    std::sort(reactions.begin(), reactions.end(), p2d::World::compare);
}

void sad::p2d::World::findAndExecuteCollisionCallbacks()
{
    reactions_t reactions;
    findEvents(reactions);
    sortCallbacks(reactions);
    executeCallbacks(reactions);
}

void sad::p2d::World::findEvents(reactions_t & reactions)
{
    for (size_t i = 0; i < m_callbacks.count(); i++)
    {
        findEvent(reactions, m_callbacks[i]);
    }
}

void sad::p2d::World::findEvent(reactions_t & reactions, const types_with_handler_t & twh)
{
    double step = this->timeStep();
    if (m_groups.contains(twh.p1().p1()) && m_groups.contains(twh.p1().p2()))
    {
        sad::Vector<p2d::Body *> & g1 = m_groups[twh.p1().p1()];
        sad::Vector<p2d::Body *> & g2 = m_groups[twh.p1().p2()];
        bool not_same_group = (twh.p1().p1() != twh.p1().p2());
        p2d::BasicCollisionHandler * h = twh.p2();
        for (size_t i = 0; i < g1.count(); i++)
        {
            size_t jmin = i + 1;
            if (not_same_group) jmin = 0;
            for (size_t j = jmin; j < g2.count(); j++)
            {
                p2d::Body * b1 = g1[i];
                p2d::Body * b2 = g2[j];
                if (b1 != b2 && b1->isGhost() == false && b2->isGhost() == false)
                {
                    sad::Maybe<double> time;
                    b1->TimeStep = step;
                    b2->TimeStep = step;
                    time = m_detector->collides(b1, b2, m_time_step);
                    if (time.exists())
                    {
                        BasicCollisionEvent ev(b1, b2, time.value());
                        reactions << reaction_t(ev, h);
                    }
                }
            }
        }
    }

}



void sad::p2d::World::clearNow()
{
    // To make no problems, with iterators, step through bodies
    // as vector
    sad::Vector<p2d::Body *> bodies;
    for( bodies_to_types_t::iterator it = m_allbodies.begin();
        it != m_allbodies.end();
        ++it
       )
    {
        bodies << it.key();
    }
    for(size_t i = 0; i < bodies.count(); i++)
    {
        removeNow(bodies[i]);
    }
}

sad::p2d::BasicCollisionHandler *
sad::p2d::World::addHandler( void (*p)(const sad::p2d::BasicCollisionEvent &))
{
    sad::String b = "p2d::Body";
    return this->addHandler(b, b, p);
}

sad::p2d::BasicCollisionHandler *
sad::p2d::World::addHandler(const sad::String& g1, const sad::String& g2, void (*p)(const sad::p2d::BasicCollisionEvent &))
{
    p2d::BasicCollisionHandler * h = 
    new p2d::FunctionCollisionHandler<p2d::Body, p2d::Body>(p);
    this->addHandler(h, g1, g2);
    return h;
}

void sad::p2d::World::addToGroup(unsigned int group_code, p2d::Body* b, bool to_common)
{
    assert(m_group_hash_codes.contains(group_code) && "Unknown body group code");

    this->addToGroup(m_group_hash_codes[group_code], b, to_common);
}

void sad::p2d::World::addToGroup(const sad::String& group, p2d::Body* b, bool to_common)
{
    if (m_lock_changes)
    {
        m_command_queue_lock.lock();
        m_specified_groups_queue << sad::Triplet<bool, sad::String, p2d::Body*>(to_common, group, b);
        m_command_queue_lock.unlock();
    }
    else
    {
        m_mutability_lock.lock();
        addBodyToGroupNow(b, group,  to_common);
        m_mutability_lock.unlock();
    }
}

void sad::p2d::World::remove(p2d::Body * body)
{
    body->setUserObject(NULL);
    body->clearListeners();
    this->sad::TemporarilyImmutableContainer<p2d::Body>::remove(body);
}


unsigned int sad::p2d::World::getGroupCode(const sad::String& group, unsigned int preferred)
{
    unsigned int max = preferred;
    for(sad::Hash<unsigned int, sad::String>::iterator it =  m_group_hash_codes.begin(); it != m_group_hash_codes.end(); ++it)
    {
        if (it.value() == group)
        {
            return it.key();
        }
        max = (it.key() > max) ? (it.key() + 1) : max;
    }
    if (max == 0)
    {
        max = 1;
    }
    m_group_hash_codes.insert(max, group);
    return max;
}

void sad::p2d::World::addHandler(
         sad::p2d::BasicCollisionHandler * h, 
         const sad::String & t1, 
         const sad::String & t2
)
{
    if (m_groups.contains(t1) == false)
    {
        m_groups.insert(t1, sad::Vector<p2d::Body *>());
    }
    if (m_groups.contains(t2) == false)
    {
         m_groups.insert(t2, sad::Vector<p2d::Body *>());
    }
    type_pair_t tp(t1, t2);
    types_with_handler_t twh(tp, h);
    m_callbacks << twh;
}

void  sad::p2d::World::addBodyToGroupNow(p2d::Body* b, const sad::String& g, bool to_common)
{
    b->addRef();
    sad::Vector<sad::String> groups;
    if (to_common)
    {
        sad::String body_group = "p2d::Body";
        groups << body_group;
        if (m_groups.contains(body_group))
        {
            m_groups[body_group] << b;
        }
    }
    if (m_groups.contains(g) == false)
    {
        m_groups.insert(g, sad::Vector<sad::p2d::Body*>());
    }
    groups << g;
    m_groups[g] << b;
    m_allbodies.insert(b, groups);
    b->setSamplingCount(m_detector->sampleCount());
    b->setWorld(this);
}

void sad::p2d::World::performQueuedActions()
{
    this->sad::TemporarilyImmutableContainer<p2d::Body>::performQueuedActions();
    m_command_queue_lock.lock();
    if (m_specified_groups_queue.size())
    {
        for(size_t i = 0; i < m_specified_groups_queue.size(); i++)
        {
            this->addBodyToGroupNow(m_specified_groups_queue[i].p3(), m_specified_groups_queue[i].p2(), m_specified_groups_queue[i].p1());
        }
        m_specified_groups_queue.clear();
    }
    m_command_queue_lock.unlock();
}
*/
