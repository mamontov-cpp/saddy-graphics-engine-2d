/*! \file abstractgetter.h
	\author HiddenSeeker

	Defines a basic getter for property
 */
#pragma once
#include "maybe.h"
#include "callable.h"

#include <db/dbproperty.h>
#include <db/dbobject.h>

namespace scripting
{
	
template<
	typename _Type,
	typename _PropetyType
>
class AbstractGetter: public scripting::Callable
{
public:
	/*! Represents a constructor call for a function with two arguments
		\param[in] e engine
	 */
	AbstractGetter(
		QScriptEngine* e
	) : scripting::Callable(e, "get", 2)
	{
		
	}
	/*! Represents a constructor call for a function with two arguments
		\param[in] e engine
        \param[in] propname name of property
	 */
	AbstractGetter(
		QScriptEngine* e,
		const sad::String& propname
	) : scripting::Callable(e, "get", 2)
	{
		m_matched_property_names << propname;
	}
	/*! Could be inherited
	 */
	virtual ~AbstractGetter()
	{
		
	}
	/*! Pushes new matches property name
		\param[in] name a property name
	 */
	void addMatched(const QString& name)
	{
		m_matched_property_names << name.toStdString();
	}
	/*! Adds excluded property name
		\param[in] name a property name
	 */
	void addExcluded(const QString& name)
	{
		m_excluded_property_names << name.toStdString();
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
		if (result._2().exists() == false)
		{
			sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(1));
			bool propertymatches = true;
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
				sad::Maybe<_Type> basicvalue = scripting::ToValue<_Type>::perform(ctx->argument(0));
				sad::db::Object* object = basicvalue.value();
				sad::db::Property* prop = object->getObjectProperty(propname.value());
				if (prop)
				{
					result._1() += 1;
					sad::db::TypeName<_PropetyType>::init();
					if (prop->baseType() != sad::db::TypeName<_PropetyType>::baseName() || prop->pointerStarsCount() != 0)
					{
						QString qpropname = propname.value().c_str();
						QString basetype = sad::db::TypeName<_PropetyType>::baseName().c_str();
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
		sad::Maybe<_Type>       basicvalue = scripting::ToValue<_Type>::perform(ctx->argument(0)); 
		sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(1));
		
		sad::db::Object* object = basicvalue.value();
		sad::Maybe<_PropetyType> oldvalue = object->getProperty<_PropetyType>(propname.value());

		return scripting::FromValue<_PropetyType>::perform(oldvalue.value(), engine);
	}	
protected:
	/*! A matched properties list. If empty - every kind of property is matched
	 */
	sad::Vector<sad::String> m_matched_property_names;
	/*! A property names, that should be excluded from this name
	 */
	sad::Vector<sad::String> m_excluded_property_names;
};

}
