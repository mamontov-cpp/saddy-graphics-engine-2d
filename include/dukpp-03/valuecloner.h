/*! \file valuecloner.h
 
    Defines a callable stuff, which can be used to clone object by value. Copy constructor is needed in that case.
 */
#pragma once
#include "basiccontext.h"

namespace sad
{
    
namespace dukpp03
{

/*! A scriptable object cloner. Copy constructor is required from T.
 */
template<typename T>
class ValueCloner: public ::dukpp03::Callable<sad::dukpp03::BasicContext>
{
public:
    /*! Makes new accessor
     */
    ValueCloner()
    {
        
    }
    /*! Performs check, whether callable can be called, without actually calling an object in context
        \param[in] c context
        \return pair
     */
    virtual std::pair<int, bool> canBeCalled(sad::dukpp03::BasicContext* c) override
    {
        if (c->getTop() != this->requiredArguments())
        {
            return std::make_pair(-1, false);
        }
        int matched_arguments = 0;
        ::dukpp03::Maybe<T> obj = this->checkThis(c, false);
        if (obj.exists())
        {
            matched_arguments += 1;
        }
        return std::make_pair(matched_arguments, matched_arguments == (this->requiredArguments() + 1));
    }
    /*! Could be inherited
     */
    virtual ~ValueCloner()
    {
        
    }
    /*! Returns false
        \return false
     */
    virtual bool canBeCalledAsConstructor() override
    {
        return false;
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual Callable<sad::dukpp03::BasicContext>* clone() override
    {
        return new sad::dukpp03::ValueCloner<T>();
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments() override
    {
        return 0;
    }
    /*! Performs actual function call
        \param[in] c context
     */
    virtual int _call(sad::dukpp03::BasicContext* c) override
    {
        ::dukpp03::Maybe<T> obj = this->checkThis(c, true);
        if (obj.exists())
        {
            ::dukpp03::PushValue<T, sad::dukpp03::BasicContext>::perform(c, T(obj.value()));
            return 1;
        }
        return 0;
    }
protected:
    /*! Checks first argument to be called
        \param[in] c context
        \param[in] throw_error if true, throws error if cannot be called
        \return nullptr if method cannot be called
     */
    ::dukpp03::Maybe<T>  checkThis(sad::dukpp03::BasicContext* c, bool throw_error) const
    {
        ::dukpp03::Maybe<T>  maybe_this_object;

        sad::dukpp03::BasicContext::LocalCallable::CheckArgument< T >::passedAsThis(c, maybe_this_object);
        if (maybe_this_object.exists())
        {
            return maybe_this_object;
        }
        else
        {
            if (throw_error)
            {
                sad::db::TypeName<T>::init();
                c->throwError(sad::String("The object, passed as this, is not a ") + sad::db::TypeName<T>::name());
            }
        }
        return nullptr;
    }
};  
    

}

}
