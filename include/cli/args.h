/*! \file args.h
	\author HiddenSeeker

	Defines an arguments, passed to parser
 */
#pragma once

namespace sad
{

namespace cli
{

/** \class Args
	
	Command-line arguments, passed to parser
 */
class Args 
{
 private:
	/** Argument count
	  */
	int argc;
	/** Passed arguments
	  */
	char ** argv;
 public:
	/** Constructs a new command-line arguments
	    \param[in] _argc amount of arguments
	    \param[in] _argv arguments
	 */
	inline Args(int _argc, char ** _argv): argc(_argc), argv(_argv)  
	{

	}
	/** Returns argument count
		\return argument count
	 */
	inline int count() const 
	{ 
		return this->argc; 
	}
	/** Returns argument count
		\return argument count
	 */
	inline int& count() 
	{ 
		return this->argc; 
	}
	/** Returns arguments
		\param[in] i number of argument
		\return arguments
	 */
	inline const char * argument(int i) const 
	{ 
		return this->argv[i]; 
	}
	/** Returns full arguments array
		\return arguments
	 */
	inline char ** arguments()
	{ 
		return this->argv; 
	}
	/** Returns full arguments array
		\return arguments
	 */
	inline const char ** arguments() const
	{ 
		return const_cast<const char **>(this->argv); 
	}
};

}

}
