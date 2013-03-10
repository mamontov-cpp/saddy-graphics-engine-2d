/** \file movecommand.h
	\author HiddenSeeker
	
	A move command is a command for moving object's center to another point
 */
#include "abstractcommand.h"
#include <primitives/hpoint.h>
#pragma once

class AbstractScreenObject;

/** A new command is a command for adding new object to container
 */
class MoveCommand: public AbstractCommand
{
 private:
	AbstractScreenObject * m_object;
	hPointF                m_old_point;
	hPointF                m_new_point;
 public:
    /** Creates a new command
		\param[in] object object to be added
		\param[in] oldp old point
		\param[in] newp new point
	 */
	MoveCommand(AbstractScreenObject * object, const hPointF & oldp, const hPointF & newp);
	/** Applies changes, described in command
		\param[in] c context
	  */
	virtual void commit(ActionContext *c, CommandChangeObserver * ob = NULL);
	/** Reverts changes, described in command
		\param[in] c context
	  */
	virtual void rollback(ActionContext *c, CommandChangeObserver * ob = NULL);
	/** Destroys a command
	 */
	~MoveCommand();
};
