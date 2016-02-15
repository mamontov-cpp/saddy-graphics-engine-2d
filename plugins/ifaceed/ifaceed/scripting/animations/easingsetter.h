/*! \file easingsetter.h
    

    Defines a setter for easing function properties
 */
#pragma once
#include "../abstractsetter.h"

#include "../scripting.h"

#include "../../core/editor.h"

#include <animations/animationsanimation.h>
#include <animations/easing/easingfunction.h>


namespace scripting
{
    
namespace animations
{

template<
    typename _PropertyType,
	typename _CommandType
>
class EasingSetter: public scripting::AbstractSetter<sad::animations::Animation*, _PropertyType>
{
public:
    /*! Represents a constructor call for a function with two arguments
        \param[in] e engine
        \param[in] name name of function call
		\param[in] getter a getter for property
     */
    EasingSetter(
        QScriptEngine* e, 
        const QString& name,
		_PropertyType (sad::animations::easing::Function::*getter)() const
    ) : scripting::AbstractSetter<sad::animations::Animation*, _PropertyType>(e, name), m_getter(getter)
    {
		if (name.length()) {
            this->addMatched(name);
        }
	}
    /*! Could be inherited
     */
    virtual ~EasingSetter()
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
        checkArgumentCount(result, ctx);
        checkArgument<sad::animations::Animation*>(result, 0, ctx);
        checkArgument<sad::String>(result, 1, ctx);
        // Use converters to enhance match result
        if (result._2().exists() == false)
        {
            QScriptValue argt =  ctx->argument(2);
            sad::Maybe<_PropertyType> value;
            for(size_t i = 0; i < m_converts.size() && value.exists() == false; i++)
            {
                value = m_converts[i]->toValue(argt);
            }
            if (value.exists() == false)
            {
                sad::db::TypeName<_PropertyType>::init();
                QString tname = sad::db::TypeName<_PropertyType>::baseName().c_str();
                QString argstr = QString::number(3);
                result._2().setValue(QString("must have argument ") + argstr + QString(" of type ") + tname);
            }
            else
            {
                result._1() += 1;
                for(size_t i = 0; i < m_conditons.size() && result._2().exists() == false; i++)
                {
                    result._2() = m_conditons[i]->check(value.value());
                }
            }
        }

        bool propertymatches = true;
        sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(1));			
        if (propname.exists())
        {
            if (m_matched_property_names.size())
            {
                propertymatches = std::find(
                    m_matched_property_names.begin(), 
                    m_matched_property_names.end(), 
                    propname.value()
                ) != m_matched_property_names.end();
            }
            if (m_excluded_property_names.size() && propertymatches)
            {
                propertymatches = std::find(
                    m_excluded_property_names.begin(), 
                    m_excluded_property_names.end(), 
                    propname.value()
                ) == m_excluded_property_names.end();
            }
            if (propertymatches)
            {
                result._1() += 1;				
            }
			else
			{
				result._1()  = 0;
				result._2().setValue(QString("property ") + STD2QSTRING(propname.value()) + QString(" is not writeable"));
			}
        }
        
        return result;
    }
    /*! Calls actually a function
        \param[in] ctx context
     */
    virtual QScriptValue call(QScriptContext* ctx, QScriptEngine*)
    {
        sad::Maybe<sad::animations::Animation*>       basicvalue = scripting::ToValue<sad::animations::Animation*>::perform(ctx->argument(0)); 
        sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(1));

        QScriptValue argt =  ctx->argument(2);
        sad::Maybe<_PropertyType> newvalue;
        for(size_t i = 0; i < m_converts.size() && newvalue.exists() == false; i++)
        {
            newvalue = m_converts[i]->toValue(argt);
        }
       
        _PropertyType oldvalue = ((basicvalue.value()->easing())->*m_getter)();

        std::equal_to<_PropertyType> comparator;
        if (comparator(newvalue.value(), oldvalue) == false)
        {
            setProperty(basicvalue.value(), propname.value(), oldvalue, newvalue.value());
        }
        return ctx->thisObject();
    }
    /*! Performs making new command and committing it
        \param[in] obj an object to be set
        \param[in] propertyname a property for object
        \param[in] oldvalue old value 
        \param[in] newvalue new value
     */
    virtual void setProperty(sad::animations::Animation* obj, const sad::String& propertyname, _PropertyType oldvalue,  _PropertyType newvalue)
    {
		QScriptValue main = this->engine()->globalObject().property("---");
        scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
        core::Editor* editor =  e->editor();

	    _CommandType* c = new _CommandType(obj, oldvalue, newvalue);
		c->commit(editor);
		editor->currentBatchCommand()->add(c);
    }
protected:
	/*! A getter for easing function
	 */
	_PropertyType (sad::animations::easing::Function::*m_getter)() const;
};

}

}
