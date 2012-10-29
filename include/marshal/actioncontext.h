/*! \file   actioncontext.h
	\author HiddenSeeker

	Here defined an action context, that can return current action string
 */
#include <templates/hstring.h>
#include <templates/hlvector.hpp>
#pragma once

/*! \class ActionContext
	
	Defines a context, which can return current action information	
 */
class ActionContext
{
 protected:
	      hst::vector<hst::string> m_actions_stack;  //!< Stack of performed actions
 public:
	      /*! Get current action string 
			  \return current action string
		   */
	      inline const hst::vector<hst::string> & getActions() const
		  { 
			 return  m_actions_stack;
		  }
	      /*! Sets current action
			  \param[in] str string
		   */
		  virtual void  pushAction(const hst::string & str);
		  /*!  Pops an actions
		   */
		  virtual void popAction();

		  virtual ~ActionContext();  //!< Does nothing
};

/*! A class, which logs entering and leaving actions
 */
class LoggingActionContext: public ActionContext
{
 public:
		  /*! Sets current action
			  \param[in] str string
		   */
		  virtual void  pushAction(const hst::string & str);
		  /*!  Pops an actions
		   */
		  virtual void popAction();
};
