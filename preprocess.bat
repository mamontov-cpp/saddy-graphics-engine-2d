g++ tools/hbuild.cpp -o bin/hbuild.exe
bin\hbuild.exe  preprocess\Makefile.hbd -o makefiles\Makefile_Library_Debug.mingw "PLATFORM"  "WIN32" "TYPE" "debug"
bin\hbuild.exe  preprocess\Makefile.hbd -o makefiles\Makefile_Library_Release.mingw  "PLATFORM"  "WIN32" "TYPE" "release" 
bin\hbuild.exe  preprocess\Makefile.hbd -o makefiles\Makefile_Library_Debug.linux  "PLATFORM"  "LINUX"  "TYPE" "debug" 
bin\hbuild.exe  preprocess\Makefile.hbd -o makefiles\Makefile_Library_Release.linux "PLATFORM"  "LINUX"  "TYPE" "release"