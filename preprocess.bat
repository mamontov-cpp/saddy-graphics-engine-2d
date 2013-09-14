g++ tools/hbuild.cpp -o bin/hbuild.exe
bin\hbuild.exe  preprocess\Makefile_library_mingw.hbd -o makefiles\Makefile_Library_Debug.mingw "TYPE" "debug" "CCOPTS" "-g"
bin\hbuild.exe  preprocess\Makefile_library_mingw.hbd -o makefiles\Makefile_Library_Release.mingw "TYPE" "release" "CCOPTS" "-O2"
bin\hbuild.exe  preprocess\Makefile_library_linux.hbd -o makefiles\Makefile_Library_Debug.linux "TYPE" "debug" "CCOPTS" "-g"
bin\hbuild.exe  preprocess\Makefile_library_linux.hbd -o makefiles\Makefile_Library_Release.linux "TYPE" "release" "CCOPTS" "-O2"