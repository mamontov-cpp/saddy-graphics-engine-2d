#include "factory.h"

unittests::Factory::Factory()
{
}


unittests::Factory::~Factory()
{
	for (QHash<QString, unittests::AbstractFactoryDelegate*>::iterator it = m_delegates.begin();
		it!=m_delegates.end();
		it++) 
	{
		delete it.value();
	}
}

void unittests::Factory::bind(const QString & name, unittests::AbstractFactoryDelegate * test)
{
	if (m_delegates.contains(name))
	{
		delete m_delegates[name];
	}
	m_delegates.insert(name, test);
}

#define LWR(A) owrite(sad::String( A ))
void unittests::Factory::run(const QString & name)
{
	if (m_delegates.contains(name)) 
	{
		QObject * object = m_delegates[name]->create();
		QTest::qExec(object);
		delete object;
	}
	else 
	{
		SL_CRITICAL(QString("Can\'t find test with name \"%1\"").arg(name));
	}
}

void unittests::Factory::runAll() 
{
	for (QHash<QString, unittests::AbstractFactoryDelegate*>::iterator it = m_delegates.begin();
		it!=m_delegates.end();
		it++) 
	{
		SL_SCOPE(it.key().toStdString());
		QObject * object = it.value()->create();
		QTest::qExec(object);
		delete object;
	}
}

unittests::AbstractFactoryDelegate::~AbstractFactoryDelegate()
{
}

