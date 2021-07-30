/*! \file collisionmultimethod.h
    

    Describes a collision multi-method, used to call
    various type-dependent functions
 */
#pragma once
#include "collisionshape.h"
#include "../sadptrhash.h"

namespace sad
{

namespace p2d
{

/*! Describes a basic method for invocation a multi-method
 */
template<typename _ReturnType>
class BasicCollisionMultiMethodInstance
{
 public:
     /*! Invokes a method
         \param[in] a1 first object
         \param[in] a2 second object
         \return returned type
      */
     virtual _ReturnType invoke(p2d::CollisionShape * a1, p2d::CollisionShape * a2) = 0;
     /*! Returns true, if arguments will be reversed on execution
         \return reverse flag
      */
     virtual bool reverse() const = 0;
     virtual ~BasicCollisionMultiMethodInstance() = default;
};

/*! Describes a basic method for invocation a multi-method with arguments
 */
template<typename _ReturnType, typename _Arg>
class BasicCollisionMultiMethodInstanceWithArg
{
 public:
     /*! Invokes a method
         \param[in] a1 first object
         \param[in] ak1 argument, related to first object
         \param[in] a2 second object
         \param[in] ak2 argument, related to second object
         \return returned type
      */
     virtual _ReturnType invoke(
         p2d::CollisionShape * a1, 
         const _Arg & ak1, 
         p2d::CollisionShape * a2,
         const _Arg & ak2) = 0;
     /*! Returns true, if arguments will be reversed on execution
         \return reverse flag
      */
     virtual bool reverse() const = 0;
	 virtual ~BasicCollisionMultiMethodInstanceWithArg() = default;
};

template<
typename _ReturnType, 
typename _FirstObject,
typename _SecondObject
>
class CollisionMultiMethodInstance: 
public  p2d::BasicCollisionMultiMethodInstance<_ReturnType>
{ 
public:
    /*! Constructs new instance
        \param[in] p pointer to free function
        \param[in] reverse whether arguments should be reversed
     */
    inline CollisionMultiMethodInstance(
          _ReturnType (*p)(_FirstObject * o1, _SecondObject * o2),
          bool reverse
    ) : m_p(p), m_reverse(reverse) {}
    /*! Returns true, if arguments will be reversed on execution
         \return reverse flag
      */
    bool reverse() const override { return m_reverse; }
     /*! Invokes a method
         \param[in] a1 first object
         \param[in] a2 second object
         \return returned type
      */
     virtual _ReturnType invoke(p2d::CollisionShape * a1, p2d::CollisionShape * a2) override
     {
         // Checked casts are replaced with static cast, since
         // type-checking is performed in p2d::CollisionMultiMethod
         if (!m_reverse)
         {
          _FirstObject * _a1 = static_cast<_FirstObject *>(a1);
          _SecondObject * _a2 = static_cast<_SecondObject *>(a2);
          return m_p(_a1, _a2);
         }
         _FirstObject * _a1 = static_cast<_FirstObject *>(a2);
         _SecondObject * _a2 = static_cast<_SecondObject *>(a1);
         return m_p(_a1, _a2);
     }
     virtual ~CollisionMultiMethodInstance() {}
private:
     _ReturnType (*m_p)(_FirstObject * o1, _SecondObject * o2); //!< An inner function to call
     bool m_reverse;  //!< Whether arguments should be reversed	
};


template<
typename _ReturnType, 
typename _Arg,
typename _FirstObject,
typename _SecondObject
>
class CollisionMultiMethodInstanceWithArg: 
public  p2d::BasicCollisionMultiMethodInstanceWithArg<_ReturnType, _Arg>
{
public:
     typedef _ReturnType (*inner_t)(_FirstObject * o1, 
                                const _Arg & a1, 
                                _SecondObject * o2,
                                const _Arg & a2
                               );
public:
    /*! Constructs new instance
        \param[in] p pointer to free function
        \param[in] reverse whether arguments should be reversed
     */
    inline CollisionMultiMethodInstanceWithArg(
          inner_t p,
          bool reverse
    ) : m_p(p), m_reverse(reverse) {}
    /*! Returns true, if arguments will be reversed on execution
         \return reverse flag
      */
    bool reverse() const  override { return m_reverse; }
     /*! Invokes a method
         \param[in] a1 first object
         \param[in] ak1 argument, related to first object
         \param[in] a2 second object
         \param[in] ak2 argument, related to second object
         \return returned type
      */
     virtual _ReturnType invoke(
         p2d::CollisionShape * a1, 
         const _Arg & ak1, 
         p2d::CollisionShape * a2,
         const _Arg & ak2
         ) override
     {
         if (!m_reverse)
         {
          _FirstObject * _a1 = static_cast<_FirstObject*>(a1);
          _SecondObject * _a2 = static_cast<_SecondObject*>(a2);
          return m_p(_a1, ak1, _a2, ak2);
         }
         _FirstObject * _a1 = static_cast<_FirstObject*>(a2);
         _SecondObject * _a2 = static_cast<_SecondObject*>(a1);
         return m_p(_a1, ak2, _a2,  ak1);
     }
     virtual ~CollisionMultiMethodInstanceWithArg() {}
private:
     inner_t m_p; //!< An inner function to call
     bool m_reverse;  //!< Whether arguments should be reversed	 
};


/*! An amount of registered multimethod type
 */
#define MULTIMETHOD_REGISTERED_TYPES 4

/*! Defines a multi-method as set of specific methods
 */
template<typename _ReturnType>
class CollisionMultiMethod
{
    public:
        typedef p2d::BasicCollisionMultiMethodInstance<_ReturnType> instance_t;
        typedef sad::PtrHash<sad::String, instance_t> group_t;
        typedef instance_t * instances_t[MULTIMETHOD_REGISTERED_TYPES][MULTIMETHOD_REGISTERED_TYPES];
    protected:
        instances_t m_instances; //!< Instances of method
        bool m_init;             //!< Whether multimethod initialized
        /*! This function initializes all callbacks. 
            You should add your dispatchers here 
         */
        virtual void init() 
        {
            for (auto& m_instance : m_instances)
            {
                for(int j = 0; j < MULTIMETHOD_REGISTERED_TYPES; j++)
                {
                    m_instance[j] = nullptr;
                }
            }
        }
        /*!  Registers new callbacks. You should place calls of this 
             functions in init.
         */
        template<typename _First, typename _Second>
        void add( _ReturnType (*p)(_First *, _Second *) )
        {
            unsigned int fst = _First::globalMetaIndex();
            unsigned int snd = _Second::globalMetaIndex();
            assert( fst < MULTIMETHOD_REGISTERED_TYPES );
            assert( snd < MULTIMETHOD_REGISTERED_TYPES );
            if (m_instances[fst][snd] == nullptr)
            {
                p2d::CollisionMultiMethodInstance<_ReturnType, _First, _Second>
                * a = 
                new  
                p2d::CollisionMultiMethodInstance<_ReturnType, _First, _Second>(
                p, false
                );
                m_instances[fst][snd] = a;
            }
            // Try insert reverse call
            if (m_instances[snd][fst] == nullptr)
            {
                p2d::CollisionMultiMethodInstance<_ReturnType, _First, _Second>
                * a = 
                new  
                p2d::CollisionMultiMethodInstance<_ReturnType, _First, _Second>(
                p, true
                );
                m_instances[snd][fst] = a;
            }
        }
        /*! Lookups for needed instance of multimethod
            \param[in] a first shape
            \param[in] b second shape
            \return instance if found
         */
        instance_t * lookup(CollisionShape * a, CollisionShape * b)
        {
            unsigned int type1 = a->metaIndex();
            unsigned int type2 = b->metaIndex();
            instance_t* result = m_instances[type1][type2];
            return result;
        }
    public:
        CollisionMultiMethod() { m_init = false;}
        /*! Invokes a multi-method, if possible. Returns default object,
            if can't handle.
            \param[in] a first shape
            \param[in] b second shape
         */
        virtual _ReturnType invoke(CollisionShape * a, CollisionShape * b)
        {
            if (!m_init)
            {
                m_init = true;
                init();
            }
            instance_t * d = lookup(a,b);
            if (!d) return _ReturnType();
            return d->invoke(a, b);
        }
        virtual ~CollisionMultiMethod()
        {
            if (m_init)
            {
                for (auto& m_instance : m_instances)
                {
                    for(int j = 0; j < MULTIMETHOD_REGISTERED_TYPES; j++)
                    {
                        delete m_instance[j];
                    }
                }
            }
        }
};

/*! Defines a multi-method as set of specific methods
 */
template<typename _ReturnType, typename _Arg>
class CollisionMultiMethodWithArg
{
    public:
        typedef p2d::BasicCollisionMultiMethodInstanceWithArg<_ReturnType, _Arg> instance_t;
        typedef sad::PtrHash<sad::String, instance_t> group_t;
        typedef instance_t * instances_t[MULTIMETHOD_REGISTERED_TYPES][MULTIMETHOD_REGISTERED_TYPES];
    protected:
        instances_t m_instances; //!< Instances of method
        bool m_init;             //!< Whether multimethod initialized
        /*! This function initializes all callbacks. 
            You should add your dispatchers here 
         */
        virtual void init() 
        { 
            for (auto& m_instance : m_instances)
            {
                for(int j = 0; j < MULTIMETHOD_REGISTERED_TYPES; j++)
                {
	                m_instance[j] = nullptr;
                }
            }		
        }
        /*!  Registers new callbacks. You should place calls of this 
             functions in init.
         */
        template<typename _First, typename _Second>
        void add( _ReturnType (*p)(_First *,const _Arg&,  _Second *, const _Arg&) )
        {
            unsigned int fst = _First::globalMetaIndex();
            unsigned int snd = _Second::globalMetaIndex();
            assert( fst < MULTIMETHOD_REGISTERED_TYPES );
            assert( snd < MULTIMETHOD_REGISTERED_TYPES );
            if (m_instances[fst][snd] == nullptr)
            {
                p2d::CollisionMultiMethodInstanceWithArg<_ReturnType, _Arg, _First, _Second>
                * a = 
                new  
                p2d::CollisionMultiMethodInstanceWithArg<_ReturnType, _Arg, _First, _Second>(
                p, false
                );
                m_instances[fst][snd] = a;
            }
            // Try insert reverse call
            if (m_instances[snd][fst] == nullptr)
            {
                p2d::CollisionMultiMethodInstanceWithArg<_ReturnType, _Arg, _First, _Second>
                * a = 
                new  
                p2d::CollisionMultiMethodInstanceWithArg<_ReturnType, _Arg, _First, _Second>(
                p, true
                );
                m_instances[snd][fst] = a;
            }
        }
        /*! Lookups for needed instance of multimethod
            \param[in] a first shape
            \param[in] b second shape
            \return instance if found
         */
        instance_t * lookup(CollisionShape * a, CollisionShape * b)
        {
            unsigned int type1 = a->metaIndex();
            unsigned int type2 = b->metaIndex();
            instance_t* result = m_instances[type1][type2];
            return result;
        }
        /*! Reverses a parts return type if need to
            \param[in] result result
         */
        virtual void reverse(_ReturnType & result)
        {
        }
    public:
        CollisionMultiMethodWithArg() { m_init = false;}
        /*! Invokes a multi-method, if possible. Returns default object,
            if can't handle.
            \param[in] a first shape
            \param[in] ka argument, related to first shape
            \param[in] b second shape
            \param[in] kb argument, related to second shape			
         */
        virtual _ReturnType invoke(CollisionShape * a, const _Arg & ka, CollisionShape * b, const _Arg & kb)
        {
            if (!m_init)
            {
                m_init = true;
                init();
            }
            instance_t * d = lookup(a,b);
            if (!d) return _ReturnType();
            _ReturnType k = d->invoke(a, ka, b, kb);
            if (d->reverse())
                reverse(k);
            return k;
        }
        virtual ~CollisionMultiMethodWithArg()
        {
            if (m_init)
            {
                for (auto& m_instance : m_instances)
                {
                    for(int j = 0; j < MULTIMETHOD_REGISTERED_TYPES; j++)
                    {
                        delete m_instance[j];
                    }
                }
            }
        }
};

}

}
