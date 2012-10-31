/*! \file   exceptions.h
	\author HiddenSeeker

	Here defined an action context, that can return current action string
 */
#include "actioncontext.h"
#include <string.h>
#include <stdio.h>
#pragma once


namespace serializable
{
	/*! Converts pointer to string
		\param[in] pointer pointer to object 
		\return string string data
	 */
	inline hst::string toString(void * pointer)
	{
		char buffer[25];
		sprintf(buffer,"%X",pointer);
		return hst::string(buffer);
	}
	/*!  \class AbstractException

		 Exception, that is produced by properties and other data
	 */
	class AbstractException
	{
	 private:
		      hst::vector<hst::string>  m_action_context; //!< Action context string
		      hst::string m_string;                       //!< Exception string
	 public:
		      /*! Creates an exception with string, that is consists
				  \param[in] exception_string error string, which identifies invalid action
				  \param[in] context  context, that defines what were performing, when error occured
			   */
		      inline AbstractException(const hst::string &  exception_string, 
				                       ActionContext * context)
		      {
				  m_string = exception_string;
				  m_action_context  = context->getActions();
		      }
			 
			  inline  const hst::string & getInformation() { return m_string;}
			  inline  const hst::vector<hst::string> & getContext() { return m_action_context; }
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
		  inline InvalidPropertyType( const hst::string & result_type,
			                          const hst::string & real_type,
									  ActionContext * context)
		   : AbstractException(hst::string("Can't cast type \"") + real_type + 
		                       hst::string("\" to type \"") + result_type + 
							   hst::string("\""), context)
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
		  inline InvalidPropertyValue( const hst::string & type,
			                           const hst::string & value,
									   ActionContext * context)
		   : AbstractException(hst::string("Can't asssign property of type \"") + 
		                       type + 
		                       hst::string("\" value \"") + value + 
							   hst::string("\""), context)
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
		 inline MethodIsNotImplemented( const hst::string & type, const hst::string & method, void * ptr,
									    ActionContext * context)
		 : AbstractException(type+hst::string("(")+toString(ptr)+hst::string(")::")+method+
		                     hst::string(" is not implemented"),context)
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
		 : AbstractException(hst::string::number(needed)+ hst::string(" arguments is needed, instead of") + hst::string::number(real),context)
		 {
		 }
	};
}
