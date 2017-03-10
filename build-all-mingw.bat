@echo off
SET VALID=0
SET BatchToRun=
SET QtConfig=
SET CHECKQTVERTOOL=
if [%1] EQU [Debug]  (
	SET VALID=1
	SET BatchToRun=debug.bat
	SET QtConfig=debug
	SET CHECKQTVERTOOL="bin/isqt580-debug.exe"
)
if [%1] EQU [Release]  (
	SET VALID=1
	SET BatchToRun=release.bat
	SET QtConfig=release
	SET CHECKQTVERTOOL="bin/isqt580-release.exe"
)
if [%VALID%] EQU [0]  (
	echo Build script for all libraries for MinGW. Just run it from Command line of Qt
    echo Syntax:
	echo build-all-mingw.bat "[Debug|Release]"
	exit /B 1
)
@echo on
call %BatchToRun%
cd tools/atlasgen
qmake CONFIG+=%QtConfig% atlasgen.pro
mingw32-make
cd ../..
cd tools/exporter
qmake CONFIG+=%QtConfig% exporter.pro
mingw32-make
cd ../..
cd plugins/freetype
call %BatchToRun%
cd ../..
cd tools/ifaceed
qmake CONFIG+=%QtConfig% ifaceed.pro
mingw32-make
cd ../..
cd plugins/dukpp-03
call %BatchToRun%
cd ../..
cd plugins/irrklang
call %BatchToRun%
cd ../..
cd examples/game
call %BatchToRun%
cd ../..
cd examples/multithreading
call %BatchToRun%
cd ../..
cd examples/stress-physics
call %BatchToRun%
cd ../..
cd tests/animations
call %BatchToRun%
cd ../..
cd tests/db
call %BatchToRun%
cd ../..
cd tests/dukpp-03
call %BatchToRun%
cd ../..
cd tests/freetype
call %BatchToRun%
cd ../..
cd tests/hfsm
call %BatchToRun%
cd ../..
cd tests/input
call %BatchToRun%
cd ../..
cd tests/irrklang
call %BatchToRun%
cd ../..
cd tests/p2d
call %BatchToRun%
cd ../..
cd tests/pipeline
call %BatchToRun%
cd ../..
cd tests/resource
call %BatchToRun%
cd ../..
cd tests/sad
call %BatchToRun%
cd ../..
cd tests/layouts
call %BatchToRun%
cd ../..
cd tools/isqt580
qmake CONFIG+=%QtConfig% isqt580.pro
mingw32-make
cd ../..
%CHECKQTVERTOOL%
if errorlevel 1 (
	cd plugins/qt-widget
	qmake CONFIG+=%QtConfig% qt-widget.pro
	mingw32-make
	cd ../..
	
	cd examples/example-game-in-qt
	qmake CONFIG+=%QtConfig% example-game-in-qt.pro
	mingw32-make
	cd ../..
)