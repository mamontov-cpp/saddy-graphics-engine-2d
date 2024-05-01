#include "p2d/world.h"
#include "collection.h"
#include "opticksupport.h"

DECLARE_SOBJ(sad::p2d::World);

// =============================== sad::p2d::World::GlobalBodyContainer METHODS ===============================

void sad::p2d::World::GlobalBodyContainer::performAction(const std::function<void(sad::p2d::Body*)>& f)
{
    PROFILER_EVENT;
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

void sad::p2d::World::GlobalBodyContainer::setSamplingCount(int sample_count)
{
    PROFILER_EVENT;
    this->performAction([sample_count](sad::p2d::Body* body) {
         body->setSamplingCount(sample_count);
    });
}

void sad::p2d::World::GlobalBodyContainer::trySetTransformer()
{
    PROFILER_EVENT;
    this->performAction([](sad::p2d::Body* body) {
        body->trySetTransformer();
    });
}

void sad::p2d::World::GlobalBodyContainer::buildBodyCaches(double time_step)
{
    PROFILER_EVENT;
   this->performAction([time_step](sad::p2d::Body* body) {
       body->buildCaches(time_step);
   });
}

void sad::p2d::World::GlobalBodyContainer::stepDiscreteChangingValues(double time_step)
{
    PROFILER_EVENT;
    this->performAction([time_step](sad::p2d::Body* body) {
        body->stepDiscreteChangingValues(time_step);
    });
}

void sad::p2d::World::GlobalBodyContainer::stepPositionsAndVelocities(double time_step)
{
    PROFILER_EVENT;
    this->performAction([time_step](sad::p2d::Body* body) {
        body->stepPositionsAndVelocities(time_step);
    });
}

sad::p2d::World::BodyLocation& sad::p2d::World::GlobalBodyContainer::add(sad::p2d::Body* b)
{
    PROFILER_EVENT;
    if (BodyToLocation.contains(b))
    {
        return BodyToLocation[b];
    }


    size_t position;
    if (!FreePositions.empty())
    {
        size_t last_free_position = FreePositions.size() - 1;
        position = FreePositions[last_free_position];
        FreePositions.erase(FreePositions.begin() + last_free_position);
        b->addRef();

        AllBodies[position].Body = b;
        AllBodies[position].Active = true;
    }
    else
    {
        b->addRef();
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
    PROFILER_EVENT;
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
    PROFILER_EVENT;
    if (BodyToLocation.contains(b))
    {
        BodyLocation& bl = BodyToLocation[b];
        return bl.PositionInGroups;
    }
    return __empty_vector;
}

void sad::p2d::World::GlobalBodyContainer::clear()
{
    PROFILER_EVENT;
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

void sad::p2d::World::GlobalBodyContainer::removeFromGroup(sad::p2d::Body* b, size_t group_offset)
{
    PROFILER_EVENT;
    if (BodyToLocation.contains(b))
    {
        sad::p2d::World::BodyLocation& loc = BodyToLocation[b];
        loc.PositionInGroups.removeAll(group_offset);
        if (loc.PositionInGroups.empty())
        {
            this->remove(b);
        }
    }
}

size_t sad::p2d::World::GlobalBodyContainer::bodyCount()
{
    PROFILER_EVENT;
    size_t size = this->AllBodies.size();
    size_t result = 0;
    if (size)
    {
        sad::p2d::World::BodyWithActivityFlag* p = &(this->AllBodies[0]);
        for(size_t i = 0; i < size; i++)
        {
            if (p->Active)
            {
                result += 1;
            }
            p++;
        }
    }
    return result;
}

sad::Vector<sad::p2d::Body*> sad::p2d::World::GlobalBodyContainer::activeBodies()
{
    PROFILER_EVENT;
    size_t size = this->AllBodies.size();
    sad::Vector<sad::p2d::Body*> result;
    if (size)
    {
        sad::p2d::World::BodyWithActivityFlag* p = &(this->AllBodies[0]);
        for (size_t i = 0; i < size; i++)
        {
            if (p->Active)
            {
                result.push_back(p->Body);
            }
            p++;
        }
    }
    return result;
}

// =============================== sad::p2d::World::Group METHODS ===============================

size_t sad::p2d::World::Group::add(sad::p2d::Body* b)
{
    PROFILER_EVENT;
    if (BodyToLocation.contains(b))
    {
        return BodyToLocation[b];
    }

    size_t position;
    if (!FreePositions.empty())
    {
        size_t last_free_position = FreePositions.size() - 1;
        position = FreePositions[last_free_position];
        FreePositions.erase(FreePositions.begin() + last_free_position);
        b->addRef();

        Bodies[position].Body = b;
        Bodies[position].Active = true;
    }
    else
    {
        position = Bodies.size();
        b->addRef();
        Bodies.push_back(sad::p2d::World::BodyWithActivityFlag(b));
    }

    BodyToLocation.insert(b, position);
    return position;
}



void sad::p2d::World::Group::remove(sad::p2d::Body* b)
{
    PROFILER_EVENT;
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
    PROFILER_EVENT;
    return this->Bodies;
}

sad::Maybe<size_t> sad::p2d::World::Group::getLocation(sad::p2d::Body* b) const
{
    PROFILER_EVENT;
    sad::Maybe<size_t> location;
    if (BodyToLocation.contains(b))
    {
        location.setValue(BodyToLocation[b]);
    }
    return location;
}

void sad::p2d::World::Group::clear()
{
    PROFILER_EVENT;
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

size_t sad::p2d::World::Group::bodyCount()
{
    PROFILER_EVENT;
    size_t size = this->Bodies.size();
    size_t result=  0;
    if (size)
    {
        sad::p2d::World::BodyWithActivityFlag* p = &(this->Bodies[0]);
        for(size_t i = 0; i < size; i++)
        {
            if (p->Active)
            {
                result += 1;
            }
            p++;
        }
    }
    return result;
}

sad::Vector<sad::p2d::Body*> sad::p2d::World::Group::activeBodies()
{
    PROFILER_EVENT;
    size_t size = this->Bodies.size();
    sad::Vector<sad::p2d::Body*>  result;
    if (size)
    {
        sad::p2d::World::BodyWithActivityFlag* p = &(this->Bodies[0]);
        for (size_t i = 0; i < size; i++)
        {
            if (p->Active)
            {
                result.push_back(p->Body);
            }
            p++;
        }
    }
    return result;

}


// =============================== sad::p2d::World::GroupContainer METHODS ===============================

size_t sad::p2d::World::GroupContainer::add(const sad::String& name)
{
    PROFILER_EVENT;
    if (GroupToLocation.contains(name))
    {
        return GroupToLocation[name];
    }

    size_t position;
    if (!FreePositions.empty())
    {
        size_t last_free_position = FreePositions.size() - 1;
        position = FreePositions[last_free_position];
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
    PROFILER_EVENT;
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
    PROFILER_EVENT;
    clearBodies();
    GroupToLocation.clear();
    FreePositions.clear();
    Groups.clear();
}

void sad::p2d::World::GroupContainer::clearBodies()
{
    PROFILER_EVENT;
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

void sad::p2d::World::GroupContainer::makeGroupAndAddBody(const sad::String& group_name, sad::p2d::Body* body, sad::p2d::World::BodyLocation& loc)
{
    PROFILER_EVENT;
    size_t location = add(group_name);
    sad::p2d::World::Group& group = this->Groups[location].Group;
    if (!group.BodyToLocation.contains(body))
    {
        group.add(body);
        loc.PositionInGroups.push_back(location);
    }
}

size_t sad::p2d::World::GroupContainer::groupCount()
{
    PROFILER_EVENT;
    size_t size = this->Groups.size();
    size_t result = 0;
    if (size)
    {
        sad::p2d::World::GroupWithActivityFlag* p = &(this->Groups[0]);
        for(size_t i = 0; i < size; i++)
        {
            if (p->Active)
            {
                result += 1;
            }
            p++;
        }
    }
    return result;
}

sad::Vector<sad::String> sad::p2d::World::GroupContainer::existingGroups() const
{
    PROFILER_EVENT;
    sad::Vector<sad::String> result;
    for(sad::Hash<sad::String, size_t>::const_iterator it = GroupToLocation.const_begin();
        it != GroupToLocation.const_end();
        ++it)
    {
        result.push_back(it.key());
    }
    return result;
}

sad::Maybe<size_t> sad::p2d::World::GroupContainer::getLocation(const sad::String& name) const
{
    PROFILER_EVENT;
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
    PROFILER_EVENT;
    for(size_t i = 0; i < List.size(); i++)
    {
        if ((List[i].TypeIndex1) == i1 && (List[i].TypeIndex2 == i2))
        {
            sad::Vector<sad::p2d::BasicCollisionHandler*>& lst = *(List[i].List);
            if (std::find(lst.begin(), lst.end(), h) == lst.end())
            {
                lst.push_back(h);
                h->addRef();
            }
            return;
        }
    }
    sad::p2d::World::HandlerList t;  // NOLINT(cppcoreguidelines-pro-type-member-init)
    t.TypeIndex1 = i1;
    t.TypeIndex2 = i2;
    t.List = new sad::Vector<sad::p2d::BasicCollisionHandler*>();
    t.List->push_back(h);

    List.push_back(t);
    h->addRef();
}

void sad::p2d::World::GlobalHandlerList::remove(size_t i1, size_t i2, sad::p2d::BasicCollisionHandler* h)
{
    PROFILER_EVENT;
    for(size_t i = 0; i < List.size(); i++)
    {
        if ((List[i].TypeIndex1) == i1 && (List[i].TypeIndex2 == i2))
        {
            sad::Vector<sad::p2d::BasicCollisionHandler*>& lst = *(List[i].List);
            sad::Vector<sad::p2d::BasicCollisionHandler*>::iterator it = std::find(lst.begin(), lst.end(), h);
            if (it != lst.end())
            {
                lst.erase(it);
                h->delRef();
            }
            if (lst.empty())
            {
                delete List[i].List;
                List.removeAt(i);
            }
            return;
        }
    }
}

void sad::p2d::World::GlobalHandlerList::remove(sad::p2d::BasicCollisionHandler* h)
{
    PROFILER_EVENT;
    for(size_t i = 0; i < List.size(); i++)
    {
        sad::Vector<sad::p2d::BasicCollisionHandler*>& lst = *(List[i].List);
        sad::Vector<sad::p2d::BasicCollisionHandler*>::iterator it = std::find(lst.begin(), lst.end(), h);
        if (it != lst.end())
        {
            lst.erase(it);
            h->delRef();
        }
        if (lst.empty())
        {
            delete List[i].List;
            List.removeAt(i);
            --i;
        }
    }
}



void sad::p2d::World::GlobalHandlerList::clear()
{
    PROFILER_EVENT;
    for(size_t i = 0; i < List.size(); i++)
    {
        const sad::Vector<sad::p2d::BasicCollisionHandler*>& lst = *(List[i].List);
        for(size_t j = 0; j < lst.size(); j++)
        {
            lst[j]->delRef();
        }
        delete List[i].List;
    }
    List.clear();
}

void sad::p2d::World::GlobalHandlerList::clearForGroups(size_t i1, size_t i2)
{
    PROFILER_EVENT;
    for(size_t i = 0; i < List.size(); i++)
    {
        if ((List[i].TypeIndex1) == i1 && (List[i].TypeIndex2 == i2))
        {
            const sad::Vector<sad::p2d::BasicCollisionHandler*>& lst = *(List[i].List);
            for(size_t j = 0; j < lst.size(); j++)
            {
                lst[j]->delRef();
            }
            delete List[i].List;
            List.removeAt(i);
            --i;
        }
    }
}

void sad::p2d::World::GlobalHandlerList::removeHandlersFor(size_t location)
{
    PROFILER_EVENT;
    for (size_t i = 0; i < List.size(); i++)
    {
        if ((List[i].TypeIndex1 == location) || (List[i].TypeIndex2 == location))
        {
            const sad::Vector<sad::p2d::BasicCollisionHandler*>& lst = *(List[i].List);
            for (size_t j = 0; j < lst.size(); j++)
            {
                lst[j]->delRef();
            }
            delete List[i].List;
            List.removeAt(i);
            --i;
        }
    }
}

size_t sad::p2d::World::GlobalHandlerList::totalHandlerCount()
{
    PROFILER_EVENT;
    size_t result = 0;
    for (size_t i = 0; i < List.size(); i++)
    {
        if (List[i].List)
        {
            result += List[i].List->size();
        }
    }
    return result;
}

size_t sad::p2d::World::GlobalHandlerList::totalHandlerCount(size_t i1, size_t i2)
{
    PROFILER_EVENT;
    size_t result = 0;

    for (size_t i = 0; i < List.size(); i++)
    {
        if ((List[i].TypeIndex1 == i1) && (List[i].TypeIndex2 == i2))
        {
           if (List[i].List)
           {
               result +=  List[i].List->size();
           }
        }
    }

    return result;
}

size_t sad::p2d::World::GlobalHandlerList::totalHandlerOccurrences(sad::p2d::BasicCollisionHandler* h)
{
    PROFILER_EVENT;
    size_t result = 0;
    for(size_t i = 0; i < List.size(); i++)
    {
        sad::Vector<sad::p2d::BasicCollisionHandler*>& lst = *(List[i].List);
        sad::Vector<sad::p2d::BasicCollisionHandler*>::iterator it = std::find(lst.begin(), lst.end(), h);
        if (it != lst.end())
        {
            result++;
        }
    }
    return result;
}

size_t sad::p2d::World::GlobalHandlerList::totalHandlerOccurrences(size_t i1, size_t i2, sad::p2d::BasicCollisionHandler* h)
{
    PROFILER_EVENT;
    size_t result = 0;
    for(size_t i = 0; i < List.size(); i++)
    {
        if ((List[i].TypeIndex1 == i1) && (List[i].TypeIndex2 == i2))
        {
            sad::Vector<sad::p2d::BasicCollisionHandler*>& lst = *(List[i].List);
            sad::Vector<sad::p2d::BasicCollisionHandler*>::iterator it = std::find(lst.begin(), lst.end(), h);
            if (it != lst.end())
            {
                result++;
            }
        }
    }
    return result;
}

sad::Vector<sad::p2d::BasicCollisionHandler*> sad::p2d::World::GlobalHandlerList::handlers()
{
    PROFILER_EVENT;

    sad::Vector<sad::p2d::BasicCollisionHandler*> result;
    for (size_t i = 0; i < List.size(); i++)
    {
        sad::Vector<sad::p2d::BasicCollisionHandler*>& lst = *(List[i].List);
        result << lst;
    }
    std::sort(result.begin(), result.end());
    (void)std::unique(result.begin(), result.end());
    return result;
}

sad::Vector<sad::p2d::BasicCollisionHandler*> sad::p2d::World::GlobalHandlerList::handlers(size_t i1, size_t i2)
{
    PROFILER_EVENT;
    sad::Vector<sad::p2d::BasicCollisionHandler*> result;
    for (size_t i = 0; i < List.size(); i++)
    {
        if ((List[i].TypeIndex1 == i1) && (List[i].TypeIndex2 == i2))
        {
            if (List[i].List)
            {
                result = *(List[i].List);
                return result;
            }
        }
    }
    return result;
}

// =============================== sad::p2d::World PUBLIC METHODS ===============================

sad::p2d::World::World() : m_time_step(1), m_is_locked(false)
{
    PROFILER_EVENT;
    m_transformer = new p2d::CircleToHullTransformer(*(p2d::CircleToHullTransformer::ref()));
    m_detector = new p2d::SimpleCollisionDetector();
    m_detector->addRef();

    m_command_queue = new sad::Vector<sad::p2d::World::QueuedCommand>();
}


sad::p2d::World::~World()
{
    PROFILER_EVENT;
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
            case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_ADD_BODY:
            case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_BODY:
            case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_ADD_BODY_TO_GROUP:
            case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_BODY_FROM_GROUP:
            {
                cmd.Body->delRef();
                break;
            }
            case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_ADD_HANDLER:
            case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_HANDLER_FROM_GROUPS:
            case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_HANDLER:
            {
                cmd.Handler->delRef();
                break;
            }
            default: break;
        }
    }
    delete m_command_queue;
}

sad::p2d::CircleToHullTransformer * sad::p2d::World::transformer() const
{
    PROFILER_EVENT;
    return m_transformer;
}


void sad::p2d::World::setDetector(sad::p2d::CollisionDetector * d)
{
    PROFILER_EVENT;
    m_detector->delRef();
    m_detector = d;
    m_detector->addRef();
    m_global_body_container.setSamplingCount(m_detector->sampleCount());
}

sad::p2d::CollisionDetector* sad::p2d::World::detector() const
{
    PROFILER_EVENT;
    return m_detector;
}

double sad::p2d::World::timeStep() const
{
    PROFILER_EVENT;
    return m_time_step;
}


void sad::p2d::World::setTransformer(sad::p2d::CircleToHullTransformer * t)
{
    PROFILER_EVENT;
    delete m_transformer;
    m_transformer = t;
    m_global_body_container.trySetTransformer();
}

void sad::p2d::World::addBody(sad::p2d::Body* b)
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_ADD_BODY;
        cmd.Body = b;
        b->addRef();
        addCommand(cmd);
    }
    else
    {
        this->addBodyNow(b);
    }
}

void sad::p2d::World::removeBody(sad::p2d::Body* b)
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_BODY;
        cmd.Body = b;
        b->addRef();
        addCommand(cmd);
    }
    else
    {
        this->removeBodyNow(b);
    }
}

void sad::p2d::World::clearBodies()
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_CLEAR_BODIES;
        addCommand(cmd);
    }
    else
    {
        this->clearBodiesNow();
    }
}

void sad::p2d::World::addBodyToGroup(const sad::String& group_name, sad::p2d::Body* b)
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_ADD_BODY_TO_GROUP;
        cmd.Body = b;
        cmd.GroupName = group_name;
        b->addRef();
        addCommand(cmd);
    }
    else
    {
        this->addBodyToGroupNow(group_name, b);
    }
}

void sad::p2d::World::removeBodyFromGroup(const sad::String& group_name, sad::p2d::Body* b)
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_BODY_FROM_GROUP;
        cmd.Body = b;
        cmd.GroupName = group_name;
        b->addRef();
        addCommand(cmd);
    }
    else
    {
        this->removeBodyFromGroupNow(group_name, b);
    }
}

void sad::p2d::World::clearGroup(const sad::String& group_name)
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_CLEAR_GROUP;
        cmd.GroupName = group_name;
        addCommand(cmd);
    }
    else
    {
        this->clearGroupNow(group_name);
    }
}


void sad::p2d::World::addGroup(const sad::String& group_name)
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_ADD_GROUP;
        cmd.GroupName = group_name;
        addCommand(cmd);
    }
    else
    {
        this->addGroupNow(group_name);
    }
}

void sad::p2d::World::removeGroup(const sad::String& group_name)
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_GROUP;
        cmd.GroupName = group_name;
        addCommand(cmd);
    }
    else
    {
        this->removeGroupNow(group_name);
    }
}


void sad::p2d::World::clearGroups()
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_CLEAR_GROUPS;
        addCommand(cmd);
    }
    else
    {
        this->clearGroupsNow();
    }
}

sad::p2d::BasicCollisionHandler* sad::p2d::World::addHandler(
    const sad::String & first_group_name,
    const sad::String & second_group_name,
    sad::p2d::BasicCollisionHandler* h
)
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_ADD_HANDLER;
        cmd.GroupName = first_group_name;
        cmd.SecondGroupName = second_group_name;
        cmd.Handler = h;
        h->addRef();
        addCommand(cmd);
    }
    else
    {
        this->addHandlerNow(first_group_name, second_group_name, h);
    }
    return h;
}

void sad::p2d::World::removeHandler(sad::p2d::BasicCollisionHandler *h)
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_HANDLER;
        cmd.Handler = h;
        h->addRef();
        addCommand(cmd);
    }
    else
    {
        this->removeHandlerNow(h);
    }
}

void sad::p2d::World::removeHandlerFromGroups(
    const sad::String & first_group_name,
    const sad::String & second_group_name,
    sad::p2d::BasicCollisionHandler* h
)
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_HANDLER_FROM_GROUPS;
        cmd.GroupName = first_group_name;
        cmd.SecondGroupName = second_group_name;
        cmd.Handler = h;
        h->addRef();
        addCommand(cmd);
    }
    else
    {
        this->removeHandlerFromGroups(first_group_name, second_group_name, h);
    }
}

void sad::p2d::World::clearHandlers()
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_CLEAR_HANDLERS;
        addCommand(cmd);
    }
    else
    {
        this->clearHandlersNow();
    }
}

void sad::p2d::World::clearHandlersForGroups(const sad::String& first_group, const sad::String& second_group)
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_CLEAR_HANDLERS_FOR_GROUPS;
        cmd.GroupName = first_group;
        cmd.SecondGroupName = second_group;
        addCommand(cmd);
    }
    else
    {
        this->clearHandlersForGroupsNow(first_group, second_group);
    }
}

void sad::p2d::World::clear()
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_CLEAR;
        addCommand(cmd);
    }
    else
    {
        this->clearNow();
    }
}

void sad::p2d::World::step(double time)
{
    PROFILER_EVENT;
    if (isLockedForChanges())
    {
        sad::p2d::World::QueuedCommand cmd;
        cmd.Type = sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_STEP;
        cmd.StepValue = time;
        addCommand(cmd);
    }
    else
    {
        this->stepNow(time);
    }
}

bool sad::p2d::World::isBodyInWorld(sad::p2d::Body* b)
{
    PROFILER_EVENT;
    bool result = false;
    m_world_lock.lock();

    if (m_global_body_container.BodyToLocation.contains(b))
    {
        result = true;
    }

    m_world_lock.unlock();

    return result;
}

bool sad::p2d::World::isInGroup(const sad::String& group_name, sad::p2d::Body* b)
{
    PROFILER_EVENT;
    bool result = false;
    m_world_lock.lock();

    sad::Maybe<size_t> location = m_group_container.getLocation(group_name);
    if (location.exists())
    {
        if (m_group_container.Groups[location.value()].Active)
        {
            sad::p2d::World::Group& g = m_group_container.Groups[location.value()].Group;
            if (g.BodyToLocation.contains(b))
            {
                result = true;
            }
        }
    }

    m_world_lock.unlock();
    return result;
}

bool sad::p2d::World::doesGroupExists(const sad::String& group_name)
{
    PROFILER_EVENT;
    bool result = false;
    m_world_lock.lock();

    sad::Maybe<size_t> location = m_group_container.getLocation(group_name);
    if (location.exists())
    {
        if (m_group_container.Groups[location.value()].Active)
        {
            result = true;
        }
    }

    m_world_lock.unlock();
    return result;
}

size_t sad::p2d::World::totalBodyCount()
{
    PROFILER_EVENT;
    m_world_lock.lock();

    size_t result = m_global_body_container.bodyCount();

    m_world_lock.unlock();
    return result;
}

size_t sad::p2d::World::totalGroupCount()
{
    PROFILER_EVENT;
    m_world_lock.lock();

    size_t result = m_group_container.groupCount();

    m_world_lock.unlock();
    return result;
}

size_t sad::p2d::World::amountOfBodiesInGroup(const sad::String& group_name)
{
    PROFILER_EVENT;
    size_t result = 0;
    m_world_lock.lock();

    sad::Maybe<size_t> location = m_group_container.getLocation(group_name);
    if (location.exists())
    {
        if (m_group_container.Groups[location.value()].Active)
        {
            result = m_group_container.Groups[location.value()].Group.bodyCount();
        }
    }

    m_world_lock.unlock();
    return result;
}

sad::Vector<sad::p2d::Body*> sad::p2d::World::allBodies()
{
    PROFILER_EVENT;
    m_world_lock.lock();

    sad::Vector<sad::p2d::Body*> result = m_global_body_container.activeBodies();

    m_world_lock.unlock();
    return result;
}

sad::Vector<sad::String> sad::p2d::World::existingGroups()
{
    PROFILER_EVENT;
    m_world_lock.lock();

    sad::Vector<sad::String> result = m_group_container.existingGroups();

    m_world_lock.unlock();
    return result;
}

sad::Vector<sad::p2d::Body*> sad::p2d::World::allBodiesInGroup(const sad::String& group_name)
{
    PROFILER_EVENT;
    sad::Vector<sad::p2d::Body*> result;
    m_world_lock.lock();

    sad::Maybe<size_t> location = m_group_container.getLocation(group_name);
    if (location.exists())
    {
        if (m_group_container.Groups[location.value()].Active)
        {
            result = m_group_container.Groups[location.value()].Group.activeBodies();
        }
    }

    m_world_lock.unlock();
    return result;
}

size_t sad::p2d::World::amountOfHandlers()
{
    PROFILER_EVENT;
    m_world_lock.lock();

    size_t result = m_global_handler_list.totalHandlerCount();

    m_world_lock.unlock();
    return result;
}

size_t sad::p2d::World::amountOfHandlersForGroups(const sad::String& s1, const sad::String& s2)
{
    PROFILER_EVENT;
    size_t result = 0;
    m_world_lock.lock();

    sad::Maybe<size_t> location1 = m_group_container.getLocation(s1);
    sad::Maybe<size_t> location2 = m_group_container.getLocation(s2);

    if (location1.exists() && location2.exists())
    {
        result = m_global_handler_list.totalHandlerCount(location1.value(), location2.value());
    }
    m_world_lock.unlock();

    return result;
}

size_t sad::p2d::World::totalHandlerOccurrences(sad::p2d::BasicCollisionHandler* h)
{
    PROFILER_EVENT;
    m_world_lock.lock();

    size_t result = m_global_handler_list.totalHandlerOccurrences(h);

    m_world_lock.unlock();

    return result;
}

size_t sad::p2d::World::totalHandlerOccurrencesInGroups(const sad::String& s1, const sad::String& s2, sad::p2d::BasicCollisionHandler* h)
{
    PROFILER_EVENT;
    size_t result = 0;
    m_world_lock.lock();

    sad::Maybe<size_t> location1 = m_group_container.getLocation(s1);
    sad::Maybe<size_t> location2 = m_group_container.getLocation(s2);

    if (location1.exists() && location2.exists())
    {
        result = m_global_handler_list.totalHandlerOccurrences(location1.value(), location2.value(), h);
    }
    m_world_lock.unlock();

    return result;
}

bool sad::p2d::World::isHandlerInWorld(sad::p2d::BasicCollisionHandler* h)
{
    PROFILER_EVENT;
    return totalHandlerOccurrences(h) != 0;
}

bool sad::p2d::World::isHandlerInGroups(const sad::String& s1, const sad::String& s2, sad::p2d::BasicCollisionHandler* h)
{
    PROFILER_EVENT;
    return totalHandlerOccurrencesInGroups(s1, s2, h) != 0;
}

sad::Vector<sad::p2d::BasicCollisionHandler*> sad::p2d::World::allHandlers()
{
    PROFILER_EVENT;
    m_world_lock.lock();

    sad::Vector<sad::p2d::BasicCollisionHandler*> result =  m_global_handler_list.handlers();

    m_world_lock.unlock();

    return result;
}

sad::Vector<sad::p2d::BasicCollisionHandler*> sad::p2d::World::allHandlersForGroups(const sad::String& s1, const sad::String& s2)
{
    PROFILER_EVENT;
    sad::Vector<sad::p2d::BasicCollisionHandler*> result;
    m_world_lock.lock();

    sad::Maybe<size_t> location1 = m_group_container.getLocation(s1);
    sad::Maybe<size_t> location2 = m_group_container.getLocation(s2);

    if (location1.exists() && location2.exists())
    {
        result = m_global_handler_list.handlers(location1.value(), location2.value());
    }

    m_world_lock.unlock();

    return result;
}

// =============================== sad::p2d::World PRIVATE METHODS ===============================

bool sad::p2d::World::isLockedForChanges()
{
    PROFILER_EVENT;
    m_is_locked_lock.lock();
    bool result = m_is_locked;
    m_is_locked_lock.unlock();

    return result;
}

void sad::p2d::World::setIsLockedFlag(bool is_locked)
{
    PROFILER_EVENT;
    m_is_locked_lock.lock();
    m_is_locked = is_locked;
    m_is_locked_lock.unlock();
}

void sad::p2d::World::addCommand(const sad::p2d::World::QueuedCommand& c)
{
    PROFILER_EVENT;
    m_command_queue_lock.lock();
    *m_command_queue << c;
    m_command_queue_lock.unlock();
}

void sad::p2d::World::performQueuedCommands()
{
    PROFILER_EVENT;
    m_command_queue_lock.lock();

    if (!m_command_queue->empty())
    {
        sad::Vector<sad::p2d::World::QueuedCommand>* queue = m_command_queue;
        m_command_queue = new sad::Vector<sad::p2d::World::QueuedCommand>();
        for(size_t i = 0; i < queue->size(); i++)
        {
            sad::p2d::World::QueuedCommand& cmd = (*queue)[i];
            switch(cmd.Type)
            {
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_ADD_BODY:
                {
                    addBodyNow(cmd.Body);
                    cmd.Body->delRef();
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_BODY:
                {
                    removeBodyNow(cmd.Body);
                    cmd.Body->delRef();
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_CLEAR_BODIES:
                {
                    clearBodiesNow();
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_ADD_BODY_TO_GROUP:
                {
                    addBodyToGroupNow(cmd.GroupName, cmd.Body);
                    cmd.Body->delRef();
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_BODY_FROM_GROUP:
                {
                    removeBodyFromGroupNow(cmd.GroupName, cmd.Body);
                    cmd.Body->delRef();
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_ADD_GROUP:
                {
                    addGroupNow(cmd.GroupName);
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_GROUP:
                {
                    removeGroupNow(cmd.GroupName);
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_CLEAR_GROUP:
                {
                    clearGroupNow(cmd.GroupName);
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_CLEAR_GROUPS:
                {
                    clearGroupsNow();
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_ADD_HANDLER:
                {
                    addHandlerNow(cmd.GroupName, cmd.SecondGroupName, cmd.Handler);
                    cmd.Handler->delRef();
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_HANDLER_FROM_GROUPS:
                {
                    removeHandlerFromGroupsNow(cmd.GroupName, cmd.SecondGroupName, cmd.Handler);
                    cmd.Handler->delRef();
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_REMOVE_HANDLER:
                {
                    removeHandlerNow(cmd.Handler);
                    cmd.Handler->delRef();
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_CLEAR_HANDLERS:
                {
                    clearHandlersNow();
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_CLEAR_HANDLERS_FOR_GROUPS:
                {
                    clearHandlersForGroupsNow(cmd.GroupName, cmd.SecondGroupName);
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_CLEAR:
                {
                    clearNow();
                    break;
                }
                case sad::p2d::World::QueuedCommandType::P2D_WORLD_QCT_STEP:
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


void sad::p2d::World::addBodyNow(sad::p2d::Body* b)
{
    PROFILER_EVENT;
    if (!b)
    {
        return;
    }
    addBodyToGroupNow("p2d::Body", b);
    if (b->userObject() != nullptr)
    {
        const sad::String& name = b->userObject()->metaData()->name();
        addBodyToGroupNow(name, b);
    }
}

void sad::p2d::World::removeBodyNow(sad::p2d::Body* b)
{
    PROFILER_EVENT;
    m_world_lock.lock();
    setIsLockedFlag(true);

    if (m_global_body_container.BodyToLocation.contains(b))
    {
        sad::p2d::World::BodyLocation& loc = m_global_body_container.BodyToLocation[b];
        for(size_t i = 0; i < loc.PositionInGroups.size(); i++)
        {
            m_group_container.Groups[loc.PositionInGroups[i]].Group.remove(b);
        }
        m_global_body_container.remove(b);
    }

    setIsLockedFlag(false);
    m_world_lock.unlock();
}

void sad::p2d::World::clearBodiesNow()
{
    PROFILER_EVENT;
    m_world_lock.lock();
    setIsLockedFlag(true);

    m_global_body_container.clear();
    m_group_container.clearBodies();

    setIsLockedFlag(false);
    m_world_lock.unlock();
}

void sad::p2d::World::addBodyToGroupNow(const sad::String& group_name, sad::p2d::Body* o)
{
    PROFILER_EVENT;
    if (!o)
    {
        return;
    }

    m_world_lock.lock();
    setIsLockedFlag(true);

    sad::p2d::World::BodyLocation* loc;
    if (!m_global_body_container.BodyToLocation.contains(o))
    {
        loc = &(m_global_body_container.add(o));
        o->setSamplingCount(m_detector->sampleCount());
        o->setWorld(this);
    }
    else
    {
        loc = &(m_global_body_container.BodyToLocation[o]);
    }

    m_group_container.makeGroupAndAddBody(group_name, o, *loc);

    setIsLockedFlag(false);
    m_world_lock.unlock();
}

void sad::p2d::World::removeBodyFromGroupNow(const sad::String& group_name, sad::p2d::Body* o)
{
    PROFILER_EVENT;
    m_world_lock.lock();
    setIsLockedFlag(true);

    if (m_global_body_container.BodyToLocation.contains(o))
    {
        sad::Maybe<size_t> location = m_group_container.getLocation(group_name);
        if (location.exists())
        {
            m_group_container.Groups[location.value()].Group.remove(o);
            m_global_body_container.removeFromGroup(o, location.value());            
        }
    }

    setIsLockedFlag(false);
    m_world_lock.unlock();
}


void sad::p2d::World::addGroupNow(const sad::String& group_name)
{
    PROFILER_EVENT;
    m_world_lock.lock();
    setIsLockedFlag(true);

    m_group_container.add(group_name);

    setIsLockedFlag(false);
    m_world_lock.unlock();
}

void sad::p2d::World::removeGroupNow(const sad::String& group_name)
{
    PROFILER_EVENT;
    clearGroupNow(group_name);

    m_world_lock.lock();
    setIsLockedFlag(true);

    sad::Maybe<size_t> location = m_group_container.getLocation(group_name);
    if (location.exists())
    {
        m_group_container.remove(group_name);
        m_global_handler_list.removeHandlersFor(location.value());
    }

    setIsLockedFlag(false);
    m_world_lock.unlock();
}

void sad::p2d::World::clearGroupNow(const sad::String& group_name)
{
    PROFILER_EVENT;
    m_world_lock.lock();
    setIsLockedFlag(true);
    
    sad::Maybe<size_t> location = m_group_container.getLocation(group_name);
    if (location.exists())
    {
        sad::p2d::World::Group& group = m_group_container.Groups[location.value()].Group;
        size_t size = group.Bodies.size();
        if (size)
        {
            sad::p2d::World::BodyWithActivityFlag* p = &(group.Bodies[0]);
            for (size_t i = 0; i < size; i++)
            {
                if (p->Active)
                {
                    sad::p2d::Body* body = p->Body;
                    m_global_body_container.removeFromGroup(body, location.value());
                    body->delRef();
                }
                p++;
            }
        }
        group.BodyToLocation.clear();
        group.Bodies.clear();
        group.FreePositions.clear();
    }

    setIsLockedFlag(false);
    m_world_lock.unlock();
}

void sad::p2d::World::clearGroupsNow()
{
    PROFILER_EVENT;
    m_world_lock.lock();
    setIsLockedFlag(true);

    m_group_container.clear();
    m_global_body_container.clear();
    m_global_handler_list.clear();

    setIsLockedFlag(false);
    m_world_lock.unlock();
}

void sad::p2d::World::addHandlerNow(
    const sad::String & group_name_1,
    const sad::String & group_name_2,
    sad::p2d::BasicCollisionHandler* h
)
{
    PROFILER_EVENT;
    addGroupNow(group_name_1);
    addGroupNow(group_name_2);

    m_world_lock.lock();
    setIsLockedFlag(true);

    size_t group_index_1 = m_group_container.getLocation(group_name_1).value();
    size_t group_index_2 = m_group_container.getLocation(group_name_2).value();

    m_global_handler_list.add(group_index_1, group_index_2, h);

    setIsLockedFlag(false);
    m_world_lock.unlock();
}

void sad::p2d::World::removeHandlerFromGroupsNow(
    const sad::String & first_group_name,
    const sad::String & second_group_name,
    sad::p2d::BasicCollisionHandler* h
)
{
    PROFILER_EVENT;
    m_world_lock.lock();
    setIsLockedFlag(true);

    sad::Maybe<size_t> group_index_1 = m_group_container.getLocation(first_group_name);
    sad::Maybe<size_t> group_index_2 = m_group_container.getLocation(second_group_name);

    if (group_index_1.exists() && group_index_2.exists())
    {
        m_global_handler_list.remove(group_index_1.value(), group_index_2.value(), h);
    }

    setIsLockedFlag(false);
    m_world_lock.unlock();
}

void sad::p2d::World::removeHandlerNow(sad::p2d::BasicCollisionHandler* h)
{
    PROFILER_EVENT;
    m_world_lock.lock();
    setIsLockedFlag(true);

    m_global_handler_list.remove(h);

    setIsLockedFlag(false);
    m_world_lock.unlock();
}

void sad::p2d::World::clearHandlersNow()
{
    PROFILER_EVENT;
    m_world_lock.lock();
    setIsLockedFlag(true);

    m_global_handler_list.clear();

    setIsLockedFlag(false);
    m_world_lock.unlock();
}

void sad::p2d::World::clearHandlersForGroupsNow(const sad::String& first_group, const sad::String& second_group)
{
    PROFILER_EVENT;
    m_world_lock.lock();
    setIsLockedFlag(true);

    sad::Maybe<size_t> group_index_1 = m_group_container.getLocation(first_group);
    sad::Maybe<size_t> group_index_2 = m_group_container.getLocation(second_group);

    if (group_index_1.exists() && group_index_2.exists())
    {
        m_global_handler_list.clearForGroups(group_index_1.value(), group_index_2.value());
    }


    setIsLockedFlag(false);
    m_world_lock.unlock();
}

void sad::p2d::World::clearNow()
{
    PROFILER_EVENT;
    // Does all - clears both groups and all other stuff
    clearGroupsNow();
}

void sad::p2d::World::stepNow(double time)
{
    PROFILER_EVENT;
    performQueuedCommands();

    m_world_lock.lock();
    setIsLockedFlag(true);

    m_time_step = time;
    m_global_body_container.buildBodyCaches(time);
    {
        sad::p2d::World::EventsWithCallbacks events_with_callbacks;
        findEvents(events_with_callbacks);
        std::sort(events_with_callbacks.begin(), events_with_callbacks.end());
        sad::invoke_functors(events_with_callbacks);
    }

    m_global_body_container.stepPositionsAndVelocities(time);
    m_global_body_container.stepDiscreteChangingValues(time);

    setIsLockedFlag(false);
    m_world_lock.unlock();

    performQueuedCommands();
}


void sad::p2d::World::findEvents(sad::p2d::World::EventsWithCallbacks& ewc)
{
    PROFILER_EVENT;
    for (size_t i = 0; i < m_global_handler_list.List.size(); i++)
    {
        sad::p2d::World::HandlerList& lst = m_global_handler_list.List[i];
        if (lst.List)
        {
            findEvent(ewc, lst);
        }
    }
}
void sad::p2d::World::findEvent(sad::p2d::World::EventsWithCallbacks& ewc, sad::p2d::World::HandlerList& lst)
{
    PROFILER_EVENT;
    double step = this->timeStep();
    size_t group_index_1 = lst.TypeIndex1;
    size_t group_index_2 = lst.TypeIndex2;
    sad::Vector<sad::p2d::BasicCollisionHandler*>* callbacks = lst.List;

    if (m_group_container.Groups[group_index_1].Active
       && m_group_container.Groups[group_index_2].Active)
    {
        sad::p2d::World::Group& group1 = m_group_container.Groups[group_index_1].Group;
        sad::p2d::World::Group& group2 = m_group_container.Groups[group_index_2].Group;

        sad::Vector<sad::p2d::World::BodyWithActivityFlag>& bodies1 = group1.Bodies;
        sad::Vector<sad::p2d::World::BodyWithActivityFlag>& bodies2 = group2.Bodies;

        bool not_same_group = group_index_1 != group_index_2;
        for (size_t i = 0; i < bodies1.count(); i++)
        {
            size_t jmin = i + 1;
            if (not_same_group)
            {
                jmin = 0;
            }
            for (size_t j = jmin; j < bodies2.count(); j++)
            {
                sad::p2d::Body* b1 = bodies1[i].Body;
                sad::p2d::Body* b2 = bodies2[j].Body;

                bool b1active = bodies1[i].Active;
                bool b2active = bodies2[j].Active;
                if (b1active && b2active && (b1 != b2))
                {
                    if (!(b1->isGhost()) && !(b2->isGhost()))
                    {
                        b1->TimeStep = step;
                        b2->TimeStep = step;
                        sad::Maybe<double> time = m_detector->collides(b1, b2, m_time_step);
                        if (time.exists())
                        {
                            sad::p2d::BasicCollisionEvent ev(b1, b2, time.value());
                            ewc << sad::p2d::World::EventWithCallback(ev, callbacks);
                        }
                    }
                }
            }
        }
    }
}
