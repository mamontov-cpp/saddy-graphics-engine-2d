/*! \file callable.h
	

	Describes a scriptable callable object
 */
#pragma once
#include <QScriptClass>
#include <QScriptContext>

#include <sadpair.h>

#include <db/dbtypename.h>

#include "fromvalue.h"
#include "tovalue.h"


namespace scripting
{

/*! A result for matching a callable signature against arguments
 */
typedef sad::Pair<int, sad::Maybe<QString> > MatchResult;

/*! Returns callable object
 */
class Callable: public QScriptClass
{
public:
	/*! Represents a constructor call for a function with two arguments
		\param[in] e engine
		\param[in] name name of function call
		\param[in] argumentcount count of arguments
	 */
	Callable(QScriptEngine* e, const QString& name, int argumentcount);
	/*! Can be inherited
	 */
	virtual ~Callable();
	/*! Performs a call if need to
		\param[in] extension an extension
		\param[in] argument an arguments
	 */
	QVariant extension( QScriptClass::Extension extension, const QVariant & argument = QVariant() );
	/*! If extension is callable, returns true
		\return true, if extension is callable
	 */
	bool supportsExtension(QScriptClass::Extension extension) const;
	/*! Sets name for callable
		\param[in] name a name for function
	 */
	void setName(const QString& name);
	/*! Returns name for callable
		\return name
	 */
	QString name() const;
	/*! Determines, whether it can be called with this context
		\param[in] ctx context
	 */
	virtual scripting::MatchResult canBeCalled(QScriptContext* ctx) = 0;
	/*! Calls actually a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	virtual QScriptValue call(QScriptContext* ctx, QScriptEngine* engine) = 0;
	/*! Checks argument count
		\param[in] result previous computation result
		\param[in] ctx context
	 */
	void checkArgumentCount(scripting::MatchResult & result, QScriptContext* ctx);
	/*! Checks argument type
		\param[in] result previous computation result
		\param[in] argument number of argument
		\param[in] ctx context
	 */
	template<typename _ArgType>
	void checkArgument(scripting::MatchResult& result, int argument, QScriptContext* ctx)
	{
		if (result._2().exists() == false)
		{
			sad::Maybe<_ArgType> value = scripting::ToValue<_ArgType>::perform(ctx->argument(argument));
			if (value.exists() == false)
			{
				sad::db::TypeName<_ArgType>::init();
				QString tname = sad::db::TypeName<_ArgType>::baseName().c_str();
				QString argstr = QString::number(argument + 1);
				result._2().setValue(QString("must have argument ") + argstr + QString(" of type ") + tname);
			}
			else
			{
				result._1() += 1;
			}
		}
	}
protected:
	/*! A name of call
	 */
	QString m_name;	
	/*! A count of arguments for call
	 */
	int m_argument_count;
};

}
