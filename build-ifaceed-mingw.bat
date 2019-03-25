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
cd plugins/freetype
call %BatchToRun% || goto :error
cd ../..
cd tools/ifaceed
qmake CONFIG+=%QtConfig% ifaceed.pro
mingw32-make
cd ../..
goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
