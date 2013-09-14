#include "cmdoptions.h"

using namespace sad::cmd;


std::vector<bool> sad::cmd::InvalidReference<bool>::m_s;
bool sad::cmd::InvalidReference<bool>::m_initted = false;

sad::cmd::OptionType::OptionType(const hst::string & name)
{
	m_name = name;
}

const hst::string & sad::cmd::OptionType::name() const
{
	return m_name;
}

sad::cmd::OptionType::~OptionType()
{

}

Flag::Flag(const hst::string & name):OptionType(name)
{

}


MultipleOption::MultipleOption(const hst::string & name):OptionType(name)
{

}

OptionType::OptionType()
{
}

Parser::Parser()
{

}

Parser::~Parser()
{

}

bool Parser::isRegisteredOption(const hst::string & name)
{
	return this->is_in_container(name, m_flags)
		|| this->is_in_container(name, m_simple_options)
		|| this->is_in_container(name, m_multiple_options);
}

bool Parser::flag(const hst::string & name)
{
	return this->optionByName(name, m_flags);
}

hst::string & Parser::simple(const hst::string & name)
{
	return this->optionByName(name, m_simple_options);
}

hst::vector<hst::string> & Parser::multiple(const hst::string & name)
{
	return this->optionByName(name, m_multiple_options);
}

void Parser::setFlag(const hst::string & name, bool v)
{
	this->setOption(name, m_flags, v);
}

void Parser::setSimple(const hst::string & name, const hst::string & v)
{
	this->setOption(name, m_simple_options, v);
}

void Parser::setMultiple(const hst::string & name, const hst::vector<hst::string> & v)
{
	this->setOption(name, m_multiple_options, v);
}

void Parser::addFlagOption(const hst::string & name, bool v)
{
	addOption(name, m_flags, v);
}

void Parser::addSimpleOption(const hst::string & name, const hst::string & v)
{
	addOption(name, m_simple_options, v);
}

void Parser::addMultipleOption(const hst::string & name, const hst::vector<hst::string> & v)
{
	addOption(name, m_multiple_options, v);
}

sad::cmd::Token Parser::createToken(const hst::string & v)
{
	sad::cmd::Token result;
	if (v.length() != 0)
	{
	if (v[0] == '-')
	{
		result.type = PTT_KEYWORD;
		result.data = v.subString(1, v.length() - 1);
	}
	else
	{
		result.type = PTT_OPTION;
		result.data = v;
	}
	}
	else
	{
		result.type = PTT_OPTION;
		result.data = v;
	}
	return result;
}

const hst::string &  Parser::defaultOption() const
{
	return m_default_option;
}

void Parser::parse(int argc,const char ** argv)
{
  int state = 0;
  hst::string currentkeyword;
  for(int i = 1; i < argc; i++)
  {
	sad::cmd::Token t = createToken(argv[i]);
	if (state == 0)
	{
		if (t.type == PTT_OPTION) 
		{
			m_default_option = t.data;
		} 
		else
		{
			leaveState(t, currentkeyword, state);
		}
	} 
	else 
	{
	if (state == 1)
	{
		if (t.type == PTT_OPTION)
		{
			state = 0;
			setSimple(currentkeyword, t.data);
		}
		else
		{
			leaveState(t, currentkeyword, state);
		}
	}
	else
	{
	if (state == 2)
	{
		if (t.type == PTT_OPTION)
		{
			state = 2;
			multiple(currentkeyword) << t.data;
		}
		else
		{
			leaveState(t, currentkeyword, state);
		}
	}
	}
	}
  }
}


void sad::cmd::Parser::leaveState(sad::cmd::Token & t, hst::string & currentkeyword, int & state)
{
	if (is_in_container(t.data, m_flags))
	{ setFlag(t.data, true); state = 0; }
	else
	{
		if (is_in_container(t.data, m_simple_options))
		{ state = 1;  currentkeyword = t.data; }
		else
		{
			if (is_in_container(t.data, m_multiple_options))
			{  state = 2; currentkeyword = t.data; }
			else
			{  state = 0; /* ignore unknown keywords */}
		}
	}
}
