#include "commandlineoptions.h"

bool CommandLineOptions::optionSpecified(const QString & name) const
{
	return m_options.contains(name);
}

CommandLineOptions::CommandLineOptions()
{
	m_mainfile = NULL;
}


CommandLineOptions::~CommandLineOptions()
{
	delete m_mainfile;
}

bool CommandLineOptions::hasMainFile()
{
	return m_mainfile!=NULL;
}

void CommandLineOptions::setMainFile(const QString & file)
{
	delete m_mainfile;
	m_mainfile = new QString(file); 
}

const QString & CommandLineOptions::mainFile() const
{
	 if (m_mainfile) return *m_mainfile;
	 return *(new QString());
}
