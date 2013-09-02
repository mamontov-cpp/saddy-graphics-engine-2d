#include "objectxmlreader.h"
#include "../objects/screenlabel.h"
#include "../objects/screensprite.h"
#include <QFile>

SerializableObject* ScreenObjectFactory::produce(const hst::string & obj)
{
	if (obj == "ScreenLabel")
		return new ScreenLabel();
	if (obj == "ScreenSprite")
		return new ScreenSprite();
	return NULL;
}


ScreenObjectFactory::~ScreenObjectFactory()
{

}

ObjectXMLReader::ObjectXMLReader (const QString & e, SerializableFactory * factory): serializable::AbstractReader(factory)
{
	this->m_filename = e;
	m_doc = NULL;
}

ObjectXMLReader::~ObjectXMLReader()
{
	delete m_doc;
}


bool ObjectXMLReader::openStream()
{
	QFile  file(m_filename);
	// Cannot open
	if (file.open(QIODevice::ReadOnly | QIODevice::Text) == false)
	{
		QString err = file.errorString();
		return false;
	}
	QDomDocument * d =  new QDomDocument();
	bool r = d->setContent(file.readAll());
	file.close();
	// Cannot parse element
	if (!r)
	{
		delete d;
		return false;
	}
	// No root eleemnt
	m_cur = d->firstChild().toElement();
	if (m_cur.isNull())
	{
		delete d;
		return false;
	}
	m_doc = d;
	m_first = true;
	return true;
}

void ObjectXMLReader::closeStream()
{
	delete m_doc;
	m_doc = NULL;
}

void ObjectXMLReader::findReadingPoint()
{
	if (m_first)
	{
		m_cur = m_cur.firstChild().toElement();
	}
	else 
	{
		m_cur = m_cur.nextSibling().toElement();
	}
	m_first = false;
}


SerializationEntry * ObjectXMLReader::nextEntry()
{
	if (m_cur.isNull() == false)
	{
		SerializationEntry * e = new SerializationEntry();
		e->Name = m_cur.tagName().toStdString().c_str();
		QDomNamedNodeMap map = m_cur.attributes();
		for(uint i = 0; i < map.length(); i++)
		{
			QDomAttr attr = map.item(i).toAttr();
			e->PropertiesName  << attr.name().toStdString().c_str();
			e->PropertiesValue << attr.value().toStdString().c_str();
		}
		return e;
	}
	return NULL;
}

ScreenObjectXMLReader::ScreenObjectXMLReader(const QString & e) : ObjectXMLReader(e, new ScreenObjectFactory())
{

}



ScreenObjectXMLReader::~ScreenObjectXMLReader()
{

}
