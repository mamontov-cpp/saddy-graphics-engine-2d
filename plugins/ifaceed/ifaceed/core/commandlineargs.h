/** \file commandlineargs.h
	\author HiddenSeeker

	Describes a command line utility, with options to load level and load config
	as format <levelname> [--ifaceconfig <configname>] [--test TestName]+
 */
#include <QString>
#include <QVector>
/** Command line arguments part name
 */
class CommandLineArgs 
{
 private:
		QString * m_config;  //!< Used config for command line arguments
		QString * m_file;    //!< Filename of loaded file
		QVector<QString> m_unitests; //!< Unit test parameters
 public:
		/** Constructs new arguments and parses them
			\param[in] args argument count
			\param[in] argv arguments apparently
		 */
		CommandLineArgs(int args, const char ** argv);
};