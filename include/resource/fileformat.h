/*! \file resource/fileformat.h
	\author HiddenSeeker
	
	Contains definition of class FileFormat.

 */
#pragma once


namespace resource
{

/*! \class FileFormat

 */
class FileFormat  
{	
public:	
	/*! This class can be inherited 
	 */
	virtual ~FileFormat();
	/*! 
		\param[in] files
		\return
	 */
	resource::PhysicalFile* read(resource::Database files);


}

}
