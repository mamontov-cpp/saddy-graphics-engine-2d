#include "atlas.h"
#include "xmlreader.h"
#include "jsonreader.h"
#include "fullsearchpacker/fullsearchpacker.h"
#include "xmlwriter.h"
#include "jsonwriter.h"


#include <cstdio>

// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QHash>
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QString>
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QVariant>


#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#define _INC_STDIO
#include "../../include/3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

#ifndef _MSC_VER

// Taken from: http://qt-project.org/forums/viewthread/13148
// debugging asserts in Qt code is tricky with MinGW. You get a crash, instead of a backtrace.
// enable the define below to get a crash that results in a backtrace instead. Note that it does
// mess up your debug output, so don't leave it enabled if you're not working on fixing an assert
//#define DEBUG_QT_ASSERT

#ifdef DEBUG_QT_ASSERT
void crashMessageOutput(QtMsgType type, const char *msg)
 {
     switch (type) {
     case QtDebugMsg:
         fprintf(stderr, "Debug: %s\n", msg);
         break;
     case QtWarningMsg:
         fprintf(stderr, "Warning: %s\n", msg);
         break;
     case QtCriticalMsg:
         fprintf(stderr, "Critical: %s\n", msg);
         break;
     case QtFatalMsg:
         fprintf(stderr, "Fatal: %s\n", msg);
         __asm("int3");
         abort();
     }
 }
#endif

#endif

int main(int argc, char *argv[])
{
#ifndef _MSC_VER
#ifdef DEBUG_QT_ASSERT
    qInstallMsgHandler(crashMessageOutput);
#endif
#endif
    // An options for program
    QHash<QString, QVariant>  program_options;
    // An output format - JSON or XML
    program_options.insert("format", "json");

    // Should we scan and analyse index
    program_options.insert("with-index", false);

    // Should we display help
    program_options.insert("help", false);

    // Should we run tests
    program_options.insert("run-tests", false);

    // An input file
    program_options.insert("input", "");
    // Parse program options
	bool textures_should_be_unique = true;
    for(int i = 1; i < argc; i++)
    {
        QString argument(argv[i]);
        bool handled = false;
        if (argument == "-json" || argument == "--format-json")
        {
            handled = true;
            program_options["format"] = "json";
        }
        if (argument == "-xml" || argument == "--format-xml")
        {
            handled = true;
            program_options["format"] = "xml";
        }
        if (argument == "-with-index" || argument == "--with-index")
        {
            handled = true;
            program_options["with-index"] = true;
        }
        if (argument == "-h" || argument == "--help")
        {
            handled = true;
            program_options["help"] = true;
        }
        if (argument == "-run-tests" || argument == "--run-tests")
        {
            handled = true;
            program_options["run-tests"] = true;
        }	
		if (argument == "-non-unique-textures" || argument == "--non-unique-textures")
		{
			handled = true;
            textures_should_be_unique = false;			
		}
        if (!handled)
        {
            program_options["input"] = argument;
        }
    }

    if ((program_options["help"].value<bool>() || program_options["input"].value<QString>().length() == 0) 
        && program_options["run-tests"].value<bool>() == false)
    {
        if (program_options["help"].value<bool>() == false)
        {
            printf("Error: no input file specified \n");
        } 
        else
        {
            printf(
"Usage: atlasgen-<configuration> [-json|-xml] [-with-index] input  \n\
-json, --format-json (default) - parse input file as JSON \n\
-xml, --format-xml - parse input file as XML \n\
-with-index, --with-index - Scan and print index part in file definitions\n\
-h, --help - prints this help\n"
"-non-unique-textures, --non-unique-textures - do not check, that textures should be unique\n"
                  );
        }
    } 
    else
    {
        if (program_options["run-tests"].value<bool>())
        {
            tpunit::Tests::Run();
        }
        else
        {
            Atlas atlas;
            Reader* reader = NULL;
            if (program_options["format"].value<QString>() == "xml")
            {
                reader = new XMLReader();
            }
            if (program_options["format"].value<QString>() == "json")
            {
                reader = new JSONReader();
            }
			reader->toggleShouldPreserveUniqueTextures(textures_should_be_unique);
            reader->Result = &atlas;
            reader->read(program_options["input"].value<QString>());
            if (reader->Successfull && atlas.Errors.size() == 0)
            {
                if (atlas.Textures.size() != 0)
                {
                    QImage* image;
					Packer* packer;
					packer = new fullsearchpacker::FullSearchPacker();
					packer->pack(atlas, image);
                    bool saved = image->save(reader->OutputTexture);
                    if (!saved)
                    {
                        printf("Can\'t write resulting texture to file %s\n", reader->OutputTexture.toStdString().c_str());
                    }
					delete image;
					delete packer;
                }
                else
                {
                    QImage image(1, 1, QImage::Format_ARGB32);
                    image.fill(QColor(255, 255, 255, 0));
                    bool saved = image.save(reader->OutputTexture);
                    if (!saved)
                    {
                        printf("Can\'t write resulting texture to file %s\n", reader->OutputTexture.toStdString().c_str());
                    }
                }
                atlas.prepareForOutput(reader->OutputTexture);

                Writer* writer = NULL;
                if (program_options["format"].value<QString>() == "xml")
                {
                    writer = new XMLWriter();
                }
                if (program_options["format"].value<QString>() == "json")
                {
                    writer = new JSONWriter();
                }
                bool result = writer->write(atlas, reader->OutputName, reader->OutputTexture, program_options["with-index"].value<bool>());
                if (result == false || writer->Errors.size() != 0)
                {
                    for(size_t i = 0; i < writer->Errors.size(); i++)
                    {
                        printf("%s\n", writer->Errors[i].toStdString().c_str());
                    }
                }

                delete writer;
            }
            else
            {
                for(size_t i = 0; i < atlas.Errors.size(); i++)
                {
                    printf("%s\n", atlas.Errors[i].toStdString().c_str());
                }
            }
            delete reader;
        }
    }
    return 0;
}

// Link sad::String statically
#include "../../src/sadstring.cpp"
