/** \file abstractcommand.h
	\author HiddenSeeker

	Describes an abstract command for data
 */
#pragma once
#include <editorcore/commandchangeobserver.h>

class AbstractCommand
{
 public:
	 /** Applies changes, described in command
		 \param[in] ob an observer for looking for command
	  */
	 virtual void commit(CommandChangeObserver * ob = NULL)=0;
	 /** Reverts changes, described in command
		 \param[in] ob an observer for looking for command
	  */
	 virtual void rollback(CommandChangeObserver * ob = NULL)=0;
	 /** Descructor
	  */
	 virtual ~AbstractCommand();
};
