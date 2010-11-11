/*! \file   log.h
    \author HiddenSeeker, FreakyBlast, Falc_ON, Orlangur
	\brief  contains a log class

	Contains a log class, that can easily be output or serialized

*/
#include <sstream>
#include <iostream>
#include <hstring.h>
#pragma once

namespace hst
{

/*! \class log 
	Log class
*/
class log
{
 private:
	     std::ostringstream m_lg;       //!< Basic log
		 static log * m_instance;       //!< Instance
		 log();                         //!< Empty log
		 log(const log & l);            //!< Copy constructor
		 log& operator=(const log & l); //!< Assignment overload
		 static void freeInst();        //!< Free instance
 public:
	     /*! Returns current instance of log
		 */
	     static log * inst();
		 /*! Write something to log
		     \param[in] sth something
			 \return self-reference
		 */
		 template<typename T>
		 log & write(const T & sth);
		 /*! Gets a string of log
		     \return string
		 */
		 static  std::string  str() ;
		 /*! Clears a log
		 */
		 static void clear();
		 /*! Write something to log, with output on stream
		     \param[in] sth something
			 \return self-reference
		 */
		 template<typename T>
		 log & owrite(const T & sth);
		 /*! Save log to file
		     \param[in] fn filename
		 */
		 static void save(const char * fn );
};

}

/*! Fixed overload of method
*/
std::ostringstream & operator<<(std::ostringstream & o, const hst::string & s);



//================ Code goes here =================
template<typename T>
hst::log & hst::log::write(const T & sth)
{
	m_lg<<sth;
	return *this;
}

template<typename T>
hst::log & hst::log::owrite(const T & sth)
{
	m_lg<<sth;
	std::cout<<sth;
	return *this;
}

