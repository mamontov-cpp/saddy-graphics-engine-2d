#include "atlas.h"
#include "xmlreader.h"
#include "jsonreader.h"
#include "fullsearchpacker/fullsearchpacker.h"
#include "growingbinpacker/growingbinpacker.h"
#include "xmlwriter.h"
#include "jsonwriter.h"


#include <cstdio>

#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QByteArray>


// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QVector>
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

/*! Dumps errors of object
    \param[in] object an object, which holds an errors
 */
template<
    typename T
>
void dumpErrors(T* object)
{
    for(size_t i = 0; i < object->errors().size(); i++)
    {
        printf("%s\n", object->errors()[i].toStdString().c_str());
    }
}

/*! Writes texture to file
    \param[in] atlas atlas
    \param[in] image an image to be written
    \param[in] program_options program options
 */
void writeTexture(Atlas* atlas, QImage* image, const QHash<QString, QVariant>& program_options)
{
    bool saved = false;
    // If we need to write image as srgba - write it to specified file
    if (program_options["image-srgba"].value<bool>())
    {
        QFile file(atlas->outputTexture());
        if (file.open(QIODevice::ReadWrite))
        {
            QDataStream stream(&file);
            QByteArray arr;
            arr.append("SRGBA");
            // We always generate base two sizes, so it's ok to case it
            unsigned char size = static_cast<unsigned char>(log2f(image->width()));
            arr.append(reinterpret_cast<char*>(&size), 1);
            for(int x = 0; x < image->height(); x++)
            {
                for(int y = 0; y < image->width(); y++)
                {
                    QRgb px = image->pixel(x, y);
                    unsigned char pix[4] = {
                        static_cast<unsigned char>(qRed(px)),
                        static_cast<unsigned char>(qGreen(px)),
                        static_cast<unsigned char>(qBlue(px)),
                        static_cast<unsigned char>(qAlpha(px))
                    };
                    arr.append(reinterpret_cast<char*>(&pix), 4);
                }
            }
            stream.writeRawData(arr.data(), arr.length());
            saved = (stream.status() == QDataStream::Ok);
        }
    }
    else
    {
        saved = image->save(atlas->outputTexture());
    }
    if (!saved)
    {
        printf("Can\'t write resulting texture to file %s\n", atlas->outputTexture().toStdString().c_str());
    }
}

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

    // Shoul we use SRGBA as output format
    program_options.insert("image-srgba", false);

    // An input file
    QVector<QString> input_files;
    
    // Parse program options
    bool textures_should_be_unique = true;
    // Whether we need a full search
    bool full_search = false;
    // Whether we should take options only from first file
    bool take_first = true;
    
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
        if (argument == "-srgba" || argument == "--image-srgba")
        {
            handled = true;
            program_options["image-srgba"] = true;
        }
        if (argument == "-full-search" || argument == "--full-search")
        {
            handled = true;
            full_search = true;
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
        if (argument == "-take-options-from-last" || argument == "--take-options-from-last")
        {
            handled = true;
            take_first = false;			
        }		
        if (!handled)
        {
            input_files.push_back(argument);
        }
    }

    if ((program_options["help"].value<bool>() || input_files.size() == 0) 
        && program_options["run-tests"].value<bool>() == false)
    {
        if (program_options["help"].value<bool>() == false)
        {
            printf("%s", "Error: no input file specified \n");
        } 
        else
        {
            printf("%s",
"Usage: atlasgen-<configuration> [-json|-xml] [-with-index] input  \n\
-json, --format-json (default) - parse input file as JSON \n\
-xml, --format-xml - parse input file as XML \n\
-with-index, --with-index - Scan and print index part in file definitions\n\
-h, --help - prints this help\n\
-srgba, --image-srgba - save image to SRGBA \n"
"-non-unique-textures, --non-unique-textures - do not check, that textures should be unique\n"
"-take-options-from-last, --take-options-from-last - take options from the last file passed (default: off)\n"                   
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
            atlas.toggleFlagForChangingOutputPropertiesOnlyOnce(take_first);
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
            for(size_t i = 0; i < input_files.size(); i++) {
                reader->read(input_files[i], &atlas);
            }
            if (reader->ok() && reader->errors().size() == 0)
            {
                // If we use SRGBA as output image - check file extension and add here "srgba"
                if (program_options["image-srgba"].value<bool>())
                {
                    QString outputname = atlas.outputTexture();
                    if (outputname.toUpper().endsWith("SRGBA") == false)
                    {
                        // Try to replace extension with "srgba"
                        int pos = outputname.lastIndexOf(".");
                        if (pos != -1)
                        {
                            outputname.remove(pos, outputname.size() - pos);
                        }
                        outputname.append(".srgba");
                        atlas.setOutputTexture(outputname, true);
                    }
                }

                if (atlas.textures().size() != 0)
                {
                    QImage* image;
                    Packer* packer;
                    if (full_search)
                    {
                        packer = new fullsearchpacker::FullSearchPacker();
                    }
                    else
                    {
                        packer = new growingbinpacker::GrowingBinPacker();
                    }
                    packer->pack(atlas, image);
                    writeTexture(&atlas, image, program_options);
                    delete image;
                    delete packer;
                }
                else
                {
                    QImage image(1, 1, QImage::Format_ARGB32);
                    image.fill(QColor(255, 255, 255, 0));
                    writeTexture(&atlas, &image, program_options);
                }
                atlas.prepareForOutput();

                Writer* writer = NULL;
                if (program_options["format"].value<QString>() == "xml")
                {
                    writer = new XMLWriter();
                }
                if (program_options["format"].value<QString>() == "json")
                {
                    writer = new JSONWriter();
                }
                bool result = writer->write(atlas, program_options["with-index"].value<bool>());
                if (result == false || writer->errors().size() != 0)
                {
                    dumpErrors(writer);
                }

                delete writer;
            }
            else
            {
                dumpErrors(reader);
            }
            delete reader;
        }
    }
    return 0;
}

// Link sad::String statically
#include "../../src/sadstring.cpp"
