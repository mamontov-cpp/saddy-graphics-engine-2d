#include "multimethod.h"

scripting::MultiMethod::MultiMethod(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 0)
{
	
}

scripting::MultiMethod::~MultiMethod()
{
	for(size_t i = 0; i < m_methods.size(); i++)
	{
		delete m_methods[i];
	}
}

sad::Maybe<QString> scripting::MultiMethod::canBeCalled(QScriptContext* ctx)
{
	sad::Maybe<QString> result;
	for(size_t i = 0; i < m_methods.size(); i++)
	{
		result = m_methods[i]->canBeCalled(ctx);
		if (result.exists() == false)
		{
			return result;
		}
	}
	return result;
}

QScriptValue scripting::MultiMethod::call(QScriptContext* ctx, QScriptEngine* engine)
{
	QScriptValue result;
	sad::Maybe<QString> tmp;
	bool found = false;
	for(size_t i = 0; i < m_methods.size() && !found; i++)
	{
		tmp = m_methods[i]->canBeCalled(ctx);
		if (tmp.exists() == false)
		{
			found = true;
			result = m_methods[i]->call(ctx, engine);
		}
	}
	return result;
}

void  scripting::MultiMethod::add(scripting::Callable* c)
{
	c->setName(m_name);
	m_methods << c;
}
