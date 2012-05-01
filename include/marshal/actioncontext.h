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
 private:
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
		  inline void  pushAction(const hst::string & str)
		  {
			  m_actions_stack<<str;
		  }
		  /*!  Pops an actions
		   */
		  inline void popAction()
		  {
			  if (m_actions_stack.count()!=0)
				  m_actions_stack.removeAt(m_actions_stack.count()-1);
		  }

		  virtual ~ActionContext();  //!< Does nothing
};
