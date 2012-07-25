g++ -g -c ftfont.cpp -o ftfont.o -I"..\..\include" -I"%FREETYPE_INCLUDE%"  -DWIN32
ar cq ../../lib/libsaddy-ft-debug.a ftfont.o 