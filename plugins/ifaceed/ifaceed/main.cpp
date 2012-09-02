#include "mainpanel.h"
#include <QtGui/QApplication>
#include <QTest>
#include <QTimer>
#include <QThread>

#include <input.h>
#include <stdio.h>
#include "saddythread.h"
#include "objects/spritegameobject.h"
#include "xmlconfigloader.h"
#include "editorcore/editor.h"
#include "core/ifaceeditor.h"
#include <log.h>
#include "editorcore/path.h"
/**
 *  Global editor state
 */
Editor * GlobalEditor=NULL;  
/**
 *  A main window
 */
MainPanel * w;
/**
 * Whether rendering is Finished
 */
bool RendererFinished=false;
/**
 * A mutex to prevent data for rendering
 */
os::mutex RenderMutex;
/**
 * Global scene
 */
InterlockedScene * GlobalScene;
/**
 * A sprite config
 */
Sprite2DConfig   * GlobalSpritesConfig;
/** Simply quits main window
  */ 
void quit()
{
	QTimer::singleShot(0, w, SLOT(close()));
	RendererFinished=true;
}
/**
 * A callback, which is called by renderer to quit main
 */
void quit(const sad::Event & ev)
{
	sad::Renderer::instance().quit();
	quit();
}

/**
 * A callback, when user presses 'R' button
 */
void reload_sprites(const sad::Event & ev)
{
	if (GlobalSpritesConfig->reload()!=SCR_OK)
	{
		hst::log::inst()->write(hst::string("Failed to reload config"));
	}
}


/**
 *  A thread for saddy to perform window event
 */
void * saddy_thread(void *)
{
	sad::Renderer::instance().init(sad::Settings(800,600,false));
	sad::Renderer::instance().setWindowTitle("Interface Editor Game Window");
	sad::Input::inst()->bindKeyDown(KEY_ESC,quit);
	sad::Input::inst()->bindKeyDown('R',reload_sprites);


	//A global config
	//GlobalSpritesConfig=new Sprite2DConfig("reloadable_sprites");
	//GlobalSpritesConfig->setLoader(new XMLConfigLoader("../config/objects.xml"));
	//if (GlobalSpritesConfig->reload()!=SCR_OK)
	//{
	//	hst::log::inst()->write(hst::string("Loading config failed!\n"));
	//	hst::log::save("log.txt");
	//	exit(1);
	//}

		
	if (RendererFinished==false)
	{
		quit();
	}
	//Clear all containers, to make sure, that all objects is removed
	//GlobalScene->clearContainers();
	//Deletes a global config
	delete GlobalSpritesConfig;

	return 0;
}

void SaddyThread::run()
{
  saddy_thread(NULL);
}

#ifdef __UNITTESTS
QTEST_NOOP_MAIN
#else
/**
 * Main loop. where shoud gbe started saddy thread and Qt
 */
int main(int argc, char *argv[])
{
	GlobalEditor=new IFaceEditor();
	GlobalEditor->init(argc,argv);
	delete GlobalEditor;
	return 0;
}
#endif