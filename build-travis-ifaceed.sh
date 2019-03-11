if [[ $1 == "Debug"]] ; then QtConfig="debug"; else QtConfig="release" ; fi
if [[ $1 == "Debug"]] ; then export QtConfig="debug"; else export QtConfig="release" ; fi
if [[ $1 == "Debug"]] ; then CHECKQTVERTOOL="bin/isqt580-debug"; else CHECKQTVERTOOL="bin/isqt580-release" ; fi
if [[ $1 == "Debug"]] ; then export CHECKQTVERTOOL="bin/isqt580-debug"; else export CHECKQTVERTOOL="bin/isqt580-release" ; fi  
set -e
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd plugins/freetype
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tools/ifaceed
qmake CONFIG+=$QtConfig ifaceed.pro
make
cd ../..
cd tools/atlasgen
qmake CONFIG+=$QtConfig atlasgen.pro
make
cd ../..
cd tools/exporter
qmake CONFIG+=$QtConfig exporter.pro
make
cd ../..
cd tools/isqt580
qmake CONFIG+=$QtConfig isqt580.pro
make
cd ../..
cd plugins/qt-widget
qmake CONFIG+=$QtConfig qt-widget.pro
make
cd ../..
cd examples/example-game-in-qt
qmake CONFIG+=$QtConfig example-game-in-qt.pro
make
cd ../..