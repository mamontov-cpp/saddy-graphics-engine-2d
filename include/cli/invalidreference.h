/*! \file invalidreference.h
	\author HiddenSeeker

	Defines invalid reference to object for a parser
 */
#pragma once

namespace sad
{

namespace cli
{

/*! A class, that creates invalid reference
    to a vector element for a parser
 */
template<typename T>
class InvalidReference
{
 public:
	static inline typename std::vector<T>::reference  create()
	{
		return *(new T());
	}
};

/*! A class, that returns invalid reference
	to a boolean vector element for a parser
 */
template<>
class InvalidReference<bool>
{
 private:
	static std::vector<bool> m_s;
	static bool m_initted;
 public:
	 /*! Creates new reference
	  */
	static inline std::vector<bool>::reference  create()
	{
		if (InvalidReference<bool>::m_initted  == false)
		{
			InvalidReference<bool>::m_s.push_back(false);
			InvalidReference<bool>::m_initted = true;
		}
		return InvalidReference<bool>::m_s[0];
	}
};

}

}
