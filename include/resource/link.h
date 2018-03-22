/*! \file resource/link.h
    
    
    Contains definition of class Link.

    Defines a link to specific type to resource, providing simple access to it.
 */
#pragma once
#include "abstractlink.h"
#include "resource.h"

#include "../sadptrvector.h"

#include "../util/pointercallback.h"
#include "../util/free.h"

#include <algorithm>

namespace sad
{

namespace resource
{

/*! \class Link

    Defines a link to specific type to resource, providing simple access to it.
 */
template<
    typename _Resource
>
class Link: public sad::resource::AbstractLink  
{	
public:	
    /*! A callback for resource
     */
    typedef sad::util::PointerCallback<_Resource>* callback;
    /*! Creates new link to specified resource
     */
    Link() : sad::resource::AbstractLink(_Resource::globalMetaData()->name())
    {
    
    }
    /*! Creates a new link to specified resource
        \param[in] o a resource link
     */
    Link(const sad::resource::Link<_Resource> & o) 
    : sad::resource::AbstractLink(o)
    {
        
        if (m_resource)
        {
            m_resource->addLink(this);
        }
        this->copyCallbacks(o);
    }

    Link<_Resource> & operator=(const sad::resource::Link<_Resource> & o)
    {
        if (m_resource)
        {
            m_resource->removeLink(this);
        }

        this->m_notify_resource = o.m_notify_resource;
        this->m_render_dependent = o.m_render_dependent;
        this->m_path = o.m_path;
        this->m_resource = o.m_resource;
        this->m_tree = o.m_tree;

        if (m_resource)
        {
            m_resource->addLink(this);
        }
        this->copyCallbacks(o);
        return *this;
    }

    /*! Adds a callback to a link
        \param[in] callback added callback
     */
    callback addCallback(sad::util::PointerCallback<_Resource>* callback)
    {
        if (callback 
            && std::find(m_callbacks.begin(), m_callbacks.end(), callback) == m_callbacks.end())
        {
            m_callbacks << callback;
        }
        return callback;
    }
    /*! Adds a function callback with no arguments
        \param[in] f argument
        \return a callback
     */
    callback add(void (*f)())
    {
        return addCallback(new sad::util::FreeZeroArgCallback<_Resource>(f));
    }
    /*! Adds a function callback with one argument
        \param[in] f argument
        \return a callback
     */
    callback add(void (*f)(_Resource *))
    {
        return addCallback(new sad::util::FreeOneArgCallback<_Resource>(f));
    }
    /*! Adds a method call callback with no arguments
        \param[in] o an object
        \param[in] f a called method
        \return a callback
     */
    template<
        typename _Object,
        typename _CalledMethod
    >
    callback add(_Object * o, void (_CalledMethod::*f)())
    {
        return addCallback(new sad::util::MethodZeroArgCallback<_Resource, _Object, _CalledMethod>(o, f));
    }
    /*! Adds a method call callback with no arguments
        \param[in] o an object
        \param[in] f a called method
        \return a callback
     */
    template<
        typename _Object,
        typename _CalledMethod
    >
    callback add(_Object * o, void (_CalledMethod::*f)() const)
    {
        return addCallback(new sad::util::ConstMethodZeroArgCallback<_Resource, _Object, _CalledMethod>(o, f));
    }
    /*! Adds a method call callback with no arguments
        \param[in] o an object
        \param[in] f a called method
        \return a callback
     */
    template<
        typename _Object,
        typename _CalledMethod
    >
    callback add(_Object * o, void (_CalledMethod::*f)(_Resource *))
    {
        return addCallback(new sad::util::MethodOneArgCallback<_Resource, _Object, _CalledMethod>(o, f));
    }
    /*! Adds a method call callback with no arguments
        \param[in] o an object
        \param[in] f a called method
        \return a callback
     */
    template<
        typename _Object,
        typename _CalledMethod
    >
    callback add(_Object * o, void (_CalledMethod::*f)(_Resource *) const)
    {
        return addCallback(new sad::util::ConstMethodOneArgCallback<_Resource, _Object, _CalledMethod>(o, f));
    }
    /*! Adds a composed method call callback with no arguments
        \param[in] o an object
        \param[in] f a called method
        \param[in] g a second composed method
        \return a callback
     */
    template<
        typename _Object,
        typename _FCalledObject,
        typename _RetObject,
        typename _GCalledObject
    >
    callback add(_Object * o, _RetObject (_FCalledObject::*f)(), void (_GCalledObject::*g)())
    {
        return addCallback(new sad::util::CompositionZeroArgCallback<_Resource, _Object, _FCalledObject, _RetObject, _GCalledObject>(o, f, g));
    }
    /*! Adds a composed method call callback with no arguments
        \param[in] o an object
        \param[in] f a called method
        \param[in] g a second composed method
        \return a callback
     */
    template<
        typename _Object,
        typename _FCalledObject,
        typename _RetObject,
        typename _GCalledObject
    >
    callback add(_Object * o, _RetObject (_FCalledObject::*f)() const, void (_GCalledObject::*g)())
    {
        return addCallback(new sad::util::CompositionZeroArgCallbackC1<_Resource, _Object, _FCalledObject, _RetObject, _GCalledObject>(o, f, g));
    }
    /*! Adds a composed method call callback with no arguments
        \param[in] o an object
        \param[in] f a called method
        \param[in] g a second composed method
        \return a callback
     */
    template<
        typename _Object,
        typename _FCalledObject,
        typename _RetObject,
        typename _GCalledObject
    >
    callback add(_Object * o, _RetObject (_FCalledObject::*f)(), void (_GCalledObject::*g)() const)
    {
        return addCallback(new sad::util::CompositionZeroArgCallbackC2<_Resource, _Object, _FCalledObject, _RetObject, _GCalledObject>(o, f, g));
    }
    /*! Adds a composed method call callback with no arguments
        \param[in] o an object
        \param[in] f a called method
        \param[in] g a second composed method
        \return a callback
     */
    template<
        typename _Object,
        typename _FCalledObject,
        typename _RetObject,
        typename _GCalledObject
    >
    callback add(_Object * o, _RetObject (_FCalledObject::*f)() const, void (_GCalledObject::*g)() const)
    {
        return addCallback(new sad::util::CompositionZeroArgCallbackC12<_Resource, _Object, _FCalledObject, _RetObject, _GCalledObject>(o, f, g));
    }
    /*! Adds a composed method call callback with no arguments
        \param[in] o an object
        \param[in] f a called method
        \param[in] g a second composed method
        \return a callback
     */
    template<
        typename _Object,
        typename _FCalledObject,
        typename _RetObject,
        typename _GCalledObject
    >
    callback add(
        _Object * o, 
        _RetObject (_FCalledObject::*f)(), 
        void (_GCalledObject::*g)(_Resource *)
    )
    {
        return addCallback(new sad::util::CompositionOneArgCallback<_Resource, _Object, _FCalledObject, _RetObject, _GCalledObject>(o, f, g));
    }
    /*! Adds a composed method call callback with no arguments
        \param[in] o an object
        \param[in] f a called method
        \param[in] g a second composed method
        \return a callback
     */
    template<
        typename _Object,
        typename _FCalledObject,
        typename _RetObject,
        typename _GCalledObject
    >
    callback add(
        _Object * o, 
        _RetObject (_FCalledObject::*f)() const, 
        void (_GCalledObject::*g)(_Resource *)
    )
    {
        return addCallback(new sad::util::CompositionOneArgCallbackC1<_Resource, _Object, _FCalledObject, _RetObject, _GCalledObject>(o, f, g));
    }
    /*! Adds a composed method call callback with no arguments
        \param[in] o an object
        \param[in] f a called method
        \param[in] g a second composed method
        \return a callback
     */
    template<
        typename _Object,
        typename _FCalledObject,
        typename _RetObject,
        typename _GCalledObject
    >
    callback add(
        _Object * o, 
        _RetObject (_FCalledObject::*f)(), 
        void (_GCalledObject::*g)(_Resource *) const
    )
    {
        return addCallback(new sad::util::CompositionOneArgCallbackC2<_Resource, _Object, _FCalledObject, _RetObject, _GCalledObject>(o, f, g));
    }
    /*! Adds a composed method call callback with no arguments
        \param[in] o an object
        \param[in] f a called method
        \param[in] g a second composed method
        \return a callback
     */
    template<
        typename _Object,
        typename _FCalledObject,
        typename _RetObject,
        typename _GCalledObject
    >
    callback add(
        _Object * o, 
        _RetObject (_FCalledObject::*f)() const, 
        void (_GCalledObject::*g)(_Resource *) const
    )
    {
        return addCallback(new sad::util::CompositionOneArgCallbackC12<_Resource, _Object, _FCalledObject, _RetObject, _GCalledObject>(o, f, g));
    }
    /*! Removes a callback from list of attached callback
        \param[in] callback a callback data
     */
    void removeCallback(sad::util::PointerCallback<_Resource>* callback)
    {
        m_callbacks.removeAll(callback);
    }

    /*! Attaches a link to specified resource
        \param[in] r a new resource, which links should be attached to
     */
    virtual void attach(sad::resource::Resource* r)
    {
        this->sad::resource::AbstractLink::attach(r);
        if (r)
        {
            _Resource * cr = static_cast<_Resource*>(r);
            for(size_t i = 0; i < m_callbacks.size(); i++)
            {
                m_callbacks[i]->call(cr);
            }
        }
    }
    /*! Returns resource of specific type. No run-time checks are performed due
        possible errors
        \return	resource of specific type
     */
    inline _Resource* get() const
    {
        return static_cast<_Resource*>(this->resource());
    }
protected:
    /*! Copies callbacks from other link
        \param[in] o resources
     */
    void copyCallbacks(const sad::resource::Link<_Resource> & o)
    {
        sad::util::free(m_callbacks);
        m_callbacks.clear();
        for(size_t i = 0; i < o.m_callbacks.size(); i++)
        {
            m_callbacks << o.m_callbacks[i]->copy();
        }
    }
    /*! A callbacks, should executed for attachment
     */
    sad::PtrVector<sad::util::PointerCallback<_Resource> > m_callbacks;
};

}

}
