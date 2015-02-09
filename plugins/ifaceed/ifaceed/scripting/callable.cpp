#include "callable.h"

Q_DECLARE_METATYPE(QScriptContext*)

scripting::Callable::Callable(QScriptEngine* e, const QString& name, int argumentcount)
: QScriptClass(e), m_name(name), m_argument_count(argumentcount)
{
	
}

scripting::Callable::~Callable()
{
	
}

QVariant scripting::Callable::extension( QScriptClass::Extension extension, const QVariant & argument)
{
	QVariant v;
	if (extension == QScriptClass::Callable) 
	{
		QScriptContext *context = qvariant_cast<QScriptContext*>(argument);
		QScriptEngine *engine = context->engine();
		QScriptValue tmp;
		if (context->isCalledAsConstructor())
		{
			context->throwError(QScriptContext::SyntaxError, m_name + QString(" : cannot be called as constructor"));
			return tmp.toVariant();
		}

		MatchResult result = this->canBeCalled(context);
		if (result._2().exists())
		{
			context->throwError(QScriptContext::SyntaxError, m_name + QString(" : ") + result._2().value());
			return tmp.toVariant();
		}

		QScriptValue value = call(context, engine);
		return value.toVariant();
	}
	return v;
}

bool scripting::Callable::supportsExtension(QScriptClass::Extension extension) const
{
	return extension == QScriptClass::Callable;	
}

void scripting::Callable::setName(const QString& name)
{
	m_name = name;
}

QString scripting::Callable::name() const
{
	return m_name;
}

void scripting::Callable::checkArgumentCount(
	scripting::MatchResult& result, 
	QScriptContext* ctx
)
{
	if (result._2().exists() == false)
	{
		if (ctx->argumentCount() != m_argument_count)
		{
			QString arguments = " arguments";
			if ((m_argument_count != 11) && (m_argument_count == 1 || m_argument_count % 11 == 0))
			{
				arguments = "argument ";
			}
			QString number = QString::number(m_argument_count);
			result._2().setValue(QString("accepts only ") + number + QString(" ") + arguments);
		}
		else
		{
			result._1() += 1;
		}
	}
}
