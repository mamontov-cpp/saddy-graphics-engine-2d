/** \file abstractcommand.h
	\author HiddenSeeker

	Describes an abstract command for data
 */
#pragma once
#include <stdlib.h>

namespace core
{
	class Editor;
}


namespace history
{

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
	 /** Descructor
	  */
	 virtual ~Command();
};

}
