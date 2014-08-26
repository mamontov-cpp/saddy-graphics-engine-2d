/** \file newcommand.h
	\author HiddenSeeker
	
	A new command is a command for adding new object to container
 */
#include "abstractcommand.h"
#include <sadvector.h>
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
		\param[in] ob observer for command
	  */
	virtual void commit(core::CommandChangeObserver * ob = NULL);
	/** Reverts changes, described in command
		\param[in] ob observer for command
	  */
	virtual void rollback(core::CommandChangeObserver * ob = NULL);
	/** Destroys a command
	 */
	~NewCommand();
};


class ScreenClearCommand: public AbstractCommand
{
 private:
	ScreenTemplate * m_container;
	sad::Vector<AbstractScreenObject *> m_objects;
 public:
	/** Creates a new command
		\param[in] container container with objects
	 */
	ScreenClearCommand(ScreenTemplate * container);
	/** Applies changes, described in command
		\param[in] ob observer for command
	  */
	virtual void commit(core::CommandChangeObserver * ob = NULL);
	/** Reverts changes, described in command
		\param[in] ob observer for command
	  */
	virtual void rollback(core::CommandChangeObserver * ob = NULL);
	/** Destroys a command
	 */
	~ScreenClearCommand();
};