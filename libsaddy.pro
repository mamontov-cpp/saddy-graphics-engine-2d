#-------------------------------------------------
#
# Project created by QtCreator 2014-10-30T09:57:54
#
#-------------------------------------------------

QT       -= core gui

TARGET = libsaddy
TEMPLATE = lib
CONFIG += staticlib

SOURCES += src/camera.cpp \
    src/classmetadata.cpp \
    src/classmetadatacastfunction.cpp \
    src/classmetadatacontainer.cpp \
    src/closure.cpp \
    src/font.cpp \
    src/formattedlabel.cpp \
    src/fpsinterpolation.cpp \
    src/fuzzyequal.cpp \
    src/geometry2d.cpp \
    src/geometry3d.cpp \
    src/glcontext.cpp \
    src/keymouseconditions.cpp \
    src/label.cpp \
    src/mainloop.cpp \
    src/mousecursor.cpp \
    src/object.cpp \
    src/objectdependentfpsinterpolation.cpp \
    src/opengl.cpp \
    src/orthographiccamera.cpp \
    src/pausabletimer.cpp \
    src/periodicalevent.cpp \
    src/primitiverenderer.cpp \
    src/refcountable.cpp \
    src/renderer.cpp \
    src/sadhash.cpp \
    src/sadluvcolor.cpp \
    src/sadmutex.cpp \
    src/sadscopedlock.cpp \
    src/sadsemaphore.cpp \
    src/sadsleep.cpp \
    src/sadstring.cpp \
    src/sadthread.cpp \
    src/sadthreadexecutablefunction.cpp \
    src/sadwstring.cpp \
    src/scene.cpp \
    src/scenenode.cpp \
    src/settings.cpp \
    src/sprite2d.cpp \
    src/sprite3d.cpp \
    src/texture.cpp \
    src/texturemappedfont.cpp \
    src/timer.cpp \
    src/window.cpp \
    src/3rdparty/format/format.cc \
    src/3rdparty/picopng/picopng.cpp \
    src/3rdparty/SDL/x11messagebox.cpp \
    src/animations/animationscallback.cpp \
    src/cli/clioptionvalue.cpp \
    src/cli/cliparser.cpp \
    src/db/custom/customobject.cpp \
    src/db/custom/customschema.cpp \
    src/db/custom/customschemafile.cpp \
    src/db/schema/schema.cpp \
    src/db/dbcanbecastedfromto.cpp \
    src/db/dbconversiontable.cpp \
    src/db/dbdatabase.cpp \
    src/db/dberror.cpp \
    src/db/dblink.cpp \
    src/db/dbobject.cpp \
    src/db/dbobjectfactory.cpp \
    src/db/dbpopulatescenesfromdatabase.cpp \
    src/db/dbproperty.cpp \
    src/db/dbstoredpropertyfactory.cpp \
    src/db/dbtable.cpp \
    src/db/dbtypename.cpp \
    src/db/dbvariant.cpp \
    src/dialogue/dialogue.cpp \
    src/dialogue/phrase.cpp \
    src/hfsm/hfsmhandler.cpp \
    src/hfsm/hfsmmachine.cpp \
    src/hfsm/hfsmshared.cpp \
    src/hfsm/hfsmstate.cpp \
    src/hfsm/hfsmtransition.cpp \
    src/hfsm/hfsmtransitionrepository.cpp \
    src/imageformats/bmploader.cpp \
    src/imageformats/loader.cpp \
    src/imageformats/pngloader.cpp \
    src/imageformats/tgaloader.cpp \
    src/input/controls.cpp \
    src/input/events.cpp \
    src/input/handlerconditions.cpp \
    src/input/handlers.cpp \
    src/log/consoletarget.cpp \
    src/log/filetarget.cpp \
    src/log/log.cpp \
    src/log/logmessage.cpp \
    src/log/logscope.cpp \
    src/log/logtarget.cpp \
    src/os/consoleimpl.cpp \
    src/os/generatemipmaps30.cpp \
    src/os/glcontextimpl.cpp \
    src/os/keydecoder.cpp \
    src/os/muteximpl.cpp \
    src/os/semaphoreimpl.cpp \
    src/os/systemeventdispatcher.cpp \
    src/os/systemwindowevent.cpp \
    src/os/threadimpl.cpp \
    src/os/timerimpl.cpp \
    src/os/windowimpl.cpp \
    src/p2d/app/app.cpp \
    src/p2d/app/appobject.cpp \
    src/p2d/app/objectemitter.cpp \
    src/p2d/app/way.cpp \
    src/p2d/axle.cpp \
    src/p2d/body.cpp \
    src/p2d/bouncesolver.cpp \
    src/p2d/bounds.cpp \
    src/p2d/broadcollisiondetector.cpp \
    src/p2d/circle.cpp \
    src/p2d/circletohulltransformer.cpp \
    src/p2d/collisiondetector.cpp \
    src/p2d/collisionevent.cpp \
    src/p2d/collisionhandler.cpp \
    src/p2d/collisionshape.cpp \
    src/p2d/collisiontest.cpp \
    src/p2d/convexhull.cpp \
    src/p2d/elasticforce.cpp \
    src/p2d/findcontactpoints.cpp \
    src/p2d/force.cpp \
    src/p2d/grahamscan.cpp \
    src/p2d/infiniteline.cpp \
    src/p2d/line.cpp \
    src/p2d/multisamplingcollisiondetector.cpp \
    src/p2d/rectangle.cpp \
    src/p2d/simplecollisiondetector.cpp \
    src/p2d/vector.cpp \
    src/p2d/walls.cpp \
    src/p2d/weight.cpp \
    src/p2d/world.cpp \
    src/p2d/worldsteptask.cpp \
    src/pipeline/pipeline.cpp \
    src/pipeline/pipelinedelegate.cpp \
    src/pipeline/pipelineprocess.cpp \
    src/pipeline/pipelinestep.cpp \
    src/pipeline/pipelinetask.cpp \
    src/resource/abstractlink.cpp \
    src/resource/error.cpp \
    src/resource/folder.cpp \
    src/resource/physicalfile.cpp \
    src/resource/resource.cpp \
    src/resource/resourcefactory.cpp \
    src/resource/textureatlasfile.cpp \
    src/resource/tree.cpp \
    src/util/deletetexturetask.cpp \
    src/util/fs.cpp \
    src/util/swaplayerstask.cpp

INCLUDEPATH += ../saddy/include ../saddy/include/png ../saddy/include/tga

HEADERS += include/camera.h \
    include/classmetadata.h \
    include/classmetadatacastfunction.h \
    include/classmetadatacontainer.h \
    include/closure.h \
    include/font.h \
    include/formattedlabel.h \
    include/fpsinterpolation.h \
    include/fuzzyequal.h \
    include/geometry2d.h \
    include/geometry3d.h \
    include/glcontext.h \
    include/keycodes.h \
    include/keymouseconditions.h \
    include/label.h \
    include/mainloop.h \
    include/matrix2x2.h \
    include/matrix3x3.h \
    include/maybe.h \
    include/mousecursor.h \
    include/object.h \
    include/objectdependentfpsinterpolation.h \
    include/opengl.h \
    include/orthographiccamera.h \
    include/pausabletimer.h \
    include/periodicalevent.h \
    include/primitiverenderer.h \
    include/refcountable.h \
    include/renderer.h \
    include/sadcolor.h \
    include/sadhash.h \
    include/sadhash_src.h \
    include/sadlinkedlist.h \
    include/sadlinkedlist_src.h \
    include/sadlist.h \
    include/sadlist_src.h \
    include/sadluvcolor.h \
    include/sadmutex.h \
    include/sadpair.h \
    include/sadpair_src.h \
    include/sadpoint.h \
    include/sadptrhash.h \
    include/sadptrie.h \
    include/sadptrie_src.h \
    include/sadptrvector.h \
    include/sadrect.h \
    include/sadscopedlock.h \
    include/sadsemaphore.h \
    include/sadsize.h \
    include/sadsleep.h \
    include/sadstring.h \
    include/sadthread.h \
    include/sadthreadexecutablefunction.h \
    include/sadvector.h \
    include/sadvector_src.h \
    include/sadwstring.h \
    include/scene.h \
    include/scenenode.h \
    include/settings.h \
    include/sprite2d.h \
    include/sprite3d.h \
    include/temporarilyimmutablecontainer.h \
    include/texture.h \
    include/texturemappedfont.h \
    include/timer.h \
    include/unused.h \
    include/window.h \
    include/3rdparty/format/format.h \
    include/3rdparty/format/stdint.h \
    include/3rdparty/glext/glext.h \
    include/3rdparty/glext/glxext.h \
    include/3rdparty/glext/wglext.h \
    include/3rdparty/picojson/getproperty.h \
    include/3rdparty/picojson/picojson.h \
    include/3rdparty/picojson/valuetotype.h \
    include/3rdparty/tpunit++/tpunit++.hpp \
    include/3rdparty/tpunit++/tpunitw.h \
    include/3rdparty/ttmath/ttmath.h \
    include/3rdparty/ttmath/ttmathbig.h \
    include/3rdparty/ttmath/ttmathdec.h \
    include/3rdparty/ttmath/ttmathint.h \
    include/3rdparty/ttmath/ttmathmisc.h \
    include/3rdparty/ttmath/ttmathobjects.h \
    include/3rdparty/ttmath/ttmathparser.h \
    include/3rdparty/ttmath/ttmaththreads.h \
    include/3rdparty/ttmath/ttmathtypes.h \
    include/3rdparty/ttmath/ttmathuint.h \
    include/3rdparty/ttmath/ttmathuint_noasm.h \
    include/3rdparty/ttmath/ttmathuint_x86.h \
    include/3rdparty/ttmath/ttmathuint_x86_64.h \
    include/animations/animationscallback.h \
    include/cli/args.h \
    include/cli/optiontype.h \
    include/cli/optionvalue.h \
    include/cli/parser.h \
    include/cli/token.h \
    include/db/custom/customobject.h \
    include/db/custom/customschema.h \
    include/db/custom/customschemafile.h \
    include/db/schema/schema.h \
    include/db/dbcanbecastedfromto.h \
    include/db/dbconversiontable.h \
    include/db/dbdatabase.h \
    include/db/dberror.h \
    include/db/dbfield.h \
    include/db/dblink.h \
    include/db/dbmethodpair.h \
    include/db/dbobject.h \
    include/db/dbobjectfactory.h \
    include/db/dbpopulatescenesfromdatabase.h \
    include/db/dbproperty.h \
    include/db/dbstoredproperty.h \
    include/db/dbstoredpropertyfactory.h \
    include/db/dbtable.h \
    include/db/dbtypename.h \
    include/db/dbvariant.h \
    include/db/load.h \
    include/db/save.h \
    include/db/saveloadfwd.h \
    include/dialogue/dialogue.h \
    include/dialogue/phrase.h \
    include/freetype/font.h \
    include/hfsm/hfsmhandler.h \
    include/hfsm/hfsmmachine.h \
    include/hfsm/hfsmshared.h \
    include/hfsm/hfsmstate.h \
    include/hfsm/hfsmtransition.h \
    include/hfsm/hfsmtransitionrepository.h \
    include/imageformats/bmploader.h \
    include/imageformats/loader.h \
    include/imageformats/pngloader.h \
    include/imageformats/tgaloader.h \
    include/input/controls.h \
    include/input/events.h \
    include/input/handlerconditions.h \
    include/input/handlers.h \
    include/log/color.h \
    include/log/consoletarget.h \
    include/log/filetarget.h \
    include/log/log.h \
    include/log/logmessage.h \
    include/log/logscope.h \
    include/log/logtarget.h \
    include/log/priority.h \
    include/log/stringcaster.h \
    include/os/consoleimpl.h \
    include/os/generatemipmaps30.h \
    include/os/glcontexthandle.h \
    include/os/glcontextimpl.h \
    include/os/glheaders.h \
    include/os/keydecoder.h \
    include/os/muteximpl.h \
    include/os/semaphoreimpl.h \
    include/os/systemeventdispatcher.h \
    include/os/systemwindowevent.h \
    include/os/threadimpl.h \
    include/os/timerimpl.h \
    include/os/windowhandles.h \
    include/os/windowimpl.h \
    include/p2d/app/app.h \
    include/p2d/app/constants.h \
    include/p2d/app/object.h \
    include/p2d/app/objectemitter.h \
    include/p2d/app/way.h \
    include/p2d/angularforce.h \
    include/p2d/axle.h \
    include/p2d/body.h \
    include/p2d/bouncesolver.h \
    include/p2d/bounds.h \
    include/p2d/broadcollisiondetector.h \
    include/p2d/circle.h \
    include/p2d/circletohulltransformer.h \
    include/p2d/collides1d.h \
    include/p2d/collisiondetector.h \
    include/p2d/collisionevent.h \
    include/p2d/collisionhandler.h \
    include/p2d/collisionmultimethod.h \
    include/p2d/collisionshape.h \
    include/p2d/collisiontest.h \
    include/p2d/convexhull.h \
    include/p2d/elasticforce.h \
    include/p2d/findcontactpoints.h \
    include/p2d/force.h \
    include/p2d/ghostoptions.h \
    include/p2d/grahamscan.h \
    include/p2d/infiniteline.h \
    include/p2d/line.h \
    include/p2d/movement.h \
    include/p2d/multisamplingcollisiondetector.h \
    include/p2d/point.h \
    include/p2d/rectangle.h \
    include/p2d/simplecollisiondetector.h \
    include/p2d/tickable.h \
    include/p2d/vector.h \
    include/p2d/walls.h \
    include/p2d/weight.h \
    include/p2d/world.h \
    include/p2d/worldsteptask.h \
    include/pipeline/pipeline.h \
    include/pipeline/pipelinedelegate.h \
    include/pipeline/pipelineprocess.h \
    include/pipeline/pipelinestep.h \
    include/pipeline/pipelinetask.h \
    include/resource/abstractlink.h \
    include/resource/error.h \
    include/resource/folder.h \
    include/resource/link.h \
    include/resource/physicalfile.h \
    include/resource/resource.h \
    include/resource/resourcefactory.h \
    include/resource/textureatlasfile.h \
    include/resource/tree.h \
    include/util/commoncheckedcast.h \
    include/util/deletetexturetask.h \
    include/util/formattedlabelarg.h \
    include/util/free.h \
    include/util/fs.h \
    include/util/getterproxy.h \
    include/util/pointercallback.h \
    include/util/sadthreadexecutablefunction.h \
    include/util/setterproxy.h \
    include/util/swaplayerstask.h


DESTDIR = ../saddy/lib/

unix {
	DEFINES += "UNIX=1"
	DEFINES += "LINUX=1"
	DEFINES += "GCC=1"
}

win32 {
	DEFINES += "WIN32=1"
	DEFINES +=  "MINGW=1"
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG(debug, debug|release) {
    TARGET = saddy-debug
}

CONFIG(release, debug|release) {
    TARGET = saddy-release
}

QMAKE_CXXFLAGS += -Wno-reorder -Wno-unused -Wno-sign-compare -w
