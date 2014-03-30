#include "fontdatabase.h"
#include <fontmanager.h>
#include <QStringList>
#include <log/log.h>
#include <unused.h>


IFaceEditorFont::IFaceEditorFont()
{
	m_qtfont = QFont();
	m_appfontindex = -1;
	m_saddyfont = NULL;
	m_qdb = NULL;
	m_name = "";
}

void IFaceEditorFont::unloadFromQDB(const QString & name,UNUSED QFontDatabase & db)
{
 if (m_appfontindex != -1) {
	 QFontDatabase::removeApplicationFont(m_appfontindex);
	 m_appfontindex = -1;
	m_qtfont = QFont();
 }
 if (m_saddyfont !=NULL) {
	 sad::FontManager::ref()->remove(name.toStdString().c_str());
	m_saddyfont = NULL;
 }
}

bool IFaceEditorFont::loadFont(const QString & file, const QString & name, QFontDatabase & db, sad::FontManager * mgr)
{
	SL_SCOPE("IFaceEditorFont::loadFont");
	if (m_appfontindex!=-1 || m_saddyfont!=NULL) {
		if (m_qdb==NULL || m_name.count() == 0)
			SL_CRITICAL("Empty QDB or name is empty!");
		unloadFromQDB(m_name,*m_qdb);
	}
	QFont fnt;
	sad::freetype::Font * tmp = new sad::freetype::Font();
	bool ok =true;

	int id = db.addApplicationFont(file);
	ok = ok && (id!=-1);
	
	if (id!=-1) {
		QStringList lst = db.applicationFontFamilies ( id );
		fnt = db.font(lst[0],"",18);
		ok = (fnt!=QFont());
	}
	ok = ok && tmp->load(file.toStdString().c_str());
	if (!ok) {
		delete tmp;
	} else {
		m_name = name;
		m_qdb =  &db;
		m_qtfont = fnt;
		m_appfontindex = id;
		m_saddyfont = tmp;
		mgr->add(m_saddyfont,m_name.toStdString().c_str());
	}
	return ok;
}

IFaceEditorFontListCursor::IFaceEditorFontListCursor(const IFaceEditorFontsMap::const_iterator & i, 
												     IFaceEditorFontList * parent)
{
	m_impl = i;
	m_parent = parent;
}

IFaceEditorFont * IFaceEditorFontListCursor::fonts() 
{
	if (end())
		return NULL;
	return m_impl.value();
}

QString    IFaceEditorFontListCursor::name()
{
	if (end())
		return QString("");
	return QString(m_impl.key().data());
}

IFaceEditorFontListCursor & IFaceEditorFontListCursor::operator++(int)
{
	if (end())
		return *this;
	m_impl++;
	return *this;
}

bool IFaceEditorFontListCursor::end()
{
	return (m_parent->m_map.const_end() == m_impl);
}

IFaceEditorFontList::IFaceEditorFontList()
{
	m_manager = new sad::FontManager();
}

IFaceEditorFontList::~IFaceEditorFontList()
{
	//delete m_manager;
}

bool IFaceEditorFontList::hasFont(const QString & fontName)
{
	return m_map.contains(fontName.toStdString().c_str());
}

IFaceEditorFontListCursor IFaceEditorFontList::begin()
{
	return IFaceEditorFontListCursor(m_map.const_begin(),this);
}

IFaceEditorFontLoadResult IFaceEditorFontList::tryLoadFont(const QString & fontName,const QString & fileName)
{
	if (hasFont(fontName))
		return IEFLR_ALREADY_EXISTS;
	IFaceEditorFont * fnt = new IFaceEditorFont();
	IFaceEditorFontLoadResult result = IEFLR_OK;
	if (fnt->loadFont(fileName,fontName,m_db, m_manager)) 
	{
		m_map.insert(fontName.toStdString().c_str(),fnt);
	} 
	else 
	{
		result = IEFLR_FAIL;
		delete fnt;
	}
	return result;
}


IFaceEditorFont * IFaceEditorFontList::font(const QString & fontName)
{
	if (!hasFont(fontName))
	{
		return NULL;
	}
	return m_map[fontName.toStdString().c_str()];
}
