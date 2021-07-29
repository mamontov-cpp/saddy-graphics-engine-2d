/*! \file parser.h
    

    Defines a parser for command-line arguments
 */
#pragma once
#include "args.h"
#include "optiontype.h"
#include "optionvalue.h"
#include "token.h"

#include "../sadhash.h"
#include "../sadpair.h"
#include "../maybe.h"

namespace sad
{

namespace cli
{

/*! Defines a command-line  option 
 */
typedef sad::Pair<sad::cli::OptionType, sad::cli::OptionValue> Option;

/*! Defines a simple command-line option parser
 */
class Parser
{
public:
    /*! Creates default empty command-line argument parser
     */
    Parser();
    /*! Destroys all data from command-line argument parser
     */
    virtual ~Parser();
    /*! Sets default options to a value. A default options is an option, which does not belong to any options,
        defined by keywords. 
        It can be file name or any kind of default options.
        \param[in] s value of an option
     */
    virtual void setDefaultOption(const sad::String & s);
    /*! Returns a default option by a value. A default option is an option, 
        which does not belong to any options defined by keywords. 
        \return default option value if set
     */
    virtual const sad::Maybe<sad::String> & defaultOption() const;
    /*! Returns flag value. A result could be empty, if option, named as such is unregistered
        \param[in] s an option name
        \return flag value (can be not set if not registered)
     */
    virtual sad::Maybe<bool> flag(const sad::String & s) const;
    /*! Returns simple option value. A result could be empty, if option, named as such is unregistered
        \param[in] s an option name
        \return simple option value (can be not set if not registered)
     */
    virtual sad::Maybe<sad::String> single(const sad::String & s) const;
    /*! Returns multiple options value. A result could be empty, if option, named as such is unregistered
        \param[in] s an option name
        \return multiple option value (can be not set if not registered)
     */
    virtual sad::Maybe<sad::Vector<sad::String> > vector(const sad::String & s) const;
    /*! Returns true, if command line option is specified
        \param[in] s a string
        \return whether command line option  is specified by user
     */
    virtual bool specified(const sad::String & s) const;
    /*! Adds new flag option. A default value for flag is false
        \param[in] s name of flag
     */
    virtual void addFlag(const sad::String & s);
    /*! Adds new simple option value.
        \param[in] s name of option
        \param[in] d a default value for simple option
     */
    virtual void addSingleValuedOption(const sad::String & s, const sad::String & d = "");
    /*! Adds option, which could take several values. A default value for this option is
        empty vector
        \param[in] s name of option
     */
    virtual void addVectorValuedOption(const sad::String & s);
    /*! Removes an option
        \param[in] s option name
     */
    virtual void remove(const sad::String & s);
    /*! Sets flag value to a name
        \param[in] name name of option
        \param[in] value a value for a flag
     */
    virtual void setFlag(const sad::String & name, bool value);
    /*! Sets single string value to a name
        \param[in] name name of option
        \param[in] value a value for an option
     */
    virtual void setSingleValuedOption(const sad::String & name, const sad::String & value);
    /*! Sets multiple valued options to a name
        \param[in] name name of option
        \param[in] v value a value for an option
     */
    virtual void setMultipleValuedOption(const sad::String & name, const sad::Vector<sad::String> & v);
    /*! Pushes value for multiple values option to a vector
        \param[in] name name for option
        \param[in] v value for a string
     */
    virtual void pushValueForMultipleValuedOption(const sad::String & name, const sad::String & v);
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
         \param[in] a arguments
      */
    virtual void parse(const sad::cli::Args & a);
    /*! Parses an arguments
        \param[in] argc argument count
        \param[in] argv arguments
     */
    virtual void parse(int argc, const char ** argv);
protected:
    /*! Creates a token by looking up to an option container
        \param[in] v text of token
      */
    virtual sad::cli::Token createToken(const sad::String & v);
    /*!  Leaves a state, setting values if needed and proceeding to next state
         \param[in,out] t token
         \param[in,out] current_keyword keyword value by state
         \param[in,out] state state value
     */
    virtual void leaveState(sad::cli::Token & t, sad::String & current_keyword, int & state);
    /*! Default option is an option, which does not belong to any options, defined by keywords. 
        It can be file name or any kind of default options.
     */ 
    sad::Maybe<sad::String>         m_default_option;
    /*! A map of options in parser
     */
    sad::Hash<sad::String, Option>  m_options;
};

}

}
