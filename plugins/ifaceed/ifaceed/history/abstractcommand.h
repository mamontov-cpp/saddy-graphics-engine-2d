/** \file abstractcommand.h
	\author HiddenSeeker

	Describes an abstract command for data
 */
#pragma once


class AbstractCommand
{
 public:
	 /** Applies changes, described in command
	  */
	 virtual void commit()=0;
	 /** Reverts changes, described in command
	  */
	 virtual void rollback()=0;
	 /** Descructor
	  */
	 virtual ~AbstractCommand();
};
