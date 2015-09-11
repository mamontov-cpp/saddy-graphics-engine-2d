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
	/*! Constructs default command
	 */
	Command();
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
	/*! Enables updating UI when committing or rolling back for command
		\param[in] value new value (true if need to enable)
		\return old value
	 */
	bool enableUpdatingUIWhenCommittingOrRollingBack(bool value);
protected:
	/*! Updates UI if set to true
	 */
	bool m_enable_update_ui;
};

}
