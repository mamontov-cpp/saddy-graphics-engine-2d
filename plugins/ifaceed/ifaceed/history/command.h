/** \file command.h
	

	Describes an abstract command for history
 */
#pragma once
#include <stdlib.h>

namespace core
{
class Editor;
}


namespace history
{
/*! All history consists of those commands. A command must describe one user action
 */
class Command
{
public:
	 /*! Applies changes, described in command
		 \param[in] ob an observer for looking for command
	  */
	 virtual void commit(core::Editor * ob = NULL)=0;
	 /*! Reverts changes, described in command
		 \param[in] ob an observer for looking for command
	  */
	 virtual void rollback(core::Editor * ob = NULL)=0;
     /*! Must be inherited
	  */
	 virtual ~Command();
};

}
