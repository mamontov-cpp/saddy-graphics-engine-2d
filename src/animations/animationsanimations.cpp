#include "animations/animationsanimations.h"

// ========================= PUBLIC METHODS =========================

sad::animations::Animations::Animations() : m_lock_changes(false)
{
    
}

sad::animations::Animations::~Animations()
{
    this->clearNow();
}

void sad::animations::Animations::add(sad::animations::Process * o)
{
    if (!o)
    {
        return;
    }
    o->addRef();
    performOrQueue([this, o]() -> void {
        this->addNow(o); o->delRef();
    });
}

void sad::animations::Animations::remove(sad::animations::Process *o)
{
    if (!o)
    {
        return;
    }
    o->addRef();
    performOrQueue([this, o]() -> void {
        this->removeNow(o); o->delRef();
    });
}

void sad::animations::Animations::removeByName(const sad::String& name)
{
    performOrQueue([this, name]() -> void {
        std::function<bool(sad::animations::Process*)> matcher = [name](sad::animations::Process* o) {
            return o->objectName() == name;
        };
        int pos = this->findFirst(matcher);
        while (pos >= 0)
        {
            removeAtNow(pos);
            pos = this->findFirst(matcher);
        }
    });
}

void sad::animations::Animations::removeByNameAndType(const sad::String& type, const sad::String& name)
{
    performOrQueue([this, type, name]() -> void {
        std::function<bool(sad::animations::Process*)> matcher = [type, name](sad::animations::Process* o) {
            if (o->isInstanceOf(type))
            { 
                return o->objectName() == name;
            }
            return false;
        };
        int pos = this->findFirst(matcher);
        while (pos >= 0)
        {
            removeAtNow(pos);
            pos = this->findFirst(matcher);
        }
    });
}

void sad::animations::Animations::removeByMajorId(unsigned long long major_id)
{
    performOrQueue([this, major_id]() -> void {
        std::function<bool(sad::animations::Process*)> matcher = [major_id](sad::animations::Process* o) {
            return o->MajorId == major_id;
        };
        int pos = this->findFirst(matcher);
        while (pos >= 0)
        {
            removeAtNow(pos);
            pos = this->findFirst(matcher);
        }
    });
}

void sad::animations::Animations::clear()
{
    performOrQueue([this]() -> void {
        this->clearNow();
    });
}

void sad::animations::Animations::insertAt(unsigned int position, sad::animations::Process* o)
{
    o->addRef();
    performOrQueue([this, o, position]() -> void {
        this->insertAtNow(position, o); o->delRef();
    });
}

void sad::animations::Animations::removeAt(unsigned int position)
{
    performOrQueue([this, position]() -> void {
        this->removeAtNow(position);
    });
}

unsigned int sad::animations::Animations::count() const
{
    return m_list.size();
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::list() const
{
    sad::animations::Animations* me = const_cast<sad::animations::Animations*>(this);
    me->m_lock.lock();
    sad::Vector<sad::animations::Process*> result = m_list;
    me->m_lock.unlock();
    return result;
}

int sad::animations::Animations::find(sad::animations::Process* o)
{
    m_lock.lock();
    sad::Vector<sad::animations::Process*>::iterator it = std::find(m_list.begin(), m_list.end(), o);
    int result = -1;
    if (it != m_list.end())
    {
        result = static_cast<int>(it - m_list.begin());
    }
    m_lock.unlock();
    return result;
}

void sad::animations::Animations::insertBefore(sad::animations::Process* before, sad::animations::Process* o)
{
    o->addRef();
    before->addRef();
    performOrQueue([this, before, o]() -> void {
        this->insertBeforeNow(before, o); o->delRef(); before->delRef();
    });
}

void sad::animations::Animations::insertBefore(const sad::String& name, sad::animations::Process* o)
{
    o->addRef();
    performOrQueue([this, name, o]() -> void {
        this->insertBeforeNow(name, o); o->delRef();
    });
}

void sad::animations::Animations::insertBefore(const sad::String& type, const sad::String& name, sad::animations::Process* o)
{
    o->addRef();
    performOrQueue([this, name, type, o]() -> void {
        this->insertBeforeNow(type, name, o); o->delRef();
    });
}

void sad::animations::Animations::insertBefore(unsigned long long major_id, sad::animations::Process* o)
{
    o->addRef();
    performOrQueue([this, major_id, o]() -> void {
        this->insertBeforeNow(major_id, o); o->delRef();
    });
}

void  sad::animations::Animations::insertBeforeInstanceWithObject(sad::db::Object* before, sad::animations::Process* o)
{
    std::function<bool(sad::db::Object*)> object_matcher = [before](sad::db::Object* object) -> bool {
        return object == before;
    };
    std::function<bool(sad::animations::Process*)> matcher = [object_matcher](sad::animations::Process* m) -> bool  {
        return m->isRelatedToMatchedObject(object_matcher);
    };
    o->addRef();
    before->addRef();
    performOrQueue([this, before, matcher, o]() -> void {
        this->insertBeforeFirstMatched(matcher, o); 
        o->delRef(); 
        before->delRef();
    });
}


void  sad::animations::Animations::insertBeforeInstanceWithObject(const sad::String& name, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [name](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedObject([name](sad::db::Object* obj) -> bool {   
            if (obj) 
                return obj->objectName() == name; 
            return false; 
        });
    };
    o->addRef();
    performOrQueue([this, matcher, o]() -> void {
        this->insertBeforeFirstMatched(matcher, o);
        o->delRef();
    });
}

void sad::animations::Animations::insertBeforeInstanceWithObject(const sad::String& type, const sad::String& name, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [name, type](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedObject([name, type](sad::db::Object* obj) -> bool {
            if (obj) 
            { 
                if (obj->isInstanceOf(type))
                { 
                    return obj->objectName() == name;
                }
            }
            return false;
        });
    };
    o->addRef();
    performOrQueue([this, matcher, o]() -> void {
        this->insertBeforeFirstMatched(matcher, o);
        o->delRef();
    });
}

void sad::animations::Animations::insertBeforeInstanceWithObject(unsigned long long major_id, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [major_id](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedObject([major_id](sad::db::Object* obj) -> bool {
            if (obj)
                return obj->MajorId == major_id;
            return false;
        });
    };
    o->addRef();
    performOrQueue([this, matcher, o]() -> void {
        this->insertBeforeFirstMatched(matcher, o);
        o->delRef();
    });
}

void sad::animations::Animations::insertBeforeInstanceWithAnimation(sad::animations::Animation* before, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [before](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedAnimation([before](sad::animations::Animation* obj) -> bool {
            return obj == before;
        });
    };
    o->addRef();
    before->addRef();
    performOrQueue([this, before, matcher, o]() -> void {
        this->insertBeforeFirstMatched(matcher, o);
        o->delRef();
        before->delRef();
    });
}

void sad::animations::Animations::insertBeforeInstanceWithAnimation(const sad::String& name, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [name](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedAnimation([name](sad::animations::Animation* obj) -> bool {
            if (obj)
            {
                return obj->objectName() == name;
            }
            return false;
        });
    };
    o->addRef();
    performOrQueue([this, matcher, o]() -> void {
        this->insertBeforeFirstMatched(matcher, o);
        o->delRef();
    });
}

void sad::animations::Animations::insertBeforeInstanceWithAnimation(const sad::String& type, const sad::String& name, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [name, type](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedAnimation([name, type](sad::animations::Animation* obj) -> bool {
            if (obj)
            {
                if (obj->isInstanceOf(type)) 
                {
                    return obj->objectName() == name;
                }
            }
            return false;
        });
    };
    o->addRef();
    performOrQueue([this, matcher, o]() -> void {
        this->insertBeforeFirstMatched(matcher, o);
        o->delRef();
    });
}

void sad::animations::Animations::insertBeforeInstanceWithAnimation(unsigned long long major_id, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [major_id](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedAnimation([major_id](sad::animations::Animation* obj) -> bool {
            if (obj)
            {
                return obj->MajorId == major_id;
            }
            return false;
        });
    };
    o->addRef();
    performOrQueue([this, matcher, o]() -> void {
        this->insertBeforeFirstMatched(matcher, o);
        o->delRef();
    });
}



void sad::animations::Animations::insertAfter(sad::animations::Process* after, sad::animations::Process* o)
{
    o->addRef();
    after->addRef();
    performOrQueue([this, after, o]() -> void {
        this->insertAfterNow(after, o); o->delRef(); after->delRef();
    });
}

void sad::animations::Animations::insertAfter(const sad::String& name, sad::animations::Process* o)
{
    o->addRef();
    performOrQueue([this, name, o]() -> void {
        this->insertAfterNow(name, o); o->delRef();
    });
}

void sad::animations::Animations::insertAfter(const sad::String& type, const sad::String& name, sad::animations::Process* o)
{
    o->addRef();
    performOrQueue([this, name, type, o]() -> void {
        this->insertAfterNow(type, name, o); o->delRef();
    });
}

void sad::animations::Animations::insertAfter(unsigned long long major_id, sad::animations::Process* o)
{
    o->addRef();
    performOrQueue([this, major_id, o]() -> void {
        this->insertAfterNow(major_id, o); o->delRef();
    });
}

void  sad::animations::Animations::insertAfterInstanceWithObject(sad::db::Object* after, sad::animations::Process* o)
{
    std::function<bool(sad::db::Object*)> object_matcher = [after](sad::db::Object* object) -> bool {
        return object == after;
    };
    std::function<bool(sad::animations::Process*)> matcher = [object_matcher](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedObject(object_matcher);
    };
    o->addRef();
    after->addRef();
    performOrQueue([this, after, matcher, o]() -> void {
        this->insertAfterLastMatched(matcher, o);
        o->delRef();
        after->delRef();
    });
}


void  sad::animations::Animations::insertAfterInstanceWithObject(const sad::String& name, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [name](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedObject([name](sad::db::Object* obj) -> bool {
            if (obj)
                return obj->objectName() == name;
            return false;
        });
    };
    o->addRef();
    performOrQueue([this, matcher, o]() -> void {
        this->insertAfterLastMatched(matcher, o);
        o->delRef();
    });
}

void sad::animations::Animations::insertAfterInstanceWithObject(const sad::String& type, const sad::String& name, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [name, type](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedObject([name, type](sad::db::Object* obj) -> bool {
            if (obj)
            {
                if (obj->isInstanceOf(type))
                {
                    return obj->objectName() == name;
                }
            }
            return false;
        });
    };
    o->addRef();
    performOrQueue([this, matcher, o]() -> void {
        this->insertAfterLastMatched(matcher, o);
        o->delRef();
    });
}

void sad::animations::Animations::insertAfterInstanceWithObject(unsigned long long major_id, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [major_id](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedObject([major_id](sad::db::Object* obj) -> bool {
            if (obj)
                return obj->MajorId == major_id;
            return false;
        });
    };
    o->addRef();
    performOrQueue([this, matcher, o]() -> void {
        this->insertAfterLastMatched(matcher, o);
        o->delRef();
    });
}

void sad::animations::Animations::insertAfterInstanceWithAnimation(sad::animations::Animation* after, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [after](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedAnimation([after](sad::animations::Animation* obj) -> bool {
            return obj == after;
        });
    };
    o->addRef();
    after->addRef();
    performOrQueue([this, after, matcher, o]() -> void {
        this->insertAfterLastMatched(matcher, o);
        o->delRef();
        after->delRef();
    });
}

void sad::animations::Animations::insertAfterInstanceWithAnimation(const sad::String& name, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [name](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedAnimation([name](sad::animations::Animation* obj) -> bool {
            if (obj)
            {
                return obj->objectName() == name;
            }
            return false;
        });
    };
    o->addRef();
    performOrQueue([this, matcher, o]() -> void {
        this->insertAfterLastMatched(matcher, o);
        o->delRef();
    });
}

void sad::animations::Animations::insertAfterInstanceWithAnimation(const sad::String& type, const sad::String& name, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [name, type](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedAnimation([name, type](sad::animations::Animation* obj) -> bool {
            if (obj)
            {
                if (obj->isInstanceOf(type))
                {
                    return obj->objectName() == name;
                }
            }
            return false;
        });
    };
    o->addRef();
    performOrQueue([this, matcher, o]() -> void {
        this->insertAfterLastMatched(matcher, o);
        o->delRef();
    });
}

void sad::animations::Animations::insertAfterInstanceWithAnimation(unsigned long long major_id, sad::animations::Process* o)
{
    std::function<bool(sad::animations::Process*)> matcher = [major_id](sad::animations::Process* m) -> bool {
        return m->isRelatedToMatchedAnimation([major_id](sad::animations::Animation* obj) -> bool {
            if (obj)
            {
                return obj->MajorId == major_id;
            }
            return false;
        });
    };
    o->addRef();
    performOrQueue([this, matcher, o]() -> void {
        this->insertAfterLastMatched(matcher, o);
        o->delRef();
    });
}

void sad::animations::Animations::swap(sad::animations::Process* first, sad::animations::Process* second)
{
    if (!first || !second)
    {
        return;
    }
    std::function<void()> command = [this, first, second]()  {
        sad::Vector<sad::animations::Process*>::iterator first_iterator = std::find(this->m_list.begin(), this->m_list.end(), first);
        sad::Vector<sad::animations::Process*>::iterator second_iterator = std::find(this->m_list.begin(), this->m_list.end(), second);
        if ((first_iterator != this->m_list.end()) && (second_iterator != this->m_list.end()))
        {
            size_t pos1 = first_iterator - this->m_list.begin();
            size_t pos2 = second_iterator - this->m_list.begin();
            this->m_list[pos1] = second;
            this->m_list[pos2] = first;
        }

        first->delRef();
        second->delRef();
    };

    first->addRef();
    second->addRef();
    performOrQueue(command);
}

void sad::animations::Animations::swap(const sad::String& name1, const sad::String name2)
{
    std::function<std::function<bool(sad::animations::Process*)>(sad::String)> make_matcher = [](sad::String name) {
        return [name](sad::animations::Process* p) -> bool {
            return p->objectName() == name;
        };
    };

    std::function<void()> command = [this, make_matcher, name1, name2]() {
        int pos1 = this->findFirst(make_matcher(name1));
        int pos2 = this->findLast(make_matcher(name2));
        if ((pos1 >= 0) && (pos2 >= 0))
        {
            sad::animations::Process* first = this->m_list[pos1];
            sad::animations::Process* second = this->m_list[pos2];

            this->m_list[pos1] = second;
            this->m_list[pos2] = first;
        }
    };

    performOrQueue(command);
}

void sad::animations::Animations::swap(const sad::String& type1, const sad::String& name1, const sad::String& type2, const sad::String name2)
{
    std::function<std::function<bool(sad::animations::Process*)>(sad::String, sad::String)> make_matcher = [](sad::String type, sad::String name) {
        return [type, name](sad::animations::Process* p) -> bool {
            if (p->isInstanceOf(type)) {
                return p->objectName() == name;
            }
            return false;
        };
    };

    std::function<void()> command = [this, make_matcher, type1, type2, name1, name2]() {
        int pos1 = this->findFirst(make_matcher(type1, name1));
        int pos2 = this->findLast(make_matcher(type2, name2));
        if ((pos1 >= 0) && (pos2 >= 0))
        {
            sad::animations::Process* first = this->m_list[pos1];
            sad::animations::Process* second = this->m_list[pos2];

            this->m_list[pos1] = second;
            this->m_list[pos2] = first;
        }
    };

    performOrQueue(command);
}

void sad::animations::Animations::swap(unsigned long long major_id1, unsigned long long major_id2)
{
    std::function<std::function<bool(sad::animations::Process*)>(unsigned long long)> make_matcher = [](unsigned long long  major_id) {
        return [major_id](sad::animations::Process* p) -> bool {
            return p->MajorId == major_id;
        };
    };

    std::function<void()> command = [this, make_matcher, major_id1, major_id2]() {
        int pos1 = this->findFirst(make_matcher(major_id1));
        int pos2 = this->findLast(make_matcher(major_id2));
        if ((pos1 >= 0) && (pos2 >= 0))
        {
            sad::animations::Process* first = this->m_list[pos1];
            sad::animations::Process* second = this->m_list[pos2];

            this->m_list[pos1] = second;
            this->m_list[pos2] = first;
        }
    };

    performOrQueue(command);
}

sad::animations::SavedObjectStateCache& sad::animations::Animations::cache()
{
    return m_cache;
}

sad::Vector<sad::animations::Process*>  sad::animations::Animations::queryProcessesByName(const sad::String& name)
{
    return this->queryProcesses([name](sad::animations::Process* o) -> bool {
        return o->objectName() == name;
    });
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesByTypeAndName(const sad::String& type, const sad::String& name)
{
    return this->queryProcesses([type, name](sad::animations::Process* o) -> bool {
        if (o->isInstanceOf(type))
        { 
            return o->objectName() == name;
        }
        return false;
    });
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesByMajorId(unsigned long long major_id)
{
    return this->queryProcesses([major_id](sad::animations::Process* o) -> bool {
        return o->MajorId == major_id;
    });
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesRelatedToObject(sad::db::Object* o)
{
    return this->queryProcessesRelatedToMatchedObject([o](sad::db::Object* object) -> bool {
        return object == o;
    });
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesRelatedToObjectByName(const sad::String& name)
{
    return this->queryProcessesRelatedToMatchedObject([name](sad::db::Object* o) -> bool {
        if (o)
        {
            return o->objectName() == name;
        }
        return false;
    });
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesRelatedToObjectByTypeAndName(const sad::String& type, const sad::String& name)
{
    return this->queryProcessesRelatedToMatchedObject([type, name](sad::db::Object* o) -> bool {
        if (o)
        {
            if (o->isInstanceOf(type))
            { 
                return o->objectName() == name;
            }
        }
        return false;
    });
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesRelatedToObjectByMajorId(unsigned long long major_id)
{
    return this->queryProcessesRelatedToMatchedObject([major_id](sad::db::Object* o) -> bool {
        if (o)
        {
            return o->MajorId == major_id;
        }
        return false;
    });
}


sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesRelatedToAnimation(sad::animations::Animation* o)
{
    return this->queryProcessesRelatedToMatchedAnimation([o](sad::animations::Animation* animation) -> bool {
        return animation == o;
    });
}


sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesRelatedToAnimationByName(const sad::String& name)
{
    return this->queryProcessesRelatedToMatchedAnimation([name](sad::animations::Animation* o) -> bool {
        if (o)
        {
            return o->objectName() == name;
        }
        return false;
    });
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesRelatedToAnimationByTypeAndName(const sad::String& type, const sad::String& name)
{
    return this->queryProcessesRelatedToMatchedAnimation([type, name](sad::animations::Animation* o) -> bool {
        if (o)
        {
            if (o->isInstanceOf(type))
            {
                return o->objectName() == name;
            }
        }
        return false;
    });
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesRelatedToAnimationByMajorId(unsigned long long major_id)
{
    return this->queryProcessesRelatedToMatchedAnimation([major_id](sad::animations::Animation* o) -> bool {
        if (o)
        {
            return o->MajorId == major_id;
        }
        return false;
    });
}

void sad::animations::Animations::stopProcess(sad::animations::Process* o)
{
    if (!o)
    {
        return;
    }
    o->addRef();
    performOrQueue([this, o] {
        std::function<bool(sad::animations::Process*)> matcher = [o](sad::animations::Process* obj) {
            return obj == o;
        };
        sad::Vector<sad::animations::Process*> list = this->list();
        for (size_t i = 0; i < list.size(); i++)
        {
            list[i]->stopInstancesRelatedToMatchedProcess(matcher, this);
        }
        o->delRef();
    });
}

void sad::animations::Animations::stopProcessByName(const sad::String& name)
{
    performOrQueue([this, name] {
        std::function<bool(sad::animations::Process*)> matcher = [name](sad::animations::Process* obj) {
            return obj->objectName() == name;
        };
        sad::Vector<sad::animations::Process*> list = this->list();
        for (size_t i = 0; i < list.size(); i++)
        {
            list[i]->stopInstancesRelatedToMatchedProcess(matcher, this);
        }
    });
}

void sad::animations::Animations::stopProcessByTypeAndName(const sad::String& type, const sad::String& name)
{
    performOrQueue([this, type, name] {
        std::function<bool(sad::animations::Process*)> matcher = [type, name](sad::animations::Process* obj) {
            if (obj->isInstanceOf(type)) 
            {
                return obj->objectName() == name;
            }
            return false;
        };
        sad::Vector<sad::animations::Process*> list = this->list();
        for (size_t i = 0; i < list.size(); i++)
        {
            list[i]->stopInstancesRelatedToMatchedProcess(matcher, this);
        }
    });
}

void sad::animations::Animations::stopProcessByMajorId(unsigned long long major_id)
{
    performOrQueue([this, major_id] {
        std::function<bool(sad::animations::Process*)> matcher = [major_id](sad::animations::Process* obj) {
            return obj->MajorId == major_id;
        };
        sad::Vector<sad::animations::Process*> list = this->list();
        for (size_t i = 0; i < list.size(); i++)
        {
            list[i]->stopInstancesRelatedToMatchedProcess(matcher, this);
        }
    });
}

void sad::animations::Animations::stopProcessesRelatedToObject(sad::db::Object* o)
{
    if (!o)
    {
        return;
    }
    o->addRef();
    performOrQueue([this, o] {
        std::function<bool(sad::db::Object*)> matcher = [o](sad::db::Object* obj) {
            if (obj)
            {
                return obj == o;
            }
            return false;
        };
        sad::Vector<sad::animations::Process*> list = this->list();
        for (size_t i = 0; i < list.size(); i++)
        {
            list[i]->stopInstancesRelatedToMatchedObject(matcher, this);
        }
        o->delRef();
    });
}

void sad::animations::Animations::stopProcessesRelatedToObjectWithName(const sad::String& name)
{
    performOrQueue([this, name] {
        std::function<bool(sad::db::Object*)> matcher = [name](sad::db::Object* o) {
            if (o)
            {
                return o->objectName() == name;
            }
            return false;
        };
        sad::Vector<sad::animations::Process*> list = this->list();
        for (size_t i = 0; i < list.size(); i++)
        {
            list[i]->stopInstancesRelatedToMatchedObject(matcher, this);
        }
    });
}

void sad::animations::Animations::stopProcessesRelatedToObjectWithTypeAndName(const sad::String& type, const sad::String& name)
{
    performOrQueue([this, type, name] {
        std::function<bool(sad::db::Object*)> matcher = [type, name](sad::db::Object* o) {
            if (o)
            {
                if (o->isInstanceOf(type))
                { 
                    return o->objectName() == name;
                }
            }
            return false;
        };
        sad::Vector<sad::animations::Process*> list = this->list();
        for (size_t i = 0; i < list.size(); i++)
        {
            list[i]->stopInstancesRelatedToMatchedObject(matcher, this);
        }
    });
}

void sad::animations::Animations::stopProcessesRelatedToObjectWithMajorId(unsigned long long major_id)
{
    performOrQueue([this, major_id] {
        std::function<bool(sad::db::Object*)> matcher = [major_id](sad::db::Object* o) {
            if (o)
            {
                return o->MajorId == major_id;
            }
            return false;
        };
        sad::Vector<sad::animations::Process*> list = this->list();
        for (size_t i = 0; i < list.size(); i++)
        {
            list[i]->stopInstancesRelatedToMatchedObject(matcher, this);
        }
    });
}

void sad::animations::Animations::stopProcessesRelatedToAnimation(sad::animations::Animation* o)
{
    if (!o)
    {
        return;
    }
    o->addRef();
    performOrQueue([this, o] {
        std::function<bool(sad::animations::Animation*)> f = [o](sad::animations::Animation* me) {
            return me == o;
        };
        sad::Vector<sad::animations::Process*> list = this->list();
        for (size_t i = 0; i < list.size(); i++)
        {
            list[i]->stopInstancesRelatedToMatchedAnimation(f, this);
        }
        o->delRef();
    });
}


void sad::animations::Animations::stopProcessesRelatedToAnimationWithName(const sad::String& name)
{
    performOrQueue([this, name] {
        std::function<bool(sad::animations::Animation*)> matcher = [name](sad::animations::Animation* o) {
            if (o)
            {
                return o->objectName() == name;
            }
            return false;
        };
        sad::Vector<sad::animations::Process*> list = this->list();
        for (size_t i = 0; i < list.size(); i++)
        {
            list[i]->stopInstancesRelatedToMatchedAnimation(matcher, this);
        }
    });
}

void sad::animations::Animations::stopProcessesRelatedToAnimationWithTypeAndName(const sad::String& type, const sad::String& name)
{
    performOrQueue([this, type, name] {
        std::function<bool(sad::animations::Animation*)> matcher = [type, name](sad::animations::Animation* o) {
            if (o)
            {
                if (o->isInstanceOf(type))
                {
                    return o->objectName() == name;
                }
            }
            return false;
        };
        sad::Vector<sad::animations::Process*> list = this->list();
        for (size_t i = 0; i < list.size(); i++)
        {
            list[i]->stopInstancesRelatedToMatchedAnimation(matcher, this);
        }
    });
}

void sad::animations::Animations::stopProcessesRelatedToAnimationWithMajorId(unsigned long long major_id)
{
    performOrQueue([this, major_id] {
        std::function<bool(sad::animations::Animation*)> matcher = [major_id](sad::animations::Animation* o) {
            if (o)
            {
                return o->MajorId == major_id;
            }
            return false;
        };
        sad::Vector<sad::animations::Process*> list = this->list();
        for (size_t i = 0; i < list.size(); i++)
        {
            list[i]->stopInstancesRelatedToMatchedAnimation(matcher, this);
        }
    });
}

size_t sad::animations::Animations::countProcesses(sad::animations::Process* o)
{
    return queryProcesses([o](sad::animations::Process* p) ->bool {
        return p == o;
    }).size();
}

size_t sad::animations::Animations::countProcessesByName(const sad::String& name)
{
    return queryProcessesByName(name).size();
}

size_t sad::animations::Animations::countProcessesByTypeAndName(const sad::String& type, const sad::String& name)
{
    return queryProcessesByTypeAndName(type, name).size();
}

size_t sad::animations::Animations::countProcessesByMajorId(unsigned long long major_id)
{
    return queryProcessesByMajorId(major_id).size();
}

size_t sad::animations::Animations::countProcessesRelatedToObject(sad::db::Object* o)
{
    return queryProcessesRelatedToObject(o).size();
}

size_t sad::animations::Animations::countProcessesRelatedToObjectByName(const sad::String& name)
{
    return queryProcessesRelatedToObjectByName(name).size();
}

size_t sad::animations::Animations::countProcessesRelatedToObjectByTypeAndName(const sad::String& type, const sad::String& name)
{
    return queryProcessesRelatedToObjectByTypeAndName(type, name).size();
}

size_t sad::animations::Animations::countProcessesRelatedToObjectByMajorId(unsigned long long major_id)
{
    return queryProcessesRelatedToObjectByMajorId(major_id).size();
}


size_t sad::animations::Animations::countProcessesRelatedToAnimation(sad::animations::Animation* o)
{
    return queryProcessesRelatedToAnimation(o).size();
}

size_t sad::animations::Animations::countProcessesRelatedToAnimationByName(const sad::String& name)
{
    return queryProcessesRelatedToAnimationByName(name).size();
}

size_t sad::animations::Animations::countProcessesRelatedToAnimationByTypeAndName(const sad::String& type, const sad::String& name)
{
    return queryProcessesRelatedToAnimationByTypeAndName(type, name).size();
}

size_t sad::animations::Animations::countProcessesRelatedToAnimationByMajorId(unsigned long long major_id)
{
    return queryProcessesRelatedToAnimationByMajorId(major_id).size();
}


bool sad::animations::Animations::hasProcesses(sad::animations::Process* o)
{
    return countProcesses(o) != 0;
}

bool sad::animations::Animations::hasProcessesByName(const sad::String& name)
{
    return queryProcessesByName(name).size() != 0;
}

bool sad::animations::Animations::hasProcessesByTypeAndName(const sad::String& type, const sad::String& name)
{
    return queryProcessesByTypeAndName(type, name).size() != 0;
}

bool sad::animations::Animations::hasProcessesByMajorId(unsigned long long major_id)
{
    return queryProcessesByMajorId(major_id).size() != 0;
}

bool sad::animations::Animations::hasProcessesRelatedToObject(sad::db::Object* o)
{
    return queryProcessesRelatedToObject(o).size() != 0;
}

bool sad::animations::Animations::hasProcessesRelatedToObjectByName(const sad::String& name)
{
    return queryProcessesRelatedToObjectByName(name).size() != 0;
}

bool sad::animations::Animations::hasProcessesRelatedToObjectByTypeAndName(const sad::String& type, const sad::String& name)
{
    return queryProcessesRelatedToObjectByTypeAndName(type, name).size() != 0;
}

bool sad::animations::Animations::hasProcessesRelatedToObjectByMajorId(unsigned long long major_id)
{
    return queryProcessesRelatedToObjectByMajorId(major_id).size() != 0;
}


bool sad::animations::Animations::hasProcessesRelatedToAnimation(sad::animations::Animation* o)
{
    return queryProcessesRelatedToAnimation(o).size() != 0;
}

bool sad::animations::Animations::hasProcessesRelatedToAnimationByName(const sad::String& name)
{
    return queryProcessesRelatedToAnimationByName(name).size() != 0;
}

bool sad::animations::Animations::hasProcessesRelatedToAnimationByTypeAndName(const sad::String& type, const sad::String& name)
{
    return queryProcessesRelatedToAnimationByTypeAndName(type, name).size() != 0;
}

bool sad::animations::Animations::hasProcessesRelatedToAnimationByMajorId(unsigned long long major_id)
{
    return queryProcessesRelatedToAnimationByMajorId(major_id).size() != 0;
}

void sad::animations::Animations::notifyProcessRemoval(sad::animations::Process* process)
{
    this->removeNow(process);
}

// ========================= PROTECTED METHODS =========================

sad::animations::Animations::Animations(const sad::animations::Animations& o) : m_lock_changes(false)
{
    throw std::logic_error("sad::animations::Animations cannot be copied");
}

sad::animations::Animations& sad::animations::Animations::operator=(const sad::animations::Animations& o)
{
    throw std::logic_error("sad::animations::Animations cannot be copied");
    // ReSharper disable once CppUnreachableCode
    return *this;
}

void sad::animations::Animations::_process()
{
    performQueuedActions();
    lockChanges();
    m_lock.lock();
    for(size_t i = 0; i < m_list.size(); i++)
    {
        sad::animations::Process * p = m_list[i];
        p->process(this);
        if (p->finished())
        {
            p->removedFromPipeline();
            p->delRef();
            m_list.removeAt(i);
            --i;
        }
    }
    m_lock.unlock();
    unlockChanges();
    performQueuedActions();
}

void sad::animations::Animations::addNow(sad::animations::Process* o)
{
    m_lock.lock();
    // If process is already in animation, don't do stuff
    const sad::Vector<sad::animations::Process*>::iterator it = std::find(
        m_list.begin(),
        m_list.end(),
        o
    );
    if (it == m_list.end())
    {
        if (o)
        {
            o->addRef();
            o->addedToPipeline();
            m_list.push_back(o);
        }
    }
    m_lock.unlock();
}

void sad::animations::Animations::removeNow(sad::animations::Process* o)
{
    m_lock.lock();
    if (o)
    {
        const sad::Vector<sad::animations::Process*>::iterator it = std::find(
            m_list.begin(),
            m_list.end(),
            o
        );
        if (it != m_list.end())
        {
            o->cancel(this);
            o->removedFromPipeline();
            o->delRef();
            m_list.erase(it);
        }
    }
    m_lock.unlock();
}

void sad::animations::Animations::clearNow()
{
    m_lock.lock();
    for(size_t i = 0; i < m_list.size(); i++)
    {
        m_list[i]->cancel(this);
        m_list[i]->removedFromPipeline();
        m_list[i]->delRef();
    }
    m_list.clear();
    m_lock.unlock();
}

void sad::animations::Animations::insertAtNow(unsigned int position, sad::animations::Process* o)
{
    m_lock.lock();
    // If process is already in animation, don't do stuff
    const sad::Vector<sad::animations::Process*>::iterator it = std::find(
        m_list.begin(),
        m_list.end(),
        o
    );
    if (it == m_list.end())
    {
        if (o)
        {
            o->addRef();
            o->addedToPipeline();
            m_list.insert(o, position);
        }
    }
    m_lock.unlock();
}

void sad::animations::Animations::removeAtNow(unsigned int position)
{
    m_lock.lock();
    if (position < m_list.size())
    { 
        sad::animations::Process* o = m_list[position];
        o->cancel(this);
        o->removedFromPipeline();
        o->delRef();
        m_list.removeAt(position);
    }
    m_lock.unlock();
}

int sad::animations::Animations::findFirst(const std::function<bool(sad::animations::Process*)>& f)
{
    m_lock.lock();
    int pos = -1;
    for (size_t i = 0; (i < m_list.size()) && (pos == -1); i++)
    {
        if (f(m_list[i]))
        {
            pos = static_cast<int>(i);
        }
    }
    m_lock.unlock();
    return pos;
}

int sad::animations::Animations::findLast(const std::function<bool(sad::animations::Process*)>& f)
{
    m_lock.lock();
    int pos = -1;
    for (size_t i = 0; (i < m_list.size()); i++)
    {
        if (f(m_list[i]))
        {
            pos = static_cast<int>(i);
        }
    }
    m_lock.unlock();
    return pos;
}

void sad::animations::Animations::insertBeforeFirstMatched(const std::function<bool(sad::animations::Process*)>& f, sad::animations::Process* o)
{
    int pos = findFirst(f);
    if (pos < 0)
    {
        addNow(o);
    }
    else
    {
        insertAtNow(static_cast<unsigned int>(pos), o);
    }
}

void sad::animations::Animations::insertAfterLastMatched(const std::function<bool(sad::animations::Process*)>& f, sad::animations::Process* o)
{
    int pos = findLast(f);
    if (pos  < 0)
    {
        addNow(o);
    }
    else
    {
        if ((pos + 1) >= m_list.size())
        {
            addNow(o);
        }
        else
        {
            insertAtNow(static_cast<unsigned int>(pos + 1), o);
        }
    }
}

void sad::animations::Animations::insertBeforeNow(sad::animations::Process* before, sad::animations::Process* o)
{
    insertBeforeFirstMatched([before](sad::animations::Process* m) -> bool { return m == before;}, o);
}

void sad::animations::Animations::insertBeforeNow(const sad::String& name, sad::animations::Process* o)
{
    insertBeforeFirstMatched([name](sad::animations::Process* m) -> bool { return m->objectName() == name; }, o);
}

void sad::animations::Animations::insertBeforeNow(const sad::String& type, const sad::String& name, sad::animations::Process* o)
{
    insertBeforeFirstMatched([type, name](sad::animations::Process* m) -> bool { 
        if (m->metaData()->canBeCastedTo(type))
        { 
            return m->objectName() == name; 
        }
        return false;
    }, o);
}

void sad::animations::Animations::insertBeforeNow(unsigned long long major_id, sad::animations::Process* o)
{
    insertBeforeFirstMatched([major_id](sad::animations::Process* m) -> bool { return m->MajorId == major_id; }, o);
}

void sad::animations::Animations::insertAfterNow(sad::animations::Process* after, sad::animations::Process* o)
{
    insertAfterLastMatched([after](sad::animations::Process* m) -> bool { return m == after; }, o);
}

void sad::animations::Animations::insertAfterNow(const sad::String& name, sad::animations::Process* o)
{
    insertAfterLastMatched([name](sad::animations::Process* m) -> bool { return m->objectName() == name; }, o);
}

void sad::animations::Animations::insertAfterNow(const sad::String& type, const sad::String& name, sad::animations::Process* o)
{
    insertAfterLastMatched([type, name](sad::animations::Process* m) -> bool {
        if (m->metaData()->canBeCastedTo(type))
        {
            return m->objectName() == name;
        }
        return false;
    }, o);
}

void sad::animations::Animations::insertAfterNow(unsigned long long major_id, sad::animations::Process* o)
{
    insertAfterLastMatched([major_id](sad::animations::Process* m) -> bool { return m->MajorId == major_id; }, o);
}

void sad::animations::Animations::pushCommand(const std::function<void()>& f)
{
    m_command_queue_lock.lock();
    m_command_queue.push_back(f);
    m_command_queue_lock.unlock();
}

void sad::animations::Animations::lockChanges()
{
    m_lock_changes_lock.lock();
    m_lock_changes = true;
    m_lock_changes_lock.unlock();
}

void sad::animations::Animations::unlockChanges()
{
    m_lock_changes_lock.lock();
    m_lock_changes = false;
    m_lock_changes_lock.unlock();
}

bool sad::animations::Animations::containerLocked()
{
    m_lock_changes_lock.lock();
    const bool result = m_lock_changes;
    m_lock_changes_lock.unlock();
    return result;
}


void sad::animations::Animations::performQueuedActions()
{
   m_command_queue_lock.lock();
   for(size_t i = 0; i < m_command_queue.count(); i++)
   {
       m_command_queue[i]();
   }
   m_command_queue.clear();
   m_command_queue_lock.unlock();
}


void sad::animations::Animations::performOrQueue(const std::function<void()>& f)
{
    if (containerLocked())
    {
        m_command_queue_lock.lock();
        pushCommand(f);
        m_command_queue_lock.unlock();
    }
    else
    {
        m_lock.lock();
        f();
        m_lock.unlock();
    }
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcesses(const std::function<bool(sad::animations::Process*)>& f)
{
    m_lock.lock();
    sad::Vector<sad::animations::Process*> result;
    for (size_t i = 0; i < m_list.size(); i++)
    {
        if (f(m_list[i]))
        {
            result << m_list[i];
        }
    }
    m_lock.unlock();
    return result;
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesRelatedToMatchedObject(const std::function<bool(sad::db::Object*)>& f)
{
    m_lock.lock();
    sad::Vector<sad::animations::Process*> result;
    for (size_t i = 0; i < m_list.size(); i++)
    {
        if (m_list[i]->isRelatedToMatchedObject(f))
        {
            result << m_list[i];
        }
    }
    m_lock.unlock();
    return result;
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesRelatedToMatchedAnimation(const std::function<bool(sad::animations::Animation*)>& f)
{
    m_lock.lock();
    sad::Vector<sad::animations::Process*> result;
    for (size_t i = 0; i < m_list.size(); i++)
    {
        if (m_list[i]->isRelatedToMatchedAnimation(f))
        {
            result << m_list[i];
        }
    }
    m_lock.unlock();
    return result;
}
