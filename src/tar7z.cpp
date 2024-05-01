#include "../include/3rdparty/tar7z.h"

// Embed tar7z library, so it would not be required to link against

#include "../include/3rdparty/tar7z/src/archive.cpp"
#include "../include/3rdparty/tar7z/src/entry.cpp"
#include "../include/3rdparty/tar7z/src/reader.cpp"
#include "../include/3rdparty/tar7z/src/writer.cpp"
