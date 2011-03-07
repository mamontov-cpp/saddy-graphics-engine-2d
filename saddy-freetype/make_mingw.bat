g++ -c ftfont.cpp -o ftfont.o -I"C:\GnuWin32\include\freetype2"  -DWIN32
ar cq ../lib/libsaddy-freetype.a ftfont.o 