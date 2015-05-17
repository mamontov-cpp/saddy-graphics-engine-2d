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

scripting::MatchResult scripting::MultiMethod::canBeCalled(QScriptContext* ctx)
{
	scripting::MatchResult result;
	result._1() = 0;
	for(size_t i = 0; i < m_methods.size(); i++)
	{
		scripting::MatchResult tmpresult = m_methods[i]->canBeCalled(ctx);
		if (tmpresult._2().exists() == false)
		{
			return tmpresult;
		}
		if (result._2().exists() == false || result.p1() < tmpresult.p1())
		{
			result = tmpresult;
		}
	}
	return result;
}

QScriptValue scripting::MultiMethod::call(QScriptContext* ctx, QScriptEngine* engine)
{
	QScriptValue result;
	scripting::MatchResult tmp;
	bool found = false;
	for(size_t i = 0; i < m_methods.size() && !found; i++)
	{
		tmp = m_methods[i]->canBeCalled(ctx);
		if (tmp._2().exists() == false)
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
