../mkbuild.sh
g++ ftfont.cpp -c -o ftfont.o  -L".."  -I"/usr/include/freetype2"  -DLINUX 
ar cq ../lib/libsaddy-freetype.a ftfont.o ../build/fontmanager.o
