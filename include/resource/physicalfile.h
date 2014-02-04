/*! \file resource/physicalfile.h
	\author HiddenSeeker
	
	Contains definition of class PhysicalFile.

 */
#pragma once


namespace resource
{

/*! \class PhysicalFile

 */
class PhysicalFile  
{	
public:	
	/*! This class can be inherited 
	 */
	virtual ~PhysicalFile();
	/*! 
		\param[in] name
	 */
	void File(sad::String name);
	/*! 
		\return
	 */
	bool isAnonymous();
	/*! 
		\return
	 */
	sad::String name();
	/*! 
	 */
	void reload();
	/*! 
		\param[in] name
	 */
	void setName(sad::String name);
	/*! 
		\return
	 */
	sad::Vector<resource::Error*> reload();
	/*! 
		\param[in] filename
	 */
	void save(sad::String filename);
protected: 
	/*! 
	 */
	sad::String m_name;
	/*! 
	 */
	resource::Database* m_db;
	/*! 
	 */
	sad::Hash<sad::String, resource::Resource*> m_resources;

}

}
