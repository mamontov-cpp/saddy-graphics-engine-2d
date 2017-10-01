VALID=0
BatchToRun=
QtConfig=
CheckQtVerTool=
if [ "$1" =  "Debug" ]
then
	VALID=1
	BatchToRun=./debug.sh
	QtConfig=debug
	CheckQtVerTool="./bin/isqt580-debug"
fi
if [ "$1" = "Release" ]
then
	VALID=1
	BatchToRun=./release.sh
	QtConfig=release
	CheckQtVerTool="./bin/isqt580-release"
fi	
if [ "$VALID" -eq 0 ] 
then
	echo Build script for all libraries for Linux for GCC.
    echo Syntax:
	echo build-all-linux-gcc.sh "[Debug|Release]"
	exit 1
fi
eval $BatchToRun
cd tools/atlasgen
qmake CONFIG+=$QtConfig atlasgen.pro
make
cd ../..
cd tools/exporter
qmake CONFIG+=$QtConfig exporter.pro
make
cd ../..
cd plugins/freetype
eval $BatchToRun
cd ../..
cd tools/ifaceed
qmake CONFIG+=$QtConfig ifaceed.pro
make
cd ../..
cd plugins/dukpp-03
eval $BatchToRun
cd ../..
cd plugins/irrklang
eval $BatchToRun
cd ../..
cd plugins/dukpp-03-irrklang
eval $BatchToRun
cd ../..
cd examples/game
eval $BatchToRun
cd ../..
cd examples/multithreading
eval $BatchToRun
cd ../..
cd examples/stress-physics
eval $BatchToRun
cd ../..
cd tests/animations
eval $BatchToRun
cd ../..
cd tests/db
eval $BatchToRun
cd ../..
cd tests/dukpp-03
eval $BatchToRun
cd ../..
cd tests/freetype
eval $BatchToRun
cd ../..
cd tests/hfsm
eval $BatchToRun
cd ../..
cd tests/input
eval $BatchToRun
cd ../..
cd tests/irrklang
eval $BatchToRun
cd ../..
cd tests/dukpp-03-irrklang
eval $BatchToRun
cd ../..
cd tests/p2d
eval $BatchToRun
cd ../..
cd tests/pipeline
eval $BatchToRun
cd ../..
cd tests/resource
eval $BatchToRun
cd ../..
cd tests/sad
eval $BatchToRun
cd ../..
cd tests/layouts
eval $BatchToRun
cd ../..
cd tools/isqt580
qmake CONFIG+=$QtConfig isqt580.pro
make
cd ../..
eval $CheckQtVerTool
if [ $? -eq 1 ] 
then
	cd plugins/qt-widget
	qmake CONFIG+=$QtConfig qt-widget.pro
	make
	cd ../..
	
	cd examples/example-game-in-qt
	qmake CONFIG+=$QtConfig example-game-in-qt.pro
	make
	cd ../..
fi
