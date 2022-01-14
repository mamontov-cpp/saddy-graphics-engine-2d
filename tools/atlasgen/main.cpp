#include "atlas.h"
#include "xmlreader.h"
#include "jsonreader.h"
#include "fullsearchpacker/fullsearchpacker.h"
#include "growingbinpacker/growingbinpacker.h"
#include "horizontalpacker.h"
#include "xmlwriter.h"
#include "jsonwriter.h"
#include "outputoptions.h"

#include <math.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>

#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QByteArray>

#include <QtCore/QBuffer>
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QVector>
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QHash>
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QString>
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QVariant>

#define TAR7Z_SADDY

#include "../../include/3rdparty/tar7z/include/reader.h"
#include "../../include/3rdparty/tar7z/include/writer.h"

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

/*! A round() function replacement for C++0.3
    \param[in] f value
    \return
 */
inline float round03(float f)
{
    return floor(f + 0.5);
}

/*! Writes image as SRGBA to array
    \param[in] image an image
    \param[ou] arr an output array
 */
void writeSRGBA(QImage* image, QByteArray& arr)
{
    arr.append("SRGBA");
    unsigned char size = static_cast<unsigned char>(log(static_cast<double>(image->width())) / log(2.0));
    arr.append(reinterpret_cast<char*>(&size), 1);
    for(int x = 0; x < image->height(); x++)
    {
        for(int y = 0; y < image->width(); y++)
        {
            QRgb px = image->pixel(y, x);
            unsigned char pix[4] = {
                static_cast<unsigned char>(qRed(px)),
                static_cast<unsigned char>(qGreen(px)),
                static_cast<unsigned char>(qBlue(px)),
                static_cast<unsigned char>(qAlpha(px))
            };
            arr.append(reinterpret_cast<const char*>(pix), 4);
        }
    }
}

/*! Writes image as SR3G3B2 to array
    \param[in] image an image
    \param[ou] arr an output array
 */
void writeSR3G3B2(QImage* image, QByteArray& arr)
{
    arr.append("SR3G3B2");
    // We always generate base two sizes, so it's ok to case it
    unsigned char size = static_cast<unsigned char>(log(static_cast<double>(image->width())) / log(2.0));
    arr.append(reinterpret_cast<char*>(&size), 1);
    for(int x = 0; x < image->height(); x++)
    {
        for(int y = 0; y < image->width(); y++)
        {
            QRgb px = image->pixel(y, x);
            unsigned char pix[4] = {
                static_cast<unsigned char>(qRed(px)),
                static_cast<unsigned char>(qGreen(px)),
                static_cast<unsigned char>(qBlue(px)),
                static_cast<unsigned char>(qAlpha(px))
            };
            unsigned char nr = static_cast<unsigned char>(round03(static_cast<float>(pix[0]) / 255.0 * 7.0));
            unsigned char ng = static_cast<unsigned char>(round03(static_cast<float>(pix[1]) / 255.0 * 7.0));
            unsigned char nb = static_cast<unsigned char>(round03(static_cast<float>(pix[2]) / 255.0 * 3.0));
            unsigned char p = nb | ng << 2 | nr << 5;
            arr.append(reinterpret_cast<char*>(&p), 1);
        }
    }
}

/*! Writes image as SR5G6B5 to array
    \param[in] image an image
    \param[ou] arr an output array
 */
void writeSR5G6B5(QImage* image, QByteArray& arr)
{
    arr.append("SR5G6B5");
    // We always generate base two sizes, so it's ok to case it
    unsigned char size = static_cast<unsigned char>(log(static_cast<double>(image->width())) / log(2.0));
    arr.append(reinterpret_cast<char*>(&size), 1);
    for(int x = 0; x < image->height(); x++)
    {
        for(int y = 0; y < image->width(); y++)
        {
            QRgb px = image->pixel(y, x);
            unsigned char pix[4] = {
                static_cast<unsigned char>(qRed(px)),
                static_cast<unsigned char>(qGreen(px)),
                static_cast<unsigned char>(qBlue(px)),
                static_cast<unsigned char>(qAlpha(px))
            };
            unsigned char nr = static_cast<unsigned char>(round03(static_cast<float>(pix[0]) / 255.0 * 31.0));
            unsigned char ng = static_cast<unsigned char>(round03(static_cast<float>(pix[1]) / 255.0 * 63.0));
            unsigned char nb = static_cast<unsigned char>(round03(static_cast<float>(pix[2]) / 255.0 * 31.0));
            unsigned short p = nb | ng << 5 | nr << 11;
            arr.append(reinterpret_cast<char*>(&p), 2);
        }
    }
}

/*! Writes image as SR4G4B4A4 to array
    \param[in] image an image
    \param[ou] arr an output array
 */
void writeSR4G4B4A4(QImage* image, QByteArray& arr)
{
    arr.append("SR4G4B4A4");
    // We always generate base two sizes, so it's ok to case it
    unsigned char size = static_cast<unsigned char>(log(static_cast<double>(image->width())) / log(2.0));
    arr.append(reinterpret_cast<char*>(&size), 1);
    for(int x = 0; x < image->height(); x++)
    {
        for(int y = 0; y < image->width(); y++)
        {
            QRgb px = image->pixel(y, x);
            unsigned char pix[4] = {
                static_cast<unsigned char>(qRed(px)),
                static_cast<unsigned char>(qGreen(px)),
                static_cast<unsigned char>(qBlue(px)),
                static_cast<unsigned char>(qAlpha(px))
            };
            unsigned char nr = static_cast<unsigned char>(round03(static_cast<float>(pix[0]) / 255.0 * 15.0));
            unsigned char ng = static_cast<unsigned char>(round03(static_cast<float>(pix[1]) / 255.0 * 15.0));
            unsigned char nb = static_cast<unsigned char>(round03(static_cast<float>(pix[2]) / 255.0 * 15.0));
            unsigned char na = static_cast<unsigned char>(round03(static_cast<float>(pix[3]) / 255.0 * 15.0));

            unsigned short p = na | nb << 4 | ng << 8 | nr << 12;
            arr.append(reinterpret_cast<char*>(&p), 2);
        }
    }
}

/*! Writes texture to file
    \param[in] atlas atlas
    \param[in] image an image to be written
    \param[in] options output options
 */
void writeTexture(Atlas* atlas, QImage* image, OutputOptions& options)
{
    bool saved = false;
    QByteArray arr;

    // If we need to write image as srgba - write it to specified file
    if (options.TextureFileFormat == "srgba")
    {        
        writeSRGBA(image, arr);
    }
    else if (options.TextureFileFormat == "sr5g6b5")
    {
        writeSR5G6B5(image, arr);
    }
    else if (options.TextureFileFormat == "sr4g4b4a4")
    {
        writeSR4G4B4A4(image, arr);
    }
    else if (options.TextureFileFormat  == "sr3g3b2")
    {
        writeSR3G3B2(image, arr);
    }
    else
    {
        QBuffer buf(&arr);
        buf.open(QIODevice::WriteOnly);

        QString extension = sad::String(atlas->outputTexture().toStdString()).getExtension().c_str();
        extension = extension.toUpper();
        // We support only BMP and PNG formats
        if (extension != "BMP")
        {
            extension = "PNG";
        }
        saved = image->save(&buf, extension.toStdString().c_str());
    }

    if (options.ProgramOptions->contains("write-to-tar") && (options.Archive))
    {
        std::vector<char> v;
        v.insert(v.end(), arr.begin(), arr.end());
        QString name = OutputOptions::tar7zCompatibleName(atlas->outputTexture());
        options.Archive->add(name.toStdString(), v);
        saved = true;
    }
    else
    {
        QFile file(atlas->outputTexture());
        if (file.open(QIODevice::ReadWrite))
        {
            QDataStream stream(&file);
            stream.writeRawData(arr.data(), arr.length());
            saved = (stream.status() == QDataStream::Ok);
        }                
    }
    if (!saved)
    {
        printf("Can\'t write resulting texture to file %s\n", atlas->outputTexture().toStdString().c_str());
        options.ReturnCode = 5;
    }
}

/*! Tries to read tar into file, or create new file if failed
    \param[in, out] options for reading
 */
void tryPrepareForTarWriting(OutputOptions& options)
{
    if (options.ProgramOptions->contains("write-to-tar"))
    {
        QString tarfilename =  (*(options.ProgramOptions))["write-to-tar"].toString(); 
        QStringList parts = tarfilename.split("/");
        if (parts.size() <= 1)
        {
            parts = tarfilename.split("\\");                    
        }
        if (parts.size() == 0)
        {
            options.ProgramOptions->insert("short-tar-name",  QString());                    
        }
        else
        {
            options.ProgramOptions->insert("short-tar-name",  parts[parts.size() - 1]);
        }

        options.Archive = new tar7z::Archive();
        tar7z::Reader reader;
        if (reader.read(tarfilename.toStdString(), *(options.Archive)) != tar7z::Error::T7ZE_OK)
        {
            printf("%s\n", "Unable to read archive. Initializing new empty archive");
            delete options.Archive;
            options.ReturnCode = 5;
            options.Archive = new tar7z::Archive();
        }
    }
}

/*! Tries to write tar into file. Invalidates archive reference also.
    \param[in, out] options for reading
 */
void tryPerformTarWriting(OutputOptions& options)
{
    QHash<QString, QVariant>& program_options = *(options.ProgramOptions);
    if (program_options.contains("write-to-tar") && (options.Archive))
    {
        tar7z::Writer writer;
        QString tarfilename =  (*(options.ProgramOptions))["write-to-tar"].toString();
        tar7z::Error e = writer.write(tarfilename.toStdString(), *(options.Archive));
        if (e != tar7z::Error::T7ZE_OK)
        {
            options.ReturnCode = 6;
            printf("%s\n", "Unable to write archive");
        }
    }
    if (options.Archive)
    {
        delete options.Archive;
    }
}

/*! Replaces extension with give, if parameter optname is set
    \param[in] atlas atlas name
    \param[in] opts options
    \param[in] optname optname option
    \param[in] extension extension extension
 */
void tryReplaceExtension(Atlas& atlas, OutputOptions& opts, const QString& optname, const QString& extension)
{
     QHash<QString, QVariant>& program_options = *(opts.ProgramOptions);
    if (program_options[optname].value<bool>())
    {
        opts.TextureFileFormat = extension;
        QString outputname = atlas.outputTexture();
        if (outputname.toUpper().endsWith(extension.toUpper()) == false)
        {
            // Try to replace extension with new extension
            int pos = outputname.lastIndexOf(".");
            if (pos != -1)
            {
                outputname.remove(pos, outputname.size() - pos);
            }
            outputname.append(".").append(extension);
            atlas.setOutputTexture(outputname, true);
        }
    }
}

int main(int argc, char *argv[])
{
#ifndef _MSC_VER
#ifdef DEBUG_QT_ASSERT
    qInstallMsgHandler(crashMessageOutput);
#endif
#endif
    // A return code for program
    int return_code = 0;
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

    // Should we use SRGBA as output format
    program_options.insert("image-srgba", false);

    // Should we use SR5G6B5 as output format
    program_options.insert("image-sr5g6b6", false);

    // Should we use SR4G4B4A4 as output format
    program_options.insert("image-sr4g4b4a4", false);

    // Should we use SR3G3B2 (GL_UNSIGNED_BYTE_3_3_2)
    program_options.insert("image-sr3g3b2", false);

    // Should we add 1-pixel hack hack
    program_options.insert("add-pixel", false);

    // Should we disablie mip-maps in texture
    program_options.insert("no-mipmaps", false);

    // An input file
    QVector<QString> input_files;
    
    // Parse program options
    bool textures_should_be_unique = true;
    // Whether we need a full search
    bool full_search = false;
    // Whether we should layout all horizontally
    bool horizontal_layout = false;
    // Whether we should take options only from first file
    bool take_first = true;
    // Whether next argument is tar archive
    bool use_tar = false;
    
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
        if (argument == "-sr5g6b5" || argument == "--image-sr5g6b5")
        {
            handled = true;
            program_options["image-sr5g6b5"] = true;
        }
        if (argument == "-sr4g4b4a4" || argument == "--image-sr4g4b4a4")
        {
            handled = true;
            program_options["image-sr4g4b4a4"] = true;
        }
        if (argument == "-sr3g3b2" || argument == "--image-sr3g3b2")
        {
            handled = true;
            program_options["image-sr3g3b2"] = true;
        }
        if (argument == "-full-search" || argument == "--full-search")
        {
            handled = true;
            full_search = true;
        }
        if (argument == "-layout-horizontal" || argument == "--layout-horizontal")
        {
            handled = true;
            horizontal_layout = true;
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
        if (argument == "-add-pixel" || argument == "--add-pixel")
        {
            handled = true;
            program_options["add-pixel"] = true;
        }
        if (argument == "-do-not-add-pixel" || argument == "--do-not-add-pixel")
        {
            handled = true;
            program_options["add-pixel"] = false;
        }
        if (argument == "-no-mipmaps" || argument == "--no-mipmaps")
        {
            handled = true;
            program_options["no-mipmaps"] = true;
        }
        if (argument == "-write-to-tar" || argument == "--write-to-tar")
        {
            handled = true;
            use_tar = true;
        }
        if (!handled)
        {
            if (use_tar)
            {
                program_options["write-to-tar"] = argument;
                use_tar = false;
            }
            else
            {
                input_files.push_back(argument);
            }
        }
    }

    if ((program_options["help"].value<bool>() || input_files.empty()) 
        && program_options["run-tests"].value<bool>() == false)
    {
        if (program_options["help"].value<bool>() == false)
        {
            return_code = 1;
            printf("%s", "Error: no input file specified \n");
        } 
        else
        {
            return_code  = 2;
            printf("%s",
"Usage: atlasgen-<configuration> [-json|-xml] [options] <input-file>  \n\
Options:\n\
-json, --format-json (default) - parse input file as JSON \n\
-xml, --format-xml - parse input file as XML \n\
-with-index, --with-index - Scan and print index part in file definitions\n\
-h, --help - prints this help\n\
-srgba, --image-srgba - save image in SRGBA format\n"
"-sr5g6b5, --image-sr5g6b5 - save image in SR5G6B5 format\n"
"-sr4g4b4a4, --image-sr4g4b4a4 - save image in SR4G4B4A4 format\n"
"-sr3g3b2, --image-sr3g3b2 - save image in SR3G3B2 format\n"
"-non-unique-textures, --non-unique-textures - do not check, that textures should be unique\n"
"-take-options-from-last, --take-options-from-last - take options from the last file passed (default: off)\n"
"-add-pixel, --add-pixel - add one pixel boundary to textures to avoid rounding errors (default: off)\n"
"-do-not-add-pixel, --do-not-add-pixel - disable adding one pixel boundary to textures to avoid rounding errors (default: on)\n"
"-write-to-tar <filename>, --write-to-tar <filename> - write output to tar files\n"
"-no-mipmaps, --no-mipmaps - disable building mipmaps when loading texture\n"
"-layout-horizontal, --layout-horizontal - just layout textures horizontally. Usable for vertical tiling\n"
"-full-search, --full-search - use full search algorithm instead of growing bin for computing optimal arrangement"
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
            OutputOptions opts;
            opts.Archive = nullptr;
            opts.ProgramOptions = &program_options;
            opts.ReturnCode = 0;
            Atlas atlas;
            atlas.toggleFlagForChangingOutputPropertiesOnlyOnce(take_first);
            Reader* reader = nullptr;
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
                tryReplaceExtension(atlas, opts, "image-srgba", "srgba");
                // If we use SR5G6B5 as output image - check file extension and add here "sr5g6b5"
                tryReplaceExtension(atlas, opts, "image-sr5g6b5", "sr5g6b5");
                // If we use SR4G4B4A54 as output image - check file extension and add here "sr4g4b4a4"
                tryReplaceExtension(atlas, opts, "image-sr4g4b4a4", "sr4g4b4a4");
                // If we use SR3G3B2 as output image - check file extension and add here "sr3g3b2"
                tryReplaceExtension(atlas, opts, "image-sr3g3b2", "sr3g3b2");

                tryPrepareForTarWriting(opts);                
                if (!atlas.textures().empty())
                {
                    QImage* image;
                    Packer* packer;
                    if (full_search)
                    {
                        packer = new fullsearchpacker::FullSearchPacker();
                    }
                    else
                    {
                        if (horizontal_layout)
                        {
                            packer = new HorizontalPacker();
                        }
                        else
                        {
                            packer = new growingbinpacker::GrowingBinPacker();
                        }
                    }
                    packer->setOptions(&program_options);
                    packer->pack(atlas, image);
                    writeTexture(&atlas, image, opts);
                    delete image;
                    delete packer;
                }
                else
                {
                    QImage image(1, 1, QImage::Format_ARGB32);
                    image.fill(QColor(255, 255, 255, 0));
                    writeTexture(&atlas, &image, opts);
                }
                atlas.prepareForOutput(opts);

                Writer* writer = nullptr;
                if (program_options["format"].value<QString>() == "xml")
                {
                    writer = new XMLWriter();
                }
                if (program_options["format"].value<QString>() == "json")
                {
                    writer = new JSONWriter();
                }
                bool result = writer->write(atlas, opts);
                if (result == false || writer->errors().size() != 0)
                {
                    opts.ReturnCode = 4;
                    dumpErrors(writer);
                }
                else
                {
                    tryPerformTarWriting(opts);                     
                }

                return_code = opts.ReturnCode;

                delete writer;
            }
            else
            {
                return_code = 3;
                dumpErrors(reader);
            }
            delete reader;
        }
    }
    return return_code;
}

// Link sad::String statically
#include "../../src/sadstring.cpp"
// Link tar7z statically
#include "../../src/tar7z.cpp"
