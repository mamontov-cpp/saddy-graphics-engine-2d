g++ -g font.cpp -c -o font.o  -I"../../include"  -I"/usr/include/freetype2"  -DLINUX 
ar cr ../../lib/libsaddy-ft-debug.a font.o 