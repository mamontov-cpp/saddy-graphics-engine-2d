/** \file movecommand.h
	\author HiddenSeeker
	
	A move command is a command for moving object's center to another point
 */
#include "abstractcommand.h"
#include <sadpoint.h>
#include <sadrect.h>
#pragma once

class AbstractScreenObject;

/** A new command is a command for adding new object to container
 */
class MoveCommand: public AbstractCommand
{
 private:
	AbstractScreenObject * m_object;
	sad::Point2D                m_old_point;
	sad::Point2D                m_new_point;
 public:
    /** Creates a new command
		\param[in] object object to be added
		\param[in] oldp old point
		\param[in] newp new point
	 */
	MoveCommand(AbstractScreenObject * object, const sad::Point2D & oldp, const sad::Point2D & newp);
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
	~MoveCommand();
};


class ResizeCommand: public AbstractCommand
{
 private:
	AbstractScreenObject * m_object;
	sad::Rect2D                m_old_rect;
	sad::Rect2D                m_new_rect;
	float				  m_angle;  //!< New angle
 public:
    /** Creates a new command
        \param[in] __object object to be added
        \param[in] oldrectangle old rect
		\param[in] nr new rect
		\param[in] a   angle
	 */
    ResizeCommand(AbstractScreenObject * __object, const sad::Rect2D & oldrectangle, const sad::Rect2D & nr, float a);
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
	~ResizeCommand();
};

class MakeBackgroundCommand: public AbstractCommand
{
 private:
	AbstractScreenObject * m_o;
	unsigned int m_layer;
	sad::Rect2D m_rect;
	float  m_angle;
 public:
    /** Creates a new command
		\param[in] object object to be added
	 */
	MakeBackgroundCommand(AbstractScreenObject * object);
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
	~MakeBackgroundCommand();
};
