#include "mainpanel.h"
#include <QtGui/QApplication>
#include <QTest>
#include <QTimer>
#include <QThread>

#include <input.h>
#include <stdio.h>
#include <cstdlib>
#include "editorcore/editor.h"
#include "core/ifaceeditor.h"
#include "core/ifacecmdoptions.h"
#include "config/sprite2dconfig.h"
#include <log.h>
#include "editorcore/path.h"

#include "unittests/factory.h"
#include "unittests/ifacecmdoptions_test.h"

#include <QTextCodec>


/**
 * Main loop. where shoud gbe started saddy thread and Qt
 */
int main(int argc, char *argv[])
{	
	#ifdef WIN32
		setlocale(LC_CTYPE, ".1251");
		QTextCodec * codec=QTextCodec::codecForName("Windows-1251");
		QTextCodec::setCodecForTr(codec);
		QTextCodec::setCodecForCStrings(codec);
		QTextCodec::setCodecForLocale(codec);
	#endif
	#ifdef __UNITTESTS
		IFaceCmdOptions args;
		args.parse(argc, argv);
		unittests::Factory tests;
		#define TEST(X) tests.bind(#X, new unittests::FactoryDelegate< X >());
		TEST( IFaceCmdOptionsTest );

		QVector<QString> data = args.unitTests();
		for (int i=0;i<data.size();i++)
		{
			tests.run(data[i]);
		}
		hst::log::inst()->save("log.txt");
	#else
		Editor * editor =new IFaceEditor();
		editor->init(argc,argv);
		delete editor;
	#endif
		return 0;
}