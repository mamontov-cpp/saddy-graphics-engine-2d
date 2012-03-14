g++ -c ftfont.cpp -o ftfont.o -I"..\..\include" -I"C:\GnuWin32\include\freetype2"  -DWIN32
ar cq ../../lib/libsaddy-ft-release.a ftfont.o 