g++ -g -c font.cpp -o font.o -I"..\..\include" -I"%FREETYPE_INCLUDE%"  -DWIN32
ar cr ../../lib/libsaddy-ft-debug.a font.o 