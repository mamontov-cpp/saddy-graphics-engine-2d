/** \file deletecommand.h
	\author HiddenSeeker
	
	A delete command is a command for adding new object to container.
	Implemented as exact reversal of new command.
 */
#include "newcommand.h"
#pragma once

class ScreenTemplate;
class AbstractScreenObject;

/** A delete command is a command for adding new object to container.
 */
class DeleteCommand: public AbstractCommand
{
 private:
	NewCommand * m_cmd;
 public:
    /** Creates a new command
		\param[in] container container with objects
		\param[in] object object to be added
	 */
	DeleteCommand(ScreenTemplate * container, AbstractScreenObject * object);
	/** Applies changes, described in command
		\param[in] c context
	  */
	virtual void commit(ActionContext *c);
	/** Reverts changes, described in command
		\param[in] c context
	  */
	virtual void rollback(ActionContext *c);
	/** Destroys a command
	 */
	~DeleteCommand();
};