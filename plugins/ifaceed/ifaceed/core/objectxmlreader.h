/*! \file objectxmlreader.h
	\author HiddenSeeker

	Defines implementation of object xml reader for data, that uses Qt for writing data
 */
#include <log/log.h>
#include <marshal/abstractreader.h>
#include <QString>
#include <QDomDocument>
#include <QFile>
#include <QDomElement>
/*! A factory used for serializable objects creation
 */
class ScreenObjectFactory: public SerializableFactory
{
 public:
	     /*! Produces an object by name
			 \param[in] obj object
			 \return data
		  */
		 virtual SerializableObject* produce(const hst::string & obj) ;
		 /*! Does nothing
		  */
		 ~ScreenObjectFactory();
};

/*! A reader for deserializing XML data
 */
class ObjectXMLReader: public serializable::AbstractReader
{
 protected:
	/*! Filename data
	 */
	QString m_filename;
	/*! Document
	 */
	QDomDocument * m_doc;
	/*! Current element
	 */
	QDomElement m_cur;
	/*! Determines, whether we first time calling findReadingPoint()
	 */
	bool m_first;
	/*! Finds next point, where document can be read
	 */
	virtual void findReadingPoint();
	/*! Implement this function to read some entry.
		\param[in] entry entry data
	 */
	virtual SerializationEntry * nextEntry();
	 /*! Opens a stream data for writing
	  */ 
	virtual bool openStream();
	 /*! Closes a stream data for writing
	  */
	virtual void closeStream();
 public:
	 /*! Sets a factory for reader
		 \param[in] e      filename
		 \param[in] factory factory data
	  */
     ObjectXMLReader (const QString & e,SerializableFactory * factory);
	 /*! An XML reader data
	  */
	 ~ObjectXMLReader();
};

/*! A reader for screen objects
 */
class ScreenObjectXMLReader: public ObjectXMLReader
{
 protected:
 public:
		/*! Constructs new XML reader
		 */
		ScreenObjectXMLReader(const QString & e);
		/*! An XML reader data
		 */
		~ScreenObjectXMLReader();
};
