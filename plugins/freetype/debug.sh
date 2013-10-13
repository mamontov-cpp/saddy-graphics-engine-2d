g++ -g ftfont.cpp -c -o ftfont.o  -I"../../include"  -I"/usr/include/freetype2"  -DLINUX 
ar cr ../../lib/libsaddy-ft-debug.a ftfont.o 