/** \file editorhistory.h
	\author HiddenSeeker

	Describes an editor history
 */
#include "abstractcommand.h"
#include "sadlinkedlist.h"
#pragma once

/** A history of all saved changes
 */
class EditorHistory
{
 private:
	 /** A saved commands
	  */
	 sad::LinkedList<AbstractCommand *> m_commands;
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
		void commit(core::CommandChangeObserver * ob = NULL);
		/** Rollbacks changed, moving to previous command
		 */
		void rollback(core::CommandChangeObserver * ob = NULL);
		/** Clears a history
		 */
		void clear();
};
