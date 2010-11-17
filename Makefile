LDFLAGS= -lm -lGL -lGLU
INCLUDE= -Iinclude -Iinclude/primitives -Iinclude/templates
OUTPUT   = saddy-test
DEFINES  = -DUNIX -DGCC
BUILD       = build/log.o build/main.o build/scene.o build/texture.o build/texturemanager.o build/hcolor.o build/hhash.o build/hstring.o build/hstringlist.o
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
	g++ -c src/templates/hstringlist.cpp    -o build/hstringlist.o          $(INCLUDE) $(DEFINES)
	g++ $(BUILD) -o $(OUTPUT) $(LDLFLAGS)
clean:
	rm $(OUTPUT)
	rm -rf build
.PHONY: all clean
