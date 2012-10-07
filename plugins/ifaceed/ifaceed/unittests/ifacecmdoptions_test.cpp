#include "ifacecmdoptions_test.h"
#include "../core/ifacecmdoptions.h"

class CCStyleOptions
{
 public:
		  int argc;
		  char ** argv;
		  CCStyleOptions(const QVector<QString> & args);
		  ~CCStyleOptions();
};

CCStyleOptions::~CCStyleOptions()
{
	for (int i=0;i<argc;i++)
		delete[] argv[i];
	delete[] argv;
}

CCStyleOptions::CCStyleOptions(const QVector<QString> & args)
{
	if (args.size()==0) {
		argc = 0;
		argv = 0;
		return;
	}
	argc = args.size();
	argv = new char*[argc];
	for(int i=0;i<args.size();i++)
	{
		argv[i] = new char[args[i].length()+1];
		argv[i][args[i].length()] = 0;
		strcpy(argv[i],args[i].toStdString().c_str());
	}
}



void IFaceCmdOptionsTest::testMainFile()
{
	QVector<QString> a;
	a << "test.exe" << "test_iface.xml";
	CCStyleOptions opt(a);
	IFaceCmdOptions parser;
	parser.parse(opt.argc, opt.argv);
	QCOMPARE(parser.hasFile(),true);
	QCOMPARE(parser.interfaceFile(),QString("test_iface.xml"));
	QCOMPARE(parser.hasConfig(),false);
}

void IFaceCmdOptionsTest::testConfigOnly()
{
	QVector<QString> a;
	a << "test.exe" << "-ifaceconfig" << "config.xml";
	CCStyleOptions opt(a);
	IFaceCmdOptions parser;
	parser.parse(opt.argc, opt.argv);
	QCOMPARE(parser.hasFile(),false);
	QCOMPARE(parser.hasConfig(),true);
	QCOMPARE(parser.config(), QString("config.xml"));
}

void IFaceCmdOptionsTest::testTests()
{
	QVector<QString> a;
	a << "test.exe" << "-test" << "Class1" << "-test" << "Class2" << "-some-qt-stuff" << "garbage";
	CCStyleOptions opt(a);
	IFaceCmdOptions parser;
	parser.parse(opt.argc, opt.argv);
	QCOMPARE(parser.hasFile(),false);
	QCOMPARE(parser.hasConfig(),false);
	QVector<QString> data = parser.unitTests();
	QCOMPARE(data.size(),2);
	QCOMPARE(data[0],QString("Class1"));
	QCOMPARE(data[1],QString("Class2"));
}

void IFaceCmdOptionsTest::testCommon()
{
	QVector<QString> a;
	a << "test.exe" << "-style" << "windows" 
		            << "test.xml"
					<< "-ifaceconfig" << "config.xml"
		            << "-test" << "Class1" << "-test" << "Class2" 
					<< "-some-qt-stuff" << "garbage";
	CCStyleOptions opt(a);
	IFaceCmdOptions parser;
	parser.parse(opt.argc, opt.argv);

	QCOMPARE(parser.hasFile(),true);
	QCOMPARE(parser.interfaceFile(),QString("test.xml"));

	QCOMPARE(parser.hasConfig(),true);
	QCOMPARE(parser.config(),QString("config.xml"));
	
	QVector<QString> data = parser.unitTests();
	QCOMPARE(data.size(),2);
	QCOMPARE(data[0],QString("Class1"));
	QCOMPARE(data[1],QString("Class2"));
}