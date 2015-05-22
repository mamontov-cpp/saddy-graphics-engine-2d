/*! \mainpage core::Editor
	
	A simple editor for editing ingame screens. Supports placing some sprite and text, saving and
	loading to xml files with those. Loads sprites from specific configs - see examples.
 */
#include "mainpanel.h"

#include <QTextCodec>

#include <stdio.h>

#include "core/editor.h"

/**
 * Main loop. where shoud gbe started saddy thread and Qt
 */
int main(int argc, char *argv[])
{	
	#ifdef WIN32
		setlocale(LC_CTYPE, ".1251");
		QTextCodec * codec=QTextCodec::codecForName("Windows-1251");
    #if !HAVE_QT5
        QTextCodec::setCodecForTr(codec);
		QTextCodec::setCodecForCStrings(codec);
    #endif	
        QTextCodec::setCodecForLocale(codec);
	#endif
	core::Editor * editor =new core::Editor();
	editor->init(argc,argv);
	delete editor;
	return 0;
}
