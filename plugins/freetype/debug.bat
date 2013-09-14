g++ -g -c ftfont.cpp -o ftfont.o -I"..\..\include" -I"%FREETYPE_INCLUDE%"  -DWIN32
ar cr ../../lib/libsaddy-ft-debug.a ftfont.o 