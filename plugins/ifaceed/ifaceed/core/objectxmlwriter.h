/*! \file objectxmlwriter.h
	\author HiddenSeeker

	Defines implementation of simple xml writer for data, that uses Qt for writing data
 */
#include <log/log.h>
#include <marshal/abstractwriter.h>
#include <QString>
#include <QDomDocument>
#include <QFile>

/*! Defines a simple serializer for data into XML
 */
class ObjectXMLWriter: public serializable::AbstractWriter 
{
 protected:
			/*! Name of opened file
			 */
			QString m_filename;
			/*! Name of root element
			 */
			QString m_rootname;
			/*! A pointer to an opened file
			 */
			QFile * m_file;
			/*! A document data
			 */
			QDomDocument * m_doc;
			/*! Implement this function to write some entry to an XML
				\param[in] entry entry data
			 */
			virtual void write(SerializationEntry * entry);
			/*! Opens a stream data for writing
			 */ 
			virtual bool openStream();
			/*! Closes a stream data for writing
			 */
			virtual void closeStream();
 public:
		/*! Filename for writer
		 */
		ObjectXMLWriter(const QString & filename, const QString & rootname);
		/*! Writes an objects from container to a stream
			\param[in] container container data
			\param[in] context   context of action
		  */
		virtual bool write(SerializableContainer * container, ActionContext * context);
		/*! A writer for XML 
		 */
		~ObjectXMLWriter();
};