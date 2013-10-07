/*! \file cmdoptions.h
	\author HiddenSeeker
	Declares a simple command-line parser for
	arguments
 */
#include "sadstring.h"
#include "sadvector.h"
#include "sadpair.h"
#pragma once
namespace sad
{

namespace cmd
{

/*! A class, that creates invalid reference
    to a vector element
 */
template<typename T>
class InvalidReference
{
 public:
	static inline typename std::vector<T>::reference  create()
	{
		return *(new T());
	}
};

/*! A class, that returns invalid reference
	to a boolean vector element
 */
template<>
class InvalidReference<bool>
{
 private:
	static std::vector<bool> m_s;
	static bool m_initted;
 public:
	static inline std::vector<bool>::reference  create()
	{
		if (InvalidReference<bool>::m_initted  == false)
		{
			InvalidReference<bool>::m_s.push_back(false);
			InvalidReference<bool>::m_initted = true;
		}
		return InvalidReference<bool>::m_s[0];
	}
};


/*! Options type is basic for all other types, like Flag, SimpleOption, MultipleOption
 */
class OptionType
{
protected:
	sad::String m_name; //!< Name data
public:
	OptionType();
	/*! A simple constructor
	 */
	OptionType(const sad::String & name);
	/*! A name for option type
	 */
	virtual const sad::String & name() const;

	~OptionType();
};

/*! Simple option of flag (like -enable-stuff)
 */
class Flag:public OptionType
{
public:
	Flag(const sad::String & name);
};


class SimpleOption: public OptionType
{
protected:
	sad::String m_default;
public:
	SimpleOption(const sad::String & name, const sad::String & d = ""): OptionType(name)
	{
		m_default = d;
	}


	const sad::String & defaultValue() 
	{
		return m_default;
	}
};


/*! Multiple option of flag (like enable-stuff)
 */
class MultipleOption:public OptionType
{
public:
	MultipleOption(const sad::String & name);
};

enum ParserTokenType
{
	PTT_KEYWORD, //!< Keyword type
	PTT_OPTION,  //!< An option type
};

class Token
{
 public:
	sad::cmd::ParserTokenType type;
	sad::String data;
};

/** \class Args
	
	Command arguments, passed into editor
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
	/** Constructs a new command arguments
	    \param[in] argc amount of arguments
	    \param[in] argv arguments
	 */
	inline Args(int argc, char ** argv) {
		this->argc = argc;
		this->argv = argv;
	}
	/** Returns argument count
		\return argument count
	 */
	inline int count() const { return this->argc; }

	/** Returns argument count
		\return argument count
	 */
	inline int& mutableCount() { return this->argc; }
	/** Returns arguments
		\param[in] i number of argument
		\return arguments
	 */
	inline const char * argument(int i) const { return this->argv[i]; }
	/** Returns full arguments array
		\return arguments
	 */
	inline char ** fullArgv() { return this->argv; }
};



/*! A parser for parsing some information
 */ 
class Parser
{
 private:
	 sad::String m_default_option; //!< Default parser option
	 sad::Pair<
		 sad::Vector<Flag>,
		 sad::Vector<bool>
	 >		 m_flags; //!< Flags data
	 sad::Pair<
		 sad::Vector<SimpleOption>, 
		 sad::Vector<sad::String>
	 >		m_simple_options; //!< Simple options
	 sad::Pair<
		 sad::Vector<MultipleOption>, 
		 sad::Vector< sad::Vector<sad::String> >
	 >	   m_multiple_options; //!< Multiple options
	
	 /*! Tests, whether options are in container
		 \param[in] name name of options
		 \param[in] v option vector
		 \return whether option is in container
	  */
	 template<
	 typename _PairVector
	 >
	 bool is_in_container(
		const sad::String & name,
		const _PairVector & v)
	 {
		 for (unsigned int i = 0; i < v.p1().count(); i++) 
		 {
			if (v.p1()[i].name() == name)
				return true;
		 }
		 return false;
	 }
	 /*! Throws 0 if option not found
	  */ 
	 template<
	 typename _OptionType,
	 typename _ValueType
	 >
	 typename std::vector<_ValueType>::reference 
	 optionByName(const sad::String & name,
	 sad::Pair< sad::Vector<_OptionType>, sad::Vector<_ValueType> > & v
	 )
	 {
		 for (unsigned int i = 0; i < v._1().count(); i++) 
		 {
			if (v._1()[i].name() == name)
				return v._2()[i];
		 }
		 throw 0;
		 return sad::cmd::InvalidReference<_ValueType>::create();
	 }
	 template<
	 typename _OptionType,
	 typename _ValueType
	 >
	 void setOption(const sad::String & name,
	 sad::Pair< sad::Vector<_OptionType>, sad::Vector<_ValueType> > & v,
	 const _ValueType & val
	 )
	 {
		 for (unsigned int i = 0; i < v.p1().count(); i++) 
		 {
			if (v._1()[i].name() == name)
				v._2()[i] = val;
		 }
	 }
	template<
	 typename _OptionType,
	 typename _ValueType
	 >
	 void addOption(const sad::String & name,
	 sad::Pair< sad::Vector<_OptionType>, sad::Vector<_ValueType> > & v,
	 const _ValueType & val
	 )
	 {
		 if (this->is_in_container(name, v) == false)
		 {
			 v._1() << name;
			 v._2() << val;
		 }	
		 else
		 {
			 setOption(name, v, val);
		 }
	 }
	 /*! Creates a token, parsing it
		 \param[in] v name of token
	  */
	sad::cmd::Token createToken(const sad::String & v);
	/*!  Leaves a state, setting to a new state
	 */
	void leaveState(sad::cmd::Token & t, sad::String & currentkeyword, int & state);
 public:
	 Parser();
	 /*! Checks, whether options is registered
		 \param[in] name name of registered option
		 \return whether option  is registered
	  */
	 bool isRegisteredOption(const sad::String & name);
	 /*! Returns a flag value, throws int  0, if unregistered
		 \param[in] name name of options
		 \return value
	  */
	 bool flag(const sad::String & name);
	 /*! Returns a simple value, throws int  0, if unregistered
		 \param[in] name name of options
		 \return value
	  */
	 sad::String & simple(const sad::String & name);
	 /*! Returns a multiple value, throws int  0, if unregistered
		 \param[in] name name of options
		 \return value
	  */
	 sad::Vector<sad::String> & multiple(const sad::String & name);
	 /*! Sets a flag by name
		 \param[in] name name of option
		 \param[in] v value
	  */
	 void setFlag(const sad::String & name, bool v = false);
	 /*! Sets a simple option by name
		 \param[in] name name of option
		 \param[in] v value
	  */
	 void setSimple(const sad::String & name, const sad::String & v = sad::String());
	 /*! Sets a simple option by name
		 \param[in] name name of option
		 \param[in] v value
	  */
	 void setMultiple(const sad::String & name, const sad::Vector<sad::String> & v = sad::Vector<sad::String>());
	 /*! Adds a flag option (for "-name" option, "name" must be supplied)
		 \param[in] name name of flag option
		 \param[in] v value
	  */
	 void addFlagOption(const sad::String & name, bool v = false);
	 /*! Adds a simple option (for "-name" option, "name" must be supplied) 
		 \param[in] name name of simple option
		 \param[in] v value
	  */
	 void addSimpleOption(const sad::String & name, const sad::String & v = sad::String());
	 /*! Adds a multiple option  (for "-name" option, "name" must be supplied)
		 \param[in] name name of simple option
		 \param[in] v value
	  */
	 void addMultipleOption(const sad::String & name, const sad::Vector<sad::String> & v = sad::Vector<sad::String>());
	 /*! Parses an arguments. If in simple option after keywords
		 leads another keyword, we exit from this state and enter new,
		 same goes for multiple states. Boolean flags, can be simply supplied
		 by entering a flag.
		 The scheme so far is simple:
		 when found unknown option - goes to defaultOption()
		 when found unknown keyword - simply skipped,
		 -flag - sets flag to true
		 -simple-option opt - sets a simple option's value to "opt"
		 -multiple-option opt1 opt2 opt3 - sets a multiple option's value to "opt"
	  */
	 void parse(int argc, const char ** argv);
	 /*! A default option
		 \return default option
	  */
	 const sad::String &  defaultOption() const;
	 ~Parser();
};

}

}

