#include "xmlconfigloader.h"
#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QStringList>

#define SPRITEINDEX_TAG "index"
#define TEXTURENAME_TAG "texture"
#define TEXTURERECT_TAG "texrect"
#define SIZE_TAG        "size"
#define TRANSPARENCY_TAG    "transparent"

XMLConfigLoader::~XMLConfigLoader()
{
}

QVector<QDomElement> XMLConfigLoader::getElements(const QDomNode & doc)
{
	QVector<QDomElement> result;

	QDomNode node=doc.firstChild();
	while(!node.isNull())
	{
		QDomElement element=node.toElement();
		if (element.isNull() == false)
			result<<element;

		node=node.nextSibling();
	}

	return result;
}

bool XMLConfigLoader::load(Sprite2DTemplateContainer & container)
{
 //Open file
 QFile file(m_filename);
 if (file.open(QIODevice::ReadOnly)==false)
 {
	 return false;
 }

 //Read file
 QTextStream stream(&file);
 QString contents=stream.readAll();
 file.close();

 //Parse file
 QDomDocument doc;
 if (doc.setContent(contents)==false)
 {
	 return false;
 }

 //Scan document elements
 QVector<QDomElement> elements=getElements(getElements(doc)[0]);
 
 //Process every element
 bool ok=true;   //!< Flag, that indicates whether element processing was successfull
 for(int i=0;i<elements.size();i++)
 {
	XMLConfigEntry entry;
	ok=ok && loadEntry(entry,elements[i]);
	insertTemplate(entry.spriteGroup,entry.index,entry.spriteTemplate,container);
 }
 return ok;
}


bool XMLConfigLoader::loadEntry(XMLConfigEntry & entry, const QDomElement & element)
{
	//Whether loading was successfull
	bool ok=true;

	entry.spriteGroup=element.tagName().toStdString().c_str();
	//If element has tag SPRITEINDEX_TAG - set index from it, otherwise 0
	if (element.attributes().contains(SPRITEINDEX_TAG)) 
	{
		entry.index=element.attribute(SPRITEINDEX_TAG).toInt(&ok);
	}
	else
	{
		entry.index=0;
	}

	//Element must has tags, naming texture and texturerect
	ok=ok && element.attributes().contains(TEXTURENAME_TAG);
	if (ok)
	{
		hst::string path = element.attribute(TEXTURENAME_TAG).toStdString().c_str();
		entry.spriteTemplate.setTextureName(path::concat(m_parent,path));
	}

	//Parse texture rectangle
	ok=ok && element.attributes().contains(TEXTURERECT_TAG);
	if (ok)
	{
		//This tag contains rectangle definition, separated by semicolon in format X;Y;W;H
		QStringList list = element.attribute(TEXTURERECT_TAG).split(";");
		ok = ok && list.size()==4;
		if (ok)
		{
			int x=list[0].toInt(&ok);
			int y=list[1].toInt(&ok);
			int w=list[2].toInt(&ok);
			int h=list[3].toInt(&ok);
			entry.spriteTemplate.setTextureRect(hRectF(hPointF(x,y),hPointF(x+w,y+h)));
		}
	}

	//Parse size of sprite
	ok=ok && element.attributes().contains(SIZE_TAG);
	if (ok)
	{
		//This tag contains rectangle definition, separated by semicolon in format W;H
		QStringList list = element.attribute(SIZE_TAG).split(";");
		ok = ok && list.size()==2;
		if (ok)
		{
			int w=list[0].toInt(&ok);
			int h=list[1].toInt(&ok);
			entry.spriteTemplate.setSize(hPointF(w,h));
		}
	}
	
	//Parse transparency
	bool test = entry.spriteTemplate.isTransparent();
	if (element.attributes().contains(TRANSPARENCY_TAG))
	{	
		//This tag contains transparency definition, separated by semicolon in format R;G;B
		QStringList list = element.attribute(TRANSPARENCY_TAG).split(";");
		ok = ok && list.size()==3;
		if (ok)
		{
			int r=list[0].toInt(&ok);
			int g=list[1].toInt(&ok);
			int b=list[2].toInt(&ok);
			entry.spriteTemplate.makeTransparent(hst::color(r,g,b));
		}
	}
	//Return flag
	return ok;
}

