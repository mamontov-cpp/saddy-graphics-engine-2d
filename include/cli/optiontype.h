/*! \file optiontype.h
	\author HiddenSeeker

	Declares a type for options in command-line parser
 */
#pragma once

namespace sad
{

namespace cli
{
/*! An option type, which defines a type for an options, passed to parser
 */
enum OptionType
{
	/*!
	 * A flag is processed as passing single keyword, like
	 * -<flag-name>
	 */
	OT_FLAG = 0;
	/*!
	 * A single-option keyword is processed as passing keyword and a value
	 * after keyword:
	 * -<option-name> <option-value>
	 */
	OT_SINGLE = 1;
	/*!
	 * A multiple-options keyword is processed as passing keyword and several
	 * values: -<option-name> <option-value-1> <option-value-2> <option-value-3>
	 */
	OT_MULTIPLE = 2;
};

}

}
