#include "marshal/hashbasedserializablecontainer.h"


static const char uid_consonants_table[15][2] = { "b", "c", "d", "f", "g", 
											      "h", "k", "l", "m", "n", 
												  "p", "r", "s", "t", "z"};

static const int  uid_consonants_table_size = 15;


static const char uid_vowels_table[6][2] =     { "a", "e", "i", "o", "u" };


static const int uid_vowels_table_size = 6;

hst::string HashBasedSerializableContainer::random_uid()
{
	int length = rand() % 15 + 5;
	hst::string result;
	for (int i=0;i<length;i++)
	{
		result << uid_consonants_table[ rand() % uid_consonants_table_size];
		result << uid_vowels_table[ rand() % uid_vowels_table_size];
	}
	result << "_";
	result << hst::string::number(rand());
	return result;
}


void HashBasedSerializableContainer::add(SerializableObject * obj)
{
	if (m_reverse_container.contains(obj))
		return;

	hst::string result = this->random_uid();
	while(m_container.contains(result))
		result = this->random_uid();

	m_container.insert(result,obj);
	m_reverse_container.insert(obj,result);

	this->SerializableContainer::add(obj);
}

void HashBasedSerializableContainer::remove(SerializableObject * obj)
{
	if (!m_reverse_container.contains(obj))
		return;
	hst::string uid = m_reverse_container[obj];
	
	m_container.remove(uid);
	m_reverse_container.remove(obj);

	this->SerializableContainer::remove(obj);
}

const hst::string & HashBasedSerializableContainer::uid(SerializableObject * obj) const
{
	if (!m_reverse_container.contains(obj))
		return *(new hst::string());
	return m_reverse_container[obj];
}

SerializableObject * HashBasedSerializableContainer::object(const hst::string & uid) const
{
	if (!m_container.contains(uid))
		return NULL;
	return m_container[uid];

}

HashBasedSerializableContainer::HashBasedSerializableContainer()
{

}

SerializableObject * HashBasedSerializableContainer::begin()
{
	m_iterator = m_container.begin();

	if (m_iterator == m_container.end())
		return NULL;
	return m_iterator.value();
}

SerializableObject * HashBasedSerializableContainer::next()
{
	++m_iterator;

	if (m_iterator == m_container.end())
		return NULL;
	return m_iterator.value();
}

HashBasedSerializableContainer::~HashBasedSerializableContainer()
{

}


