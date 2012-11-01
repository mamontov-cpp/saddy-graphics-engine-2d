/** \file commandlineargs.h
	\author HiddenSeeker

	Describes a command line utility, with options to load level and load config
	as format <levelname> [-ifaceconfig <configname>] [-test TestName]+
 */
#include <QString>
#include <QVector>
#include "../editorcore/commandlineoptions.h"
#pragma once
/** Command line arguments part name
 */
class IFaceCmdOptions: public CommandLineOptions 
{
 public:
		/** Constructs new arguments and parses them
			\param[in] argc argument count
			\param[in] argv arguments apparently
		 */
		IFaceCmdOptions();
		/** Parses a command line arguments
			\param[in] argc count of arguments
			\param[in] argv arguments
		 */
	    virtual void parse(int argc, char ** argv);
		/** Frees memory from inner strings
		 */ 
		~IFaceCmdOptions();
		/** Whether running config is specified
			\return whether config as specified
		 */
		bool hasConfig();
		/** Whether loadded level specified
			\return file data
		 */
		bool hasFile();
		/** Returns passed unit tests 
			\return unit tests data
		 */
		QVector<QString> unitTests() const;
		/** Returns config to be loaded
			\return config
		 */	
		QString config() const; 
		/** Sets a new config to be loaded
			\param[in] config config to be working with
		 */
		void setConfig(const QString & config);
		/** Returns inteface file to be loaded
			\return interfaceFile to be loaded
		 */
		const QString & interfaceFile() const;
		/** Sets a new interface file to be loaded
			\param[in] config config to be working with
		 */
		void setInterfaceFile(const QString & file);
};