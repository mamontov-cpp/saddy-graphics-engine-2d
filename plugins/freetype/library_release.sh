g++ ftfont.cpp -c -o ftfont.o  -L"../../include"  -I"/usr/include/freetype2"  -DLINUX 
ar cq ../../lib/libsaddy-ft-release.a ftfont.o 