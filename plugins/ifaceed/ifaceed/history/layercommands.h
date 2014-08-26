/** \file layercommands.h
	\author HiddenSeeker
	
	A layer commands are used to prioritize objects when drawing
 */
#include "abstractcommand.h"
#pragma once

class AbstractScreenObject;

/** A new command is a command for adding new object to container
 */
class LayerCommand: public AbstractCommand
{
 private:
	AbstractScreenObject * m_object;
	unsigned int m_old_layer;
	unsigned int m_new_layer;
 public:
    /** Creates a new command
		\param[in] object object to be added
		\param[in] oldlayer old layer
		\param[in] newlayer new layer
	 */
	LayerCommand(AbstractScreenObject * object, unsigned int oldlayer, unsigned int newlayer);
	/** Applies changes, described in command
		\param[in] ob observer for command
	  */
	virtual void commit(core::Editor * ob = NULL);
	/** Reverts changes, described in command
		\param[in] ob observer for command
	  */
	virtual void rollback(core::Editor * ob = NULL);
	/** Destroys a command
	 */
	~LayerCommand();
};