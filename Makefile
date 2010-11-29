LDFLAGS= -lm -lGL -lGLU
INCLUDE= -Iinclude -Iinclude/primitives -Iinclude/templates -Iinclude/png -Iinclude/tga  -I/usr/include/
OUTPUT   = saddy-test
DEFINES  = -DUNIX -DLINUX -DGCC
BUILD       = build/log.o build/scene.o build/texture.o build/texturemanager.o build/hcolor.o build/hhash.o build/hstring.o build/hstringlist.o build/main.o build/hwstring.o build/convert.o build/decodepng.o build/decompress.o build/huffman.o  build/inflateblock.o build/inflatedynamic.o build/inflator.o build/pngbasic.o build/unfilter.o build/zlib.o build/defaulttextureloader.o build/tga.o build/tgareader.o build/bmploader.o build/input.o build/pngtexture.o build/renderer.o build/settings.o build/testnode.o build/fontmanager.o build/tmfont.o build/fontnode.o
all:
	./mkbuild.sh
	g++ -c src/log.cpp                    -o build/log.o                    $(INCLUDE) $(DEFINES)
	g++ -c src/main.cpp                 -o build/main.o                 $(INCLUDE) $(DEFINES) 
	g++ -c src/scene.cpp                -o build/scene.o                $(INCLUDE) $(DEFINES)
	g++ -c src/texture.cpp              -o build/texture.o              $(INCLUDE) $(DEFINES)
	g++ -c src/texturemanager.cpp -o build/texturemanager.o $(INCLUDE) $(DEFINES)
	g++ -c src/primitives/hcolor.cpp         -o build/hcolor.o                $(INCLUDE) $(DEFINES)
	g++ -c src/templates/hhash.cpp          -o build/hhash.o                $(INCLUDE) $(DEFINES)
	g++ -c src/templates/hstring.cpp         -o build/hstring.o              $(INCLUDE) $(DEFINES)
	g++ -c src/templates/hwstring.cpp         -o build/hwstring.o              $(INCLUDE) $(DEFINES)
	g++ -c src/templates/hstringlist.cpp    -o build/hstringlist.o          $(INCLUDE) $(DEFINES)
	g++ -c src/png/convert.cpp                  -o build/convert.o             $(INCLUDE) $(DEFINES)
	g++ -c src/png/decodepng.cpp                  -o build/decodepng.o             $(INCLUDE) $(DEFINES)
	g++ -c src/png/decompress.cpp                  -o build/decompress.o             $(INCLUDE) $(DEFINES)
	g++ -c src/png/huffman.cpp                  -o build/huffman.o             $(INCLUDE) $(DEFINES)
	g++ -c src/png/inflateblock.cpp                  -o build/inflateblock.o             $(INCLUDE) $(DEFINES)
	g++ -c src/png/inflatedynamic.cpp                  -o build/inflatedynamic.o             $(INCLUDE) $(DEFINES)
	g++ -c src/png/inflator.cpp                  -o build/inflator.o             $(INCLUDE) $(DEFINES)
	g++ -c src/png/pngbasic.cpp                  -o build/pngbasic.o             $(INCLUDE) $(DEFINES)
	g++ -c src/png/unfilter.cpp                  -o build/unfilter.o             $(INCLUDE) $(DEFINES)
	g++ -c src/png/zlib.cpp                  -o build/zlib.o             $(INCLUDE) $(DEFINES)
	g++ -c src/tga/tga.cpp                  -o build/tga.o             $(INCLUDE) $(DEFINES)
	g++ -c src/tga/tgareader.cpp                  -o build/tgareader.o             $(INCLUDE) $(DEFINES)
	g++ -c src/bmploader.cpp                  -o build/bmploader.o             $(INCLUDE) $(DEFINES)
	g++ -c src/input.cpp                  -o build/input.o             $(INCLUDE) $(DEFINES)
	g++ -c src/pngtexture.cpp                  -o build/pngtexture.o             $(INCLUDE) $(DEFINES)
	g++ -c src/renderer.cpp                  -o build/renderer.o             $(INCLUDE) $(DEFINES)
	g++ -c src/Settings.cpp                  -o build/settings.o             $(INCLUDE) $(DEFINES)
	g++ -c src/testnode.cpp                  -o build/testnode.o             $(INCLUDE) $(DEFINES)
	g++ -c src/defaulttexloader.cpp                  -o build/defaulttextureloader.o             $(INCLUDE) $(DEFINES)
	g++ -c src/fontmanager.cpp                  -o  build/fontmanager.o      $(INCLUDE) $(DEFINES)
	g++ -c src/tmfont.cpp                       -o  build/tmfont.o           $(INCLUDE) $(DEFINES)
	g++ -c src/fontnode.cpp                     -o  build/fontnode.o         $(INCLUDE) $(DEFINES)
	g++ $(BUILD)                                -o  $(OUTPUT)    $(LDFLAGS)
clean:
	rm $(OUTPUT)
	rm -rf build
.PHONY: all clean
