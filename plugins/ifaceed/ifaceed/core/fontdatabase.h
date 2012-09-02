/** \file   fontdatabase.h
	\author HiddenSeeker

	Defines a database with amount of fonts
 */
#include <QFont>
#include <ftfont.h>
#include <templates/hhash.hpp>
#include <QString>
#include <QFontDatabase>
#pragma once;

/** Defines a font, used in editor
 */
class IFaceEditorFont
{
 private:
		  QFontDatabase * m_qdb;  //!< Qt database
		  QString m_name;         //!< Name of font

		  QFont m_qtfont;
		  int   m_appfontindex;
		  FTFont * m_saddyfont;
 public:
		  /** Creates empty unusable font
		   */
		  IFaceEditorFont();
		  /** Returns Qt font
			  \return Qt font data
		   */
		  inline const QFont & qtFont() const { return m_qtfont; }
		  /** Returns Saddy font
			  \return Saddy font data
		   */
		  inline FTFont * sadFont() const { return m_saddyfont; }
		  /** Unloads a font from qt database
			  \param[in] name of fount, used to unregister it from FontManager
			  \param[out] db
		   */ 
		  void unloadFromQDB(const QString & name,QFontDatabase & db);
		  /** Loads font from file.
			  Also, registers it in font manager and in database
			  \param[in] file input file
			  \param[in] name viewable name of file
			  \param[in] db   database
			  \return whether loading was successfull, false if not
		   */
		  bool loadFont(const QString & file, const QString & name, QFontDatabase & db);
};

/** A map, which contains all fonts
 */
typedef  hst::hash<hst::string, IFaceEditorFont *>  IFaceEditorFontsMap;

/** Determines possible results of loading data
 */
enum IFaceEditorFontLoadResult
{
	IEFLR_OK = 1,
	IEFLR_ALREADY_EXISTS = 2,
	IEFLR_FAIL = 3
};
/** A cursor for iterating through font list
 */
class IFaceEditorFontListCursor
{
 friend class IFaceEditorFontList;
 private:
		/** Iterator data
		 */
		IFaceEditorFontsMap::const_iterator m_impl;
		/** Parent data
		 */
		IFaceEditorFontList * m_parent;

		/** Creates a new cursor
			\param[in] i iterator
			\param[in] parent parent data
		 */
		IFaceEditorFontListCursor(const IFaceEditorFontsMap::const_iterator & i, 
								  IFaceEditorFontList * parent);
 public:
		/** Returns assigned font, null on end
		 */
		IFaceEditorFont * fonts();
		/** Returns name of assigned data, empty on end
		 */
		 QString   name();
		/** Moves to next entry
		 */
		IFaceEditorFontListCursor & operator++(int);
		/** Determines, whether cursor is at end of list
		 */
		bool end();

};

/** List of fonts in database
 */
class IFaceEditorFontList
{
 friend class IFaceEditorFontListCursor;
 private:
	    /** Map with all of data
		 */
		IFaceEditorFontsMap m_map;
		/** Font database to store all of data
		 */
		QFontDatabase m_db;
 public:
	    /** Initializes empty font list
		 */
		IFaceEditorFontList();
		/** Destroys all data, associated with a font
		 */
		~IFaceEditorFontList();
		/** Determines, whether font with name was already loaded
			\param[in] fontName name of font
			\return whether font was already loaded
		 */
		bool hasFont(const QString & fontName);
		/** Tries load some font with
			\param[in] fontName  with which font will be registered
			\param[in] fileName  name of file
		 */
		IFaceEditorFontLoadResult tryLoadFont(const QString & fontName,const QString & fileName);
		/** Returns a starting cursor
		 */
		IFaceEditorFontListCursor begin();
};
