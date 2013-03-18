/** \file abstractcommand.h
	\author HiddenSeeker

	Describes an abstract command for data
 */
#pragma once
#include <marshal/actioncontext.h>
#include <editorcore/commandchangeobserver.h>

class AbstractCommand
{
 public:
	 /** Applies changes, described in command
		 \param[in] c context
	  */
	 virtual void commit(ActionContext *c, CommandChangeObserver * ob = NULL)=0;
	 /** Reverts changes, described in command
		 \param[in] c context
	  */
	 virtual void rollback(ActionContext *c, CommandChangeObserver * ob = NULL)=0;
	 /** Descructor
	  */
	 virtual ~AbstractCommand();
};
