/** \file factory.h
	\author HiddenSeeker

	Describes a factory for creating a new unittests, which can be run automatically from commandline

 */
#include <QTest>
#include <QHash>
#include <log/log.h>
namespace unittests
{
	/** Abstract factory delegate
	 */
	class AbstractFactoryDelegate 
	{
	public:
	/** Creates an object
	  */
	virtual QObject * create()=0;
	virtual ~AbstractFactoryDelegate();
	};
	/** A delegate, for creating a unittests object
	 */
	template<typename T>
	class FactoryDelegate: public AbstractFactoryDelegate 
	{
	public:
		/** Creates a new factory delegate, for running data
		 */
		virtual QObject * create() 
		{
			return new T();
		}
		virtual ~FactoryDelegate() 
		{
		}
	};
	/** A factory, for creating data for vectors
	 */
	class Factory  
	{
	private:
		/** A delegate for running
		  */ 
		QHash<QString, unittests::AbstractFactoryDelegate*> m_delegates;
	public:
		/** A new empty factory is created
		  */
		Factory();
		/** Pushes new delegate. Takes ownership over new delegate, removes other if needed
			\param[in] name name of delegate, which can be used in command line
			\param[in] test delegate
		  */
		void bind(const QString & name, unittests::AbstractFactoryDelegate * test);
		/** Run test of data
			\param[in] name of test, which passed to bind
		  */
		void run(const QString & name);
		/** Runs all tests
		  */
		void runAll();
		/** Destroys all delegates
		  */
		~Factory();
	};
}