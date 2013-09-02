/** \file movecommand.h
	\author HiddenSeeker
	
	A move command is a command for moving object's center to another point
 */
#include "abstractcommand.h"
#include <primitives/hpoint.h>
#include <primitives/hrect.h>
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
		\param[in] ob observer for command
	  */
	virtual void commit(ActionContext *c, CommandChangeObserver * ob = NULL);
	/** Reverts changes, described in command
		\param[in] c context
		\param[in] ob observer for command
	  */
	virtual void rollback(ActionContext *c, CommandChangeObserver * ob = NULL);
	/** Destroys a command
	 */
	~MoveCommand();
};


class ResizeCommand: public AbstractCommand
{
 private:
	AbstractScreenObject * m_object;
	hRectF                m_old_rect;
	hRectF                m_new_rect;
	float				  m_angle;  //!< New angle
 public:
    /** Creates a new command
		\param[in] object object to be added
		\param[in] or old rect
		\param[in] nr new rect
		\param[in] a   angle
	 */
	ResizeCommand(AbstractScreenObject * object, const hRectF & or, const hRectF & nr, float a);
	/** Applies changes, described in command
		\param[in] c context
		\param[in] ob observer for command
	  */
	virtual void commit(ActionContext *c, CommandChangeObserver * ob = NULL);
	/** Reverts changes, described in command
		\param[in] c context
		\param[in] ob observer for command
	  */
	virtual void rollback(ActionContext *c, CommandChangeObserver * ob = NULL);
	/** Destroys a command
	 */
	~ResizeCommand();
};

class MakeBackgroundCommand: public AbstractCommand
{
 private:
	AbstractScreenObject * m_o;
	unsigned int m_layer;
	hRectF m_rect;
	float  m_angle;
 public:
    /** Creates a new command
		\param[in] object object to be added
	 */
	MakeBackgroundCommand(AbstractScreenObject * object);
	/** Applies changes, described in command
		\param[in] c context
		\param[in] ob observer for command
	  */
	virtual void commit(ActionContext *c, CommandChangeObserver * ob = NULL);
	/** Reverts changes, described in command
		\param[in] c context
		\param[in] ob observer for command
	  */
	virtual void rollback(ActionContext *c, CommandChangeObserver * ob = NULL);
	/** Destroys a command
	 */
	~MakeBackgroundCommand();
};