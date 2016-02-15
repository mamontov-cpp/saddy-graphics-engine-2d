/*! \file easinggetter.h

	A getter for property of easing function for animation
 */
#include "../abstractgetter.h"

#include <animations/animationsanimation.h>
#include <animations/easing/easingfunction.h>


namespace scripting
{

namespace animations
{
    
template<
    typename _PropertyType
>
class EasingGetter: public scripting::AbstractGetter<sad::animations::Animation*, _PropertyType>
{
public:
    /*! Represents a constructor call for a function with two arguments
        \param[in] e engine
		\param[in] getter a getter
     */
    EasingGetter(
        QScriptEngine* e,
		_PropertyType (sad::animations::easing::Function::*getter)() const
    ) : scripting::AbstractGetter<sad::animations::Animation*, _PropertyType>(e), m_getter(getter)
    {
        
    }
    /*! Represents a constructor call for a function with two arguments
        \param[in] e engine
        \param[in] propname name of property
     */
    EasingGetter(
        QScriptEngine* e,
        const sad::String& propname,
		_PropertyType (sad::animations::easing::Function::*getter)() const
    )  : scripting::AbstractGetter<sad::animations::Animation*, _PropertyType>(e, propname), m_getter(getter)
    {

	}
    /*! Could be inherited
     */
    virtual ~EasingGetter()
    {
        
    }

    /*! Determines, whether it can be called with this context
        \param[in] ctx context
        \return whether it could be called, or error
     */
    virtual scripting::MatchResult canBeCalled(QScriptContext* ctx)
    {
        scripting::MatchResult result;
        result._1() = 0;

        this->scripting::Callable::checkArgumentCount(result, ctx);
        this->scripting::Callable::checkArgument<sad::animations::Animation*>(result, 0, ctx);
        this->scripting::Callable::checkArgument<sad::String>(result, 1, ctx);
        if (result._2().exists() == false)
        {
            sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(1));
            bool propertymatches = true;
            if (this->m_matched_property_names.size())
            {
                propertymatches = std::find(
                    this->m_matched_property_names.begin(), 
                    this->m_matched_property_names.end(), 
                    propname.value()
                ) != this->m_matched_property_names.end();
            }
            if (this->m_excluded_property_names.size() && propertymatches)
            {
                propertymatches = std::find(
                    this->m_excluded_property_names.begin(), 
                    this->m_excluded_property_names.end(), 
                    propname.value()
                ) == this->m_excluded_property_names.end();
            }            

            if (propertymatches == false)
            {
                QString qpropname = propname.value().c_str();
                result._2().setValue(QString("property ") + qpropname + QString(" is not readable"));
            }
        }
        return result;
    }
    /*! Calls actually a function
        \param[in] ctx context
        \param[in] engine an engine
     */
    virtual QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
    {
        sad::Maybe<sad::animations::Animation*>       basicvalue = scripting::ToValue<sad::animations::Animation*>::perform(ctx->argument(0));                 
        _PropertyType value = ((basicvalue.value()->easing())->*m_getter)();
        return scripting::FromValue<_PropertyType>::perform(value, engine);
    }	
protected:
	/*! A getter for easing
	 */
	_PropertyType (sad::animations::easing::Function::*m_getter)() const;
};

}

}
