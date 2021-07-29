#include "cli/parser.h"

sad::cli::Parser::Parser()
{

}

sad::cli::Parser::~Parser()
{

}

void sad::cli::Parser::setDefaultOption(const sad::String & s)
{
    m_default_option.setValue(s);
}

const sad::Maybe<sad::String> & sad::cli::Parser::defaultOption() const
{
    return m_default_option;
}

sad::Maybe<bool>  sad::cli::Parser::flag(const sad::String & s) const
{
    sad::Maybe<bool> result;
    if (m_options.contains(s))
    {
        const sad::cli::Option & option = m_options[s];
        if (option.p1() == sad::cli::OptionType::OT_FLAG)
        {	
            result.setValue(option.p2().Flag);
        }
    }
    return result;
}

sad::Maybe<sad::String> sad::cli::Parser::single(const sad::String & s) const
{
    sad::Maybe<sad::String> result;
    if (m_options.contains(s))
    {
        const sad::cli::Option & option = m_options[s];
        if (option.p1() == sad::cli::OptionType::OT_SINGLE)
        {	
            result.setValue(option.p2().Single);
        }
    }
    return result;
}


sad::Maybe<sad::Vector<sad::String> > sad::cli::Parser::vector(const sad::String & s) const
{
    sad::Maybe<sad::Vector<sad::String> > result;
    if (m_options.contains(s))
    {
        const sad::cli::Option & option = m_options[s];
        if (option.p1() == sad::cli::OptionType::OT_VECTOR)
        {	
            result.setValue(option.p2().Vector);
        }
    }
    return result;
}

bool sad::cli::Parser::specified(const sad::String & s) const
{
    bool result  = false;
    if (m_options.contains(s))
    {
        result = m_options[s].p2().Specified;
    }
    return result;
}


void sad::cli::Parser::addFlag(const sad::String & s)
{
    m_options.insert(s, sad::cli::Option(sad::cli::OptionType::OT_FLAG, sad::cli::OptionValue()));
}

void sad::cli::Parser::addSingleValuedOption(const sad::String & s, const sad::String & d)
{
    sad::cli::OptionValue v;
    v.Single = d;
    m_options.insert(s, sad::cli::Option(sad::cli::OptionType::OT_SINGLE, v));
}


void sad::cli::Parser::addVectorValuedOption(const sad::String & s)
{
    m_options.insert(s, sad::cli::Option(sad::cli::OptionType::OT_VECTOR, sad::cli::OptionValue()));
}


void sad::cli::Parser::remove(const sad::String & s)
{
    m_options.remove(s);
}

void sad::cli::Parser::setFlag(const sad::String & name, bool value)
{
    if (m_options.contains(name))
    {
        sad::cli::Option & option = m_options[name];
        if (option.p1() == sad::cli::OptionType::OT_FLAG)
        {	
            option._2().Flag = value;
            option._2().Specified = true;
        }
    }
}

void sad::cli::Parser::setSingleValuedOption(const sad::String & name, const sad::String & value)
{
    if (m_options.contains(name))
    {
        sad::cli::Option & option = m_options[name];
        if (option.p1() == sad::cli::OptionType::OT_SINGLE)
        {	
            option._2().Single = value;
            option._2().Specified = true;
        }
    }
}

void sad::cli::Parser::setMultipleValuedOption(
    const sad::String & name, 
    const sad::Vector<sad::String> & v
)
{
    if (m_options.contains(name))
    {
        sad::cli::Option & option = m_options[name];
        if (option.p1() == sad::cli::OptionType::OT_VECTOR)
        {	
            option._2().Vector = v;
            option._2().Specified = true;
        }
    }
}

void sad::cli::Parser::pushValueForMultipleValuedOption(
    const sad::String & name, 
    const sad::String & v
)
{
    if (m_options.contains(name))
    {
        sad::cli::Option & option = m_options[name];
        if (option.p1() == sad::cli::OptionType::OT_VECTOR)
        {	
            option._2().Vector << v;
        }
    }
}

void sad::cli::Parser::parse(const sad::cli::Args & a)
{
    this->parse(a.count(), a.arguments());
}


void sad::cli::Parser::parse(int argc, const char ** argv)
{
    int state = 0;
    sad::String currentkeyword;
    for(int i = 1; i < argc; i++)
    {
        sad::cli::Token t = createToken(argv[i]);
        if (state == 0)
        {
            if (t.Type == sad::cli::TokenType::TT_VALUE) 
            {
                setDefaultOption(t.Data);
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
                if (t.Type == sad::cli::TokenType::TT_VALUE)
                {
                    state = 0;
                    setSingleValuedOption(currentkeyword, t.Data);
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
                    if (t.Type == sad::cli::TokenType::TT_VALUE)
                    {
                        state = 2;
                        pushValueForMultipleValuedOption(currentkeyword, t.Data);
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

sad::cli::Token sad::cli::Parser::createToken(const sad::String & v)
{
    sad::cli::Token result;
    if (v.length() != 0)
    {
        if (v[0] == '-')
        {
            result.Type = sad::cli::TokenType::TT_KEYWORD;
            result.Data = v.subString(1, v.length() - 1);
        }
        else
        {
            result.Type = sad::cli::TokenType::TT_VALUE;
            result.Data = v;
        }
    }
    else
    {
        result.Type = sad::cli::TokenType::TT_VALUE;
        result.Data = v;
    }
    return result;
}

void  sad::cli::Parser::leaveState(sad::cli::Token & t, sad::String & current_keyword, int & state)
{
    bool isregistered = m_options.contains(t.Data);
    sad::cli::OptionType type = sad::cli::OptionType::OT_FLAG;
    if (isregistered)
    {
        type = m_options[t.Data].p1();
    }
    else
    {
        state = 0; /* ignore unknown keywords */
        return;
    }
    switch(type)
    {
        case sad::cli::OptionType::OT_FLAG:   { state = 0; setFlag(t.Data, true);    break; }
        case sad::cli::OptionType::OT_SINGLE: { state = 1; current_keyword = t.Data;  break; }
        case sad::cli::OptionType::OT_VECTOR: { state = 2; current_keyword = t.Data;  break; }
    };
}
