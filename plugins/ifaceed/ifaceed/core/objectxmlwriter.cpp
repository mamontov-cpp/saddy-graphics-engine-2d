#include "objectxmlwriter.h"
#include <QDomElement>

ObjectXMLWriter::ObjectXMLWriter(const QString & filename, const QString & rootname)
{
	m_filename = filename;
	m_rootname = rootname;
	m_file = NULL;
	m_doc = NULL;
	
}

ObjectXMLWriter::~ObjectXMLWriter()
{
	delete m_file;
	delete m_doc;
}

bool ObjectXMLWriter::openStream()
{
	m_file = new QFile(m_filename);
	if (m_file->open(QIODevice::WriteOnly | QIODevice::Text) == false)
	{
		delete m_file;
		m_file = NULL;
		return false;
	}
	m_doc = new QDomDocument();
	m_doc->appendChild(m_doc->createElement(m_rootname));
	return true;
}

void ObjectXMLWriter::closeStream()
{
	m_file->write(m_doc->toString(1).toStdString().c_str());
	m_file->close();
	delete m_doc;
	delete m_file;
	m_doc = NULL;
	m_file = NULL;
}

void ObjectXMLWriter::write(SerializationEntry * entry)
{
	QDomElement e = m_doc->createElement(entry->Name.data());
    for(unsigned int i = 0; i < entry->PropertiesName.count(); i++)
	{
		e.setAttribute(entry->PropertiesName[i].data(), entry->PropertiesValue[i].data());
	}
	m_doc->firstChild().toElement().appendChild(e);
}


bool ObjectXMLWriter::write(SerializableContainer * container)
{
	return this->serializable::AbstractWriter::write(container);
}
