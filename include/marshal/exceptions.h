/*! \file   exceptions.h
	\author HiddenSeeker

	Here defined an action context, that can return current action string
 */
#include "actioncontext.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#pragma once


namespace serializable
{
	/*! Converts pointer to string
		\param[in] pointer pointer to object 
		\return string string data
	 */
	inline sad::String toString(void * pointer)
	{
		char buffer[25];
		sprintf(buffer,"%lX", reinterpret_cast<unsigned long>(pointer));
		return sad::String(buffer);
	}
	/*!  \class AbstractException

		 Exception, that is produced by properties and other data
	 */
	class AbstractException
	{
	 private:
		      sad::Vector<sad::String>  m_action_context; //!< Action context string
		      sad::String m_string;                       //!< Exception string
	 public:
		      /*! Creates an exception with string, that is consists
				  \param[in] exception_string error string, which identifies invalid action
				  \param[in] context  context, that defines what were performing, when error occured
			   */
		      inline AbstractException(const sad::String &  exception_string, 
				                       ActionContext * context)
		      {
				  m_string = exception_string;
				  if (context != NULL)
				  {
					m_action_context  = context->getActions();
				  }
		      }
			 
			  inline  const sad::String & getInformation() { return m_string;}
			  inline  const sad::Vector<sad::String> & getContext() { return m_action_context; }
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
			  \param[in] context     used action context
		   */	
		  inline InvalidPropertyType( const sad::String & result_type,
			                          const sad::String & real_type,
									  ActionContext * context)
		   : AbstractException(sad::String("Can't cast type \"") + real_type + 
		                       sad::String("\" to type \"") + result_type + 
							   sad::String("\""), context)
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
			  \param[in] context     used action context
		   */	
		  inline InvalidPropertyValue( const sad::String & type,
			                           const sad::String & value,
									   ActionContext * context)
		   : AbstractException(sad::String("Can't asssign property of type \"") + 
		                       type + 
		                       sad::String("\" value \"") + value + 
							   sad::String("\""), context)
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
				\param[in] ptr    pointer name
				\param[in] context context
			 */
		 inline MethodIsNotImplemented( const sad::String & type, const sad::String & method, void * ptr,
									    ActionContext * context)
		 : AbstractException(type+sad::String("(")+toString(ptr)+sad::String(")::")+method+
		                     sad::String(" is not implemented"),context)
		 {
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
				\param[in] context context
			 */
		 inline InvalidParams( int needed , int real,
							   ActionContext * context)
		 : AbstractException(sad::String::number(needed)+ sad::String(" arguments is needed, instead of") + sad::String::number(real),context)
		 {
		 }
	};
}
