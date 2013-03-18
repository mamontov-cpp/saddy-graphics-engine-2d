/** \file commandlineoptions.h
	\author HiddenSeeker

	CommandLineOptions  as optons, passed to the command line
 */
#include <QHash>
#include <QVariant>
#pragma once
/** Command line options, as passed to command lien
 */
class CommandLineOptions
{
 protected:
    /** Main file, can be specified file
	 */
	QString      *           m_mainfile;
	 /** Options data
	 */
	QHash<QString, QVariant> m_options;
 public:
	/** Command line options is parsed here
		\param[in] argc count of arguments
		\param[in] argv arguments
	 */
	CommandLineOptions();
	/** Parses a command line arguments
		\param[in] argc count of arguments
		\param[in] argv arguments
	 */
	virtual void parse(int argc, char ** argv)=0;
	/** Removes a data from options
	 */
	~CommandLineOptions();
	/** Whether options is specified
		\param[in] name name of option
	 */
	bool optionSpecified(const QString & name) const;
	/** Gets option value
		\param[in] name name of option
	 */
	template<typename T> T option(const QString & name)  const
	{
		if (m_options.contains(name) == false)
			return *(new T());
		return m_options[name].value<T>();
	}
	/** Sets an option
		\param[in] name name data
		\param[in] data data 
	 */
	template<typename T> void setOption(const QString &name, const T & data) 
	{
		m_options.insert(name,QVariant::fromValue(data));
	}
	/** Sets a main file
		\param[in] file new file data
	 */
	void setMainFile(const QString & file);
	/** Has main file?
		\return whether main file is specified
	 */
	bool hasMainFile();
	/** Main file 
		\return main file data
	 */
	const QString & mainFile() const;
};

