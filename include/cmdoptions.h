/*! \file cmdoptions.h
	\author HiddenSeeker
	Declares a simple command-line parser for
	arguments
 */
#include "templates/hstring.h"
#include "templates/hlvector.hpp"
#include "templates/hpair.hpp"
#include "marshal/saveloadcallbacks.h"

namespace sad
{

namespace cmd
{

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


/*! Simple option of flag (like -enable-stuff)
 */
class MultipleOption:public OptionType
{
public:
	MultipleOption(const hst::string & name);
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
	 >	   m_multiple_options; //!< Simple options
 public:
	 Parser();
	 ~Parser();
};

}

}

