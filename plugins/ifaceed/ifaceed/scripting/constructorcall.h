/*! \file constructorcall.h
	\author HiddenSeeker

	Describes a constructor call, which returns new constructed object
 */
#pragma once
#include <QScriptClass>
#include <QScriptContext>

Q_DECLARE_METATYPE(QScriptContext*)

#include "fromvalue.h"
#include "tovalue.h"

#include <db/dbtypename.h>

namespace scripting
{
	
/*! A functor class, which represents constructable call with 2 arguments
 */
template<
	typename _Constructed,
	typename _Arg1,
	typename _Arg2
>
class ConstructorCall2: public QScriptClass
{
private:
	/*! A name of call
	 */
	QString m_name;
public:
	/*! Represents a constructor call for a function with two arguments
	 */
	ConstructorCall2(QScriptEngine* e, const QString& name) : QScriptClass(e), m_name(name)
	{
		
	}
	virtual ~ConstructorCall2()
	{
	
	}

	QVariant extension( Extension extension, const QVariant & argument = QVariant() )
	{
		QVariant v;
		if (extension == Callable) {
			 QScriptContext *context = qvariant_cast<QScriptContext*>(argument);
			 QScriptEngine *engine = context->engine();
			 QScriptValue tmp;
			 int a = context->argumentCount();
			 if (a != 2)
			 {
				 context->throwError(QScriptContext::SyntaxError, m_name + QString(" : accepts only 2 arguments"));
				 return tmp.toVariant();
			 }
			 sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(context->argument(0));
			 sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(context->argument(0));
			 if (value1.exists() == false)
			 {
				 sad::db::TypeName<_Arg1>::init();
				 QString tname = sad::db::TypeName<_Arg1>::baseName().c_str();
				 context->throwError(QScriptContext::SyntaxError, m_name + QString(" : must have argument 1 of type ") + tname);
				 return tmp.toVariant();
			 }
			 if (value2.exists() == false)
			 {
				 sad::db::TypeName<_Arg2>::init();
				 QString tname = sad::db::TypeName<_Arg2>::baseName().c_str();
				 context->throwError(QScriptContext::SyntaxError, m_name + QString(" : must have argument 2 of type ") + tname);
				 return tmp.toVariant();
			 }
			 return scripting::FromValue<_Constructed>::perform(_Constructed(value1.value(), value2.value()), engine).toVariant();
		}
		return v;
	}
	bool supportsExtension ( Extension extension ) const
	{
		return extension == Callable;	
	}
	QString name() const
	{
		return m_name;
	}
};


}