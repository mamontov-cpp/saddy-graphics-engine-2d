/*! \file duktape/errorcodes.h
    \author HiddenSeeker

    Defines error codes wrapper for Duktape
 */
#pragma once
#include "../3rdparty/duktape/duktape.h"

namespace sad
{
    
namespace duktape
{

/*! An wrapper for error codes of Duktape 
 */
enum ErrorCodes
{
    SAD_DUK_UNIMPLEMENTED_ERROR = DUK_ERR_UNIMPLEMENTED_ERROR, //!< Unimplemened error
    SAD_DUK_UNSUPPORTED_ERROR = DUK_ERR_UNSUPPORTED_ERROR,     //!< Unsupported error
    SAD_DUK_INTERNAL_ERROR = DUK_ERR_INTERNAL_ERROR,           //!< Internal Error
    SAD_DUK_ALLOC_ERROR = DUK_ERR_ALLOC_ERROR,				   //!< AllocError 
    SAD_DUK_ASSERTION_ERROR = DUK_ERR_ASSERTION_ERROR,         //!< AssertionError 
    SAD_DUK_API_ERROR = DUK_ERR_API_ERROR,					   //!< APIError
    SAD_DUK_UNCAUGHT_ERROR = DUK_ERR_UNCAUGHT_ERROR,           //!< UncaughtError 

    SAD_DUK_E5_ERROR = DUK_ERR_ERROR,                     //!< JS E5 Error 
    SAD_DUK_E5_EVAL_ERROR = DUK_ERR_EVAL_ERROR,           //!< JS E5 EvalError 
    SAD_DUK_E5_RANGE_ERROR = DUK_ERR_RANGE_ERROR,		  //!< JS E5 RangeError 
    SAD_DUK_E5_REFERENCE_ERROR = DUK_ERR_REFERENCE_ERROR, //!< JS E5 ReferenceError
    SAD_DUK_E5_SYNTAX_ERRROR = DUK_ERR_SYNTAX_ERROR,      //!< JS E5 SyntaxError 
    SAD_DUK_E5_TYPE_ERROR = DUK_ERR_TYPE_ERROR,            //!< JS E5 TypeError 
    SAD_DUK_E5_URI_ERROR = DUK_ERR_URI_ERROR               //!< JS E5 URIError 
};

}

}