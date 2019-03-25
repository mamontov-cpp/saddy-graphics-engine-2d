/*! \file bmploader.h
    

    Defines a loader for BMP images
 */
#pragma once
#include "loader.h"

#include <streambuf>
#include <vector>
#include <string>

namespace sad
{

namespace imageformats
{
/*! Defines a loader for BMP file format
 */
class BMPLoader: public sad::imageformats::Loader
{
public:
    /*! Loads a texture from file stream. file must be opened in binary format for reading
        \param[in] file
        \param[in] texture
        \return true on success
     */
    virtual bool load(FILE * file, sad::Texture * texture);
    /*! Loads texture from archive entry.
        \param[in] entry a file entry to be loaded
        \param[in] texture a source texture
     */
    virtual bool load(tar7z::Entry* entry, sad::Texture* texture);
    /*! Kept for purpose of inheritance
     */
    virtual ~BMPLoader();
protected:
    /*! Loads a texture from readable buffer
        \param[in] file
        \param[in] texture
        \return true on success
     */
    virtual bool load(std::streambuf * file, sad::Texture * texture);
};
/*! Dumps components to BMP
 *  \param[in] width  a width for texture
 *  \param[in] height a height for texture
 *  \param[in] components a components for loade (must be 3 or 4)
 *  \param[in] texture a texture data
 *  \return fully serializable to file data
 */
std::vector<unsigned char> dumpToBMP(unsigned int width, unsigned int height, int components, void* texture);

/*! Dumps components to BMP
 *  \param[in] file_name name of file
 *  \param[in] width  a width for texture
 *  \param[in] height a height for texture
 *  \param[in] components a components for loade (must be 3 or 4)
 *  \param[in] texture a texture data
 *  \return whether file was serialized
 */
bool dumpToBMP(const std::string& file_name, unsigned int width, unsigned int height, int components, void* texture);

}

}
