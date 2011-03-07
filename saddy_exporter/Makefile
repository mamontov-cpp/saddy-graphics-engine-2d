LDFLAGS=  -lm -lcairo -lpango-1.0 -lpangocairo-1.0 
INCLUDE=  -Iprimitives -Itemplates -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/pango-1.0/pango -I/usr/include/pango-1.0
OUTPUT   = saddy-exporter
DEFINES  = -DUNIX -DLINUX -DGCC
all:
	g++  saddy_exporter/saddy_exporter.cpp pngimage.cpp templates/hstring.cpp templates/hstringlist.cpp  primitives/hcolor.cpp primitives/hfont.cpp primitives/hline.cpp primitives/hlinestyle.cpp primitives/hpolyline.cpp primitives/hshape.cpp primitives/hshapestyle.cpp primitives/hxyrect.cpp -o $(OUTPUT)  $(INCLUDE) $(DEFINES) $(LDFLAGS) `pkg-config --libs glib-2.0`
