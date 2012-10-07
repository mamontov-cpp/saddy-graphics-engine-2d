#include "ifacecmdoptions.h"

IFaceCmdOptions::~IFaceCmdOptions()
{

}

bool IFaceCmdOptions::hasConfig()
{
	return this->optionSpecified("ifaceconfig");
}

bool IFaceCmdOptions::hasFile()
{
	return this->hasMainFile();
}

QVector<QString> IFaceCmdOptions::unitTests() const 
{
	return this->option< QVector<QString> >("test");
}

QString IFaceCmdOptions::config() const
{
	return this->option<QString>("ifaceconfig");
}

const QString & IFaceCmdOptions::interfaceFile() const
{
	return this->mainFile();
}

void IFaceCmdOptions::setConfig(const QString & config)
{
	this->setOption("ifaceconfig", config);
}

void IFaceCmdOptions::setInterfaceFile(const QString & file)
{
	this->setMainFile(file);
}
IFaceCmdOptions::IFaceCmdOptions()
{

}

Q_DECLARE_METATYPE(QVector<QString>);

void IFaceCmdOptions::parse(int argc, char ** argv)
{
	class 
	{
	 public:
		bool keyword;           // A state when config keyword specified
		QString keywordName;    // A name of keyword 
	} state;
	
	QVector<QString> unittests;
	state.keyword = false;
	for(int i=1;i<argc;i++)
	{
		QString data = argv[i];
		// Handle keywords
		bool assignedKeyword = false;
		if (data.length() > 0) 
		{
			if (data[0] == '-') 
			{
				state.keyword = true;
				state.keywordName = data;
				assignedKeyword = true;
			}
		}
		// Handle if data specified
		if (!assignedKeyword) 
		{
			if (state.keyword)
			{
				if (state.keywordName == "-ifaceconfig")
					this->setConfig(data);
				if (state.keywordName == "-test")
					unittests << data;
				state.keyword = false;
			}
			else
			{
				this->setInterfaceFile(data);
			}
		}
	}
	this->setOption("test", unittests);
}

