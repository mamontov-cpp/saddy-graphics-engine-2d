/*! \file mock2.h
	\author HiddenSeeker

	A mock object for item
 */
#pragma once
#include "object.h"
#include "db/schema/schema.h"

class Mock2: public sad::Object
{
SAD_OBJECT
public:
	Mock2();
	
	int id();
	int id_c() const;
	int& id_r();
	int& id_rc() const;
	const int& id_cr();
	const int& id_crc() const;

	void setId(int i);
	void setIdC(int i) const;
	void setIdR(int & i);
	void setIdRC(int & i) const;
	void setIdCR(const int & i);
	void setIdCRC(const int & i) const;

	int m_id;
};
