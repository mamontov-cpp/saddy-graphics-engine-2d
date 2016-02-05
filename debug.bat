IF not exist lib\nul ( mkdir lib )
cd makefiles
mingw32-make -f Makefile_Library_Debug.mingw
cd ..