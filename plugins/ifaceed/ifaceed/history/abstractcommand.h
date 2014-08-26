/** \file abstractcommand.h
	\author HiddenSeeker

	Describes an abstract command for data
 */
#pragma once
#include <core/commandchangeobserver.h>

class AbstractCommand
{
 public:
	 /** Applies changes, described in command
		 \param[in] ob an observer for looking for command
	  */
	 virtual void commit(core::CommandChangeObserver * ob = NULL)=0;
	 /** Reverts changes, described in command
		 \param[in] ob an observer for looking for command
	  */
	 virtual void rollback(core::CommandChangeObserver * ob = NULL)=0;
	 /** Descructor
	  */
	 virtual ~AbstractCommand();
};
