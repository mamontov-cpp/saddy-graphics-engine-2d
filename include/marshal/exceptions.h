/*! \file   exceptions.h
	\author HiddenSeeker

	Here defined an action context, that can return current action string
 */
#include "../sadstring.h"
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#pragma once


namespace serializable
{
	/*!  \class AbstractException

		 Exception, that is produced by properties and other data
	 */
	class AbstractException
	{
	 protected:
		      sad::String m_string;                       //!< Exception string
	 public:
		      /*! Creates an exception with string, that is consists
				  \param[in] exception_string error string, which identifies invalid action
			   */
		      inline AbstractException(const sad::String &  exception_string = "")
		      {
				  m_string = exception_string;
		      }
			 
			  inline  const sad::String & getInformation() { return m_string;}
			  /*! Exception data
			   */
			  virtual ~AbstractException();
	 };

    /*! \class InvalidPropertyType
	    
		This exception must be thrown when user casts data to an incorrect type
	 */
	class InvalidPropertyType: public  AbstractException
	{
	  public:
		  /*! Creates a property type exception
			  \param[in] result_type type, where it should be casted
			  \param[in] real_type   real type data
		   */	
		  inline InvalidPropertyType( const sad::String & result_type,
			                          const sad::String & real_type)
		   : AbstractException(sad::String("Can't cast type \"") + real_type + 
		                       sad::String("\" to type \"") + result_type + 
							   sad::String("\""))
		  {
		  }
	};

    /*! \class InvalidPropertyValue
	    
		This exception must be thrown when user forcefully assigns 
		incorrect string value to property.
	 */
	class InvalidPropertyValue: public AbstractException
	{
	 public:
	      /*! Creates a property type exception
			  \param[in] type        type loading data
			  \param[in] value       used value
		   */	
		  inline InvalidPropertyValue( const sad::String & type,
			                           const sad::String & value)
		   : AbstractException(sad::String("Can't asssign property of type \"") + 
		                       type + 
		                       sad::String("\" value \"") + value + 
							   sad::String("\""))
		  {
		  }
	};

	/*! \class MethodIsNotImplemented

		This exception is thrown when method is not implemented
	 */
	class MethodIsNotImplemented: public AbstractException
	{
	 public:
			/*! Constructs a new exception
				\param[in] type class name
				\param[in] method method name
			 */
		 inline MethodIsNotImplemented( 
			 const sad::String & type, 
			 const sad::String & method
		 )
		 : AbstractException()
		 {
			 std::ostringstream stream;
			 stream << type << "::" << method << " is not implemented";
			 m_string = stream.str();
		 }
	};

	/*! \class InvalidParams

		This exception is thrown when method is not implemented
	 */
	class InvalidParams: public AbstractException
	{
	 public:
			/*! Constructs a new exception
				\param[in] needed class name
				\param[in] real   real amount
			 */
		 inline InvalidParams( int needed , int real)
		 : AbstractException(sad::String::number(needed)+ sad::String(" arguments is needed, instead of") + sad::String::number(real))
		 {
		 }
	};
}
