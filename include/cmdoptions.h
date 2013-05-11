/*! \file cmdoptions.h
	\author HiddenSeeker
	Declares a simple command-line parser for
	arguments
 */
#include "templates/hstring.h"
#include "templates/hlvector.hpp"
#include "templates/hpair.hpp"
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
	static inline std::vector<bool>::reference & create()
	{
		if (m_initted  == false)
		{
			m_s.push_back(false);
			m_initted = true;
		}
		return m_s[0];
	}
};


/*! Options type is basic for all other types, like Flag, SimpleOption, MultipleOption
 */
class OptionType
{
protected:
	hst::string m_name; //!< Name data
public:
	OptionType();
	/*! A simple constructor
	 */
	OptionType(const hst::string & name);
	/*! A name for option type
	 */
	virtual const hst::string & name() const;

	~OptionType();
};

/*! Simple option of flag (like -enable-stuff)
 */
class Flag:public OptionType
{
public:
	Flag(const hst::string & name);
};


class SimpleOption: public OptionType
{
protected:
	hst::string m_default;
public:
	SimpleOption(const hst::string & name, const hst::string & d = ""): OptionType(name)
	{
		m_default = d;
	}


	const hst::string & defaultValue() 
	{
		return m_default;
	}
};


/*! Multiple option of flag (like enable-stuff)
 */
class MultipleOption:public OptionType
{
public:
	MultipleOption(const hst::string & name);
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
	hst::string data;
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
	    \param[in] argc arguments
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
	 hst::string m_default_option; //!< Default parser option
	 hst::pair<
		 hst::vector<Flag>,
		 hst::vector<bool>
	 >		 m_flags; //!< Flags data
	 hst::pair<
		 hst::vector<SimpleOption>, 
		 hst::vector<hst::string>
	 >		m_simple_options; //!< Simple options
	 hst::pair<
		 hst::vector<MultipleOption>, 
		 hst::vector< hst::vector<hst::string> >
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
		const hst::string & name,
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
	 optionByName(const hst::string & name,
	 hst::pair< hst::vector<_OptionType>, hst::vector<_ValueType> > & v
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
	 void setOption(const hst::string & name,
	 hst::pair< hst::vector<_OptionType>, hst::vector<_ValueType> > & v,
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
	 void addOption(const hst::string & name,
	 hst::pair< hst::vector<_OptionType>, hst::vector<_ValueType> > & v,
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
	sad::cmd::Token createToken(const hst::string & v);
	/*!  Leaves a state, setting to a new state
	 */
	void leaveState(sad::cmd::Token & t, hst::string & currentkeyword, int & state);
 public:
	 Parser();
	 /*! Checks, whether options is registered
		 \param[in] name name of registered option
		 \return whether option  is registered
	  */
	 bool isRegisteredOption(const hst::string & name);
	 /*! Returns a flag value, throws int  0, if unregistered
		 \param[in] name name of options
		 \return value
	  */
	 bool flag(const hst::string & name);
	 /*! Returns a simple value, throws int  0, if unregistered
		 \param[in] name name of options
		 \return value
	  */
	 hst::string & simple(const hst::string & name);
	 /*! Returns a multiple value, throws int  0, if unregistered
		 \param[in] name name of options
		 \return value
	  */
	 hst::vector<hst::string> & multiple(const hst::string & name);
	 /*! Sets a flag by name
		 \param[in] name name of option
		 \param[on] v value
	  */
	 void setFlag(const hst::string & name, bool v = false);
	 /*! Sets a simple option by name
		 \param[in] name name of option
		 \param[on] v value
	  */
	 void setSimple(const hst::string & name, const hst::string & v = hst::string());
	 /*! Sets a simple option by name
		 \param[in] name name of option
		 \param[in] v value
	  */
	 void setMultiple(const hst::string & name, const hst::vector<hst::string> & v = hst::vector<hst::string>());
	 /*! Adds a flag option (for "-name" option, "name" must be supplied)
		 \param[in] name name of flag option
		 \param[in] v value
	  */
	 void addFlagOption(const hst::string & name, bool v = false);
	 /*! Adds a simple option (for "-name" option, "name" must be supplied) 
		 \param[in] name name of simple option
		 \param[in] v value
	  */
	 void addSimpleOption(const hst::string & name, const hst::string & v = hst::string());
	 /*! Adds a multiple option  (for "-name" option, "name" must be supplied)
		 \param[in] name name of simple option
		 \param[in] v value
	  */
	 void addMultipleOption(const hst::string & name, const hst::vector<hst::string> & v = hst::vector<hst::string>());
	 /*! Parses an arguments. If in simple option after keywords
		 leads another keyword, we exit from this state and enter new,
		 same goes for multiple states. Boolean flags, can be simply supplied
		 by entering a flag.
		 The scheme so far is simple:
		 <unknown option> - goes to defaultOption()
		 <unknown keyword> - simply skipped,
		 -flag - sets flag to true
		 -simple-option opt - sets a simple option's value to "opt"
		 -multiple-option opt1 opt2 opt3 - sets a multiple option's value to "opt"
	  */
	 void parse(int argc, const char ** argv);
	 /*! A default option
		 \return default option
	  */
	 const hst::string &  defaultOption() const;
	 ~Parser();
};

}

}

