VALID=0
BatchToRun=
QtConfig=
if [ "$1" =  "Debug" ]
then
	VALID=1
	BatchToRun=./debug.sh
	QtConfig=debug
fi	
if [ "$1" = "Release" ]
then
	VALID=1
	BatchToRun=./release.sh
	QtConfig=release
fi	
if [ "$VALID" -eq 0 ] 
then
	echo Build script for all libraries for Linux for GCC.
    echo Syntax:
	echo build-all-linux-gcc.sh "[Debug|Release]"
	exit 1
fi
eval $BatchToRun
cd plugins/atlasgen
qmake CONFIG+=$QtConfig atlasgen.pro
make
cd ../..
cd plugins/exporter
qmake CONFIG+=$QtConfig exporter.pro
make
cd ../..
cd plugins/freetype
eval $BatchToRun
cd ../..
cd plugins/ifaceed
qmake CONFIG+=$QtConfig ifaceed.pro
make
cd ../..
cd plugins/duktape
eval $BatchToRun
cd ../..
cd plugins/irrklang
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
cd tests/duktape
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