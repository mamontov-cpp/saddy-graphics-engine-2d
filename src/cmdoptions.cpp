#include "cmdoptions.h"

using namespace sad::cmd;

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

