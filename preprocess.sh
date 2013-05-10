g++ tools/hbuild.cpp -o bin/hbuild -DLINUX
bin/hbuild  preprocess/Makefile_library_debug_mingw.hbd -o makefiles/Makefile_Library_Debug.mingw
bin/hbuild  preprocess/Makefile_library_release_mingw.hbd -o makefiles/Makefile_Library_Release.mingw
bin/hbuild  preprocess/Makefile_library_debug_linux.hbd -o makefiles/Makefile_Library_Debug.linux
bin/hbuild  preprocess/Makefile_library_release_linux.hbd -o makefiles/Makefile_Library_Release.linux