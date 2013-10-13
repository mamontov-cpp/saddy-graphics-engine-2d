#include "cmdoptions.h"

using namespace sad::cmd;


std::vector<bool> sad::cmd::InvalidReference<bool>::m_s;
bool sad::cmd::InvalidReference<bool>::m_initted = false;

sad::cmd::OptionType::OptionType(const sad::String & name)
{
	m_name = name;
}

const sad::String & sad::cmd::OptionType::name() const
{
	return m_name;
}

sad::cmd::OptionType::~OptionType()
{

}

Flag::Flag(const sad::String & name):OptionType(name)
{

}


MultipleOption::MultipleOption(const sad::String & name):OptionType(name)
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

bool Parser::isRegisteredOption(const sad::String & name)
{
	return this->is_in_container(name, m_flags)
		|| this->is_in_container(name, m_simple_options)
		|| this->is_in_container(name, m_multiple_options);
}

bool Parser::flag(const sad::String & name)
{
	return this->optionByName(name, m_flags);
}

sad::String & Parser::simple(const sad::String & name)
{
	return this->optionByName(name, m_simple_options);
}

sad::Vector<sad::String> & Parser::multiple(const sad::String & name)
{
	return this->optionByName(name, m_multiple_options);
}

void Parser::setFlag(const sad::String & name, bool v)
{
	this->setOption(name, m_flags, v);
}

void Parser::setSimple(const sad::String & name, const sad::String & v)
{
	this->setOption(name, m_simple_options, v);
}

void Parser::setMultiple(const sad::String & name, const sad::Vector<sad::String> & v)
{
	this->setOption(name, m_multiple_options, v);
}

void Parser::addFlagOption(const sad::String & name, bool v)
{
	addOption(name, m_flags, v);
}

void Parser::addSimpleOption(const sad::String & name, const sad::String & v)
{
	addOption(name, m_simple_options, v);
}

void Parser::addMultipleOption(const sad::String & name, const sad::Vector<sad::String> & v)
{
	addOption(name, m_multiple_options, v);
}

sad::cmd::Token Parser::createToken(const sad::String & v)
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

const sad::String &  Parser::defaultOption() const
{
	return m_default_option;
}

void Parser::parse(int argc,const char ** argv)
{
  int state = 0;
  sad::String currentkeyword;
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


void sad::cmd::Parser::leaveState(sad::cmd::Token & t, sad::String & currentkeyword, int & state)
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
