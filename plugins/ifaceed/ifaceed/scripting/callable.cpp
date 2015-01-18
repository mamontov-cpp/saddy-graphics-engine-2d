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
		// TODO: Actually do stuff
		QScriptContext *context = qvariant_cast<QScriptContext*>(argument);
		QScriptEngine *engine = context->engine();
		QScriptValue tmp;

		sad::Maybe<QString> result = this->canBeCalled(context);
		if (result.exists())
		{
			context->throwError(QScriptContext::SyntaxError, m_name + QString(" : ") + result.value());
			return tmp.toVariant();
		}

		return call(context, engine).toVariant();
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

void scripting::Callable::checkArgumentCount(sad::Maybe<QString> & result, QScriptContext* ctx)
{
	if (result.exists() == false)
	{
		if (ctx->argumentCount() != m_argument_count)
		{
			QString arguments = " arguments";
			if ((m_argument_count != 11) && (m_argument_count == 1 || m_argument_count % 11 == 0))
			{
				arguments = "argument ";
			}
			QString number = QString::number(m_argument_count);
			result.setValue(QString("accepts only ") + number + arguments);
		}
	}
}
