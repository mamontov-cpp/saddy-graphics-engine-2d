/*! \file abstractsetter.h
	\author HiddenSeeker

	Defines a basic setter for property
 */
#pragma once
#include "maybe.h"
#include "callable.h"
#include "tovalue.h"
#include "abstractcondition.h"

#include "../qstdstring.h"

#include <equalto.h>

#include <db/dbproperty.h>
#include <db/dbobject.h>

#include <sadptrvector.h>

namespace scripting
{
	
template<
	typename _Type,
	typename _PropertyType
>
class AbstractSetter: public scripting::Callable
{
public:
	/*! Represents a constructor call for a function with two arguments
		\param[in] e engine
		\param[in] name name of function call
	 */
	AbstractSetter(
		QScriptEngine* e, 
		const QString& name
	) : scripting::Callable(e, name ,3)
	{
		addConverter(new scripting::ToValue<_PropertyType>());
	}
	/*! Could be inherited
	 */
	virtual ~AbstractSetter()
	{
		
	}
	/*! Adds new converter to a setter
		\param[in] a converter 
	 */
	void addConverter(scripting::AbstractToValue<_PropertyType>* a)
	{
		m_converts << a;
	}
	/*! Adds a conditions to a setter
		\param[in] a converter
	 */
	void addCondition(scripting::AbstractCondition<_PropertyType>* a)
	{
		m_conditons << a;
	}
	/*! Pushes new matches property name
		\param[in] name a property name
	 */
	void addMatched(const QString& name)
	{
		m_matched_property_names << Q2STDSTRING(name);
	}
	/*! Adds excluded property name
		\param[in] name a property name
	 */
	void addExcluded(const QString& name)
	{
		m_excluded_property_names << Q2STDSTRING(name);
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
		checkArgument<_Type>(result, 0, ctx);
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
		}

		if (result._2().exists() == false)
		{			
			if (propertymatches)
			{
				sad::Maybe<_Type> basicvalue = scripting::ToValue<_Type>::perform(ctx->argument(0));
				sad::db::Object* object = basicvalue.value();
				sad::db::Property* prop = object->getObjectProperty(propname.value());
				if (prop)
				{
					result._1() += 1;
					sad::db::TypeName<_PropertyType>::init();
					if (prop->baseType() != sad::db::TypeName<_PropertyType>::baseName() || prop->pointerStarsCount() != 0)
					{
						QString qpropname = propname.value().c_str();
						QString basetype = sad::db::TypeName<_PropertyType>::baseName().c_str();
						result._2().setValue(QString("property ") + qpropname + QString(" is not of type ") + basetype);
					}
				}
				else
				{
					propertymatches = false;
				}
			}

			if (propertymatches == false)
			{
				QString qpropname = propname.value().c_str();
				result._2().setValue(QString("property ") + qpropname + QString(" is not writeable"));
			}
		}
		return result;
	}
	/*! Calls actually a function
		\param[in] ctx context
	 */
	virtual QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_Type>       basicvalue = scripting::ToValue<_Type>::perform(ctx->argument(0)); 
		sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(1));

		QScriptValue argt =  ctx->argument(2);
		sad::Maybe<_PropertyType> newvalue;
		for(size_t i = 0; i < m_converts.size() && newvalue.exists() == false; i++)
		{
			newvalue = m_converts[i]->toValue(argt);
		}

		sad::db::Object* object = basicvalue.value();
		sad::Maybe<_PropertyType> oldvalue = object->getProperty<_PropertyType>(propname.value());

		std::equal_to<_PropertyType> comparator;
		if (comparator(newvalue.value(), oldvalue.value()) == false)
		{
			setProperty(basicvalue.value(), propname.value(), oldvalue.value(), newvalue.value());
		}
		return ctx->thisObject();
	}
	/*! Performs actually setting property
		\param[in] obj an object to be set
		\param[in] propertyname a property for object
		\param[in] oldvalue old value 
		\param[in] newvalue new value
	 */
	virtual void setProperty(_Type obj, const sad::String& propertyname, _PropertyType oldvalue,  _PropertyType newvalue) = 0;
protected:
	/*! A matched properties list. If empty - every kind of property is matched
	 */
	sad::Vector<sad::String> m_matched_property_names;
	/*! A property names, that should be excluded from this name
	 */
	sad::Vector<sad::String> m_excluded_property_names;
	/*! Describes a list of converters for setter
	 */
	sad::PtrVector<scripting::AbstractToValue<_PropertyType> > m_converts;
	/*! Describes a list of conditions for setter
	 */
	sad::PtrVector<scripting::AbstractCondition<_PropertyType> > m_conditons;
};

}
