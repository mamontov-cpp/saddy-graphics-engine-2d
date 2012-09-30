/** \file editorhistory.h
	\author HiddenSeeker

	Describes an editor history
 */
#include "abstractcommand.h"
#include "templates/hdeque.hpp"
#pragma once

/** A history of all saved changes
 */
class EditorHistory
{
 private:
	 /** A saved commands
	  */
	 hst::deque<AbstractCommand *> m_commands;
	 /** Current saved command
	  */
	 int  m_current;
 public:
	    /** Constructs empty history
		 */
		EditorHistory();
		/** Frees memory from all commands
		 */
		~EditorHistory();
		/** Add new command to history
			\param[in] c command
		 */
		void add(AbstractCommand * c);
		/** Commits changes, moving to next non-applied command
		 */
		void commit();
		/** Rollbacks changed, moving to previous command
		 */
		void rollback();
};
