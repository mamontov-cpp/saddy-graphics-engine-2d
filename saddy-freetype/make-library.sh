g++ ftfont.cpp -c -o ftfont.o  -L".."  -I"/usr/include/freetype2"  -DLINUX 
ar cq ../libsaddy-freetype.a ftfont.o ../build/fontmanager.o
