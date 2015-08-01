/*! \file dialogue/dialogue.h
	
	
	Describes a dialogue as sequence of phrases
 */
#pragma once
#include "phrase.h"

#include "../sadptrvector.h"

#include "../db/dbobject.h"

namespace sad
{

namespace dialogue
{

/*! Describes a dialogue, which can be used to describe sequences of items
 */
class Dialogue: public sad::db::Object
{
public:
	/*! Constructs an empty dialogue
	 */
	Dialogue();
	/*! Copies phrases from dialogue
		\param[in] d dialogue
	 */
	Dialogue(const sad::dialogue::Dialogue& d);
	/*! Copies phrases from dialogue
		\param[in] d dialogue
		\return *this
	 */
	Dialogue& operator=(const sad::dialogue::Dialogue& d);
	/*! Could be inherited
	 */
	virtual ~Dialogue();
	/*! Clears dialogue, removing all phrases
	 */
	void clear();
	/*! Sets phrases
		\param[in] phrases
	 */
	void setPhrases(const sad::Vector<sad::dialogue::Phrase*>& phrases);
	/*! Returns phrases
		\return phrases for dialogue
	 */
	const sad::Vector<sad::dialogue::Phrase*>& phrases() const;
	/*! Returns phrases
		\return phrases for dialogue
	 */
	sad::Vector<sad::dialogue::Phrase*>& phrases();
	/*! Returns string "sad::dialogue::Dialogue"
		\return name, under which object will be serialized
	 */
	virtual const sad::String& serializableName() const;
	/*! Returns schema for all kinds of dialogue
		\return schema
	 */
	static sad::db::schema::Schema* basicSchema();
	/*! Returns schema for an object
		\return schema
	 */
	virtual sad::db::schema::Schema* schema() const;
protected:
	/*! Copies phrases from another dialogue
		\param[in] d dialogue
	 */
	void copy(const sad::dialogue::Dialogue& d);
	/*! A list of phrases
	 */
	sad::PtrVector<sad::dialogue::Phrase> m_phrases;
};

}

}
