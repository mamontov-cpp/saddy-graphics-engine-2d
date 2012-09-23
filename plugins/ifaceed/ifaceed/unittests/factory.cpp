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

#define LWR(A) owrite(hst::string( A ))
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
		hst::log::inst()-> LWR("Can't find test with name "). LWR (name.toStdString().c_str()). LWR("\n");
	}
}

void unittests::Factory::runAll() 
{
	for (QHash<QString, unittests::AbstractFactoryDelegate*>::iterator it = m_delegates.begin();
		it!=m_delegates.end();
		it++) 
	{
		hst::log::inst()-> LWR("Entering test "). LWR (it.key().toStdString().c_str()). LWR("\n");
		QObject * object = it.value()->create();
		QTest::qExec(object);
		delete object;
	}
}