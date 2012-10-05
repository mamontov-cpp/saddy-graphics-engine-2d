/** \file newcommand.h
	\author HiddenSeeker
	
	A new command is a command for adding new object to container
 */
#include "abstractcommand.h"
#pragma once

class ScreenTemplate;
class AbstractScreenObject;

/** A new command is a command for adding new object to container
 */
class NewCommand: public AbstractCommand
{
 private:
	ScreenTemplate * m_container;
	AbstractScreenObject * m_object;
 public:
    /** Creates a new command
		\param[in] container container with objects
		\param[in] object object to be added
	 */
	NewCommand(ScreenTemplate * container, AbstractScreenObject * object);
	/** Applies changes, described in command
	  */
	virtual void commit();
	/** Reverts changes, described in command
	  */
	virtual void rollback();
	/** Destroys a command
	 */
	~NewCommand();
};