#include "texture.h"
#ifndef TEXTURE_LOADER_TEST
#include "log/log.h"
#endif

#include <renderer.h>
#include <opengl.h>
#include <glcontext.h>

#include <os/glheaders.h>
#include <os/generatemipmaps30.h>

#include <pipeline/pipeline.h>

#include <resource/resourcefile.h>

#define TAR7Z_SADDY

#include <3rdparty/tar7z/include/archive.h>

#include <util/fs.h>
#include <util/deletetexturetask.h>
#include <util/texturedefaultimage.h>

#include <3rdparty/picojson/valuetotype.h>

// ================================ sad::Texture::Buffer implementation ================================

sad::Texture::Buffer::~Buffer()
{

}

// ================================ sad::Texture::DefaultBuffer implementation ================================

sad::Texture::DefaultBuffer::DefaultBuffer()
{
    
}

sad::uchar* sad::Texture::DefaultBuffer::buffer() const
{
    return Data.data();
}

sad::Texture::DefaultBuffer::~DefaultBuffer()
{
    
}


// ================================ sad::Texture::PointerBuffer implementation ================================

sad::Texture::PointerBuffer::PointerBuffer(sad::uchar* p) : Data(p)
{

}

sad::uchar* sad::Texture::PointerBuffer::buffer() const
{
    return Data;
}

sad::Texture::PointerBuffer::~PointerBuffer()
{
    delete Data;
}

// ================================ sad::Texture::Tar7zArchiveBuffer implementation ================================

sad::Texture::Tar7zArchiveBuffer::Tar7zArchiveBuffer() : Archive(NULL), Offset(0)
{
    
}

sad::uchar* sad::Texture::Tar7zArchiveBuffer::buffer() const
{
    if (!Archive)
    {
        return NULL;
    }
    return reinterpret_cast<unsigned char*>(const_cast<char *>(&(Archive->Contents[Offset])));
}

sad::Texture::Tar7zArchiveBuffer::~Tar7zArchiveBuffer()
{
    
}

// ================================ sad::Texture::DefaultImageBuffer implementation ================================

sad::Texture::DefaultImageBuffer::DefaultImageBuffer()
{
    
}

sad::uchar* sad::Texture::DefaultImageBuffer::buffer() const
{
    return const_cast<sad::uchar*>(sad::util::DefaultImage::Data);
}

sad::Texture::DefaultImageBuffer::~DefaultImageBuffer()
{
    
}

// ==================================== sad::Texture implementation ====================================
#ifndef TEXTURE_LOADER_TEST
DECLARE_SOBJ_INHERITANCE(sad::Texture, sad::resource::Resource);
#endif

sad::Texture::Texture() 
: BuildMipMaps(true), Buffer(new sad::Texture::DefaultBuffer()), Bpp(32), Format(sad::Texture::SFT_R8_G8_B8_A8), Width(0), Height(0), Id(0), OnGPU(false), m_renderer(NULL)
{

}

sad::Texture::~Texture()
{
#ifndef TEXTURE_LOADER_TEST 
    if (this->renderer() && OnGPU)
    {
        if (this->renderer()->context()->valid())
        {
            if (this->renderer()->isOwnThread())
            {
                this->unload();
            }
            else
            {
                if (this->renderer()->running())
                {
                    // Added since, constructor can generate exception
                    try
                    { 
                        this->renderer()->pipeline()->append(new  sad::util::DeleteTextureTask(Id));
                    }
                    catch(...)
                    {
                        
                    }
                }
            }
        }
    }
#endif
    delete Buffer;
}

void sad::Texture::upload()
{
#ifndef TEXTURE_LOADER_TEST 
    // We must not upload on our own to not cause
    // undefined behaviour
    sad::Renderer * r = renderer();
    if (!r || Width == 0 || Height == 0)
        return;

    OnGPU = true;
    
    // Get texture type and components
    GLuint opengl_format = GL_RGBA, opengl_internalformat = GL_RGBA, components = 4;
    GLenum opengl_type = GL_UNSIGNED_BYTE, opengl10_type = GL_UNSIGNED_BYTE;
    if (Bpp == 24)
    {
        opengl_internalformat = GL_RGB;
        opengl_format = GL_RGB;  
        components = 3;
    }
    switch(Format)
    {
    case sad::Texture::SFT_R5_G6_B5:
        opengl_format = GL_RGB;
        opengl_internalformat = GL_RGB;
        opengl_type = GL_UNSIGNED_SHORT_5_6_5;
        opengl10_type = GL_UNSIGNED_SHORT_5_6_5; // GLU counterpart for this doesn't seem to be defined on any platform
        components = 3;
        break;
    case sad::Texture::SFT_R3_G3_B2:
        opengl_format = GL_RGB;
        opengl_internalformat = GL_RGB;
        opengl_type = GL_UNSIGNED_BYTE_3_3_2;
        opengl10_type = GL_UNSIGNED_BYTE_3_3_2; // GLU counterpart for this doesn't seem to be defined on any platform
        components = 3;
        break;
    case sad::Texture::SFT_R4_G4_B4_A4:
        opengl_format = GL_RGBA;
        opengl_internalformat = GL_RGBA;
        opengl_type = GL_UNSIGNED_SHORT_4_4_4_4;
        opengl10_type = GL_UNSIGNED_SHORT_4_4_4_4; // GLU counterpart for this doesn't seem to be defined on any platform
        components = 4;
    default: break;
    };    

    if ((Width & (Width - 1)) != 0 || (Height & (Height - 1)) != 0 || Width != Height)
    {
        if (r->opengl()->supportsExtension("GL_ARB_texture_rectangle") == false
            || r->opengl()->supportsExtension("GL_ARB_texture_non_power_of_two") == false)
        {
            convertToPOTTexture();
        }
    }
    // Create ID of texture and bind it
    glGenTextures(1, static_cast<GLuint *>(&Id));
    glBindTexture(GL_TEXTURE_2D, Id);

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    if (!BuildMipMaps)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    }

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if (BuildMipMaps)
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    }
    else
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    // Actually upload image to GPU   
    GLint res;
    // ReSharper disable once CppEntityAssignedButNoRead
    unsigned char const * errorstring;
    sad::Pair<int,int> version = r->opengl()->version();
    if (version.p1() < 3) // In OpenGL 3.0  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);  is deprecated
    {
        if (version.p1() == 1 && version.p2() < 4)
        {
            // In case of OpenGL <1.4 there is not much we can do, so we ignore BuildMipMap flags
            res = gluBuild2DMipmaps(GL_TEXTURE_2D, components, Width, Height, opengl_format, opengl10_type, Buffer->buffer());
        }
        else
        {
            if (BuildMipMaps)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
            }
            glTexImage2D(GL_TEXTURE_2D, 0, opengl_internalformat, Width, Height, 0, opengl_format, opengl_type, Buffer->buffer());
            res = glGetError();
            // ReSharper disable once CppAssignedValueIsNeverUsed
            errorstring = gluErrorString(res);
        }
    } 
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, opengl_internalformat,  Width, Height, 0, opengl_format, opengl_type,  Buffer->buffer());     
        // ReSharper disable once CppAssignedValueIsNeverUsed
        res = glGetError();
        if (BuildMipMaps)
        {
            sad::os::generateMipMaps30(r, GL_TEXTURE_2D);
            GLint mipres = glGetError(); //-V519
            if (mipres)
            {
                SL_COND_LOCAL_INTERNAL(gluErrorString(mipres), r);
            }
        }
    }
    if (res)
    {
        SL_COND_LOCAL_INTERNAL(gluErrorString(res), r);
    }
#endif
}

void sad::Texture::loadDefaultTexture()
{
    Bpp = 32;
    Width = 64;
    Height = 64;
    
    delete Buffer;
    Buffer = new sad::Texture::DefaultImageBuffer();
}

bool sad::Texture::supportsLoadingFromTar7z() const
{
    return true;
}

void sad::Texture::unloadFromGPU()
{
    this->unload();
}

#ifndef TEXTURE_LOADER_TEST

bool sad::Texture::load(
        const sad::resource::ResourceFile & file,
        sad::Renderer * r,
        const picojson::value& options
)
{
    const sad::resource::ResourceFileIdentifier& ri  = file.rfi();
    bool result = false;
    if (ri.Valid)
    {   
        if (ri.Type == sad::resource::RFT_FILE)
        {
            result = load(ri.FileName, r);
            if (!result && !util::isAbsolutePath(ri.FileName))
            {
                sad::String newpath = util::concatPaths(r->executablePath(), ri.FileName);
                result = load(newpath, r);
            }
        }
        
        if (ri.Type == sad::resource::RFT_TAR7Z_INNER_FILE)
        {
            bool force_reload = picojson::get_property_or_default(options, "force_archive_reload", false);
            tar7z::Entry* e = file.tree()->archiveEntry(ri.ArchiveName, ri.FileName, force_reload);
            if (e)
            {
                result = this->load(e, r);
            }
        }
        
        if (result)
        {
            sad::Maybe<sad::Color> maybecolor = picojson::to_type<sad::Color>(
                picojson::get_property(options, "transparent")
            );
            if (maybecolor.exists())
            {
                this->setAlpha(255, maybecolor.value());
            }
            sad::Maybe<bool> maybenomips = picojson::to_type<bool>(
                picojson::get_property(options, "no-mipmaps")
            );
            if (maybenomips.exists())
            {
                this->BuildMipMaps = !(maybenomips.value());
            }
        }
    }
    return result;
}

bool sad::Texture::load(tar7z::Entry* e, sad::Renderer* r)
{
    if (!r)
    {
        r = sad::Renderer::ref();
    }
    m_renderer = r;
    sad::String ff(sad::String(e->Name).getExtension());
    char * f=const_cast<char *>(ff.data());
    while(*f) { *f=toupper(*f); ++f; }

    sad::imageformats::Loader * l = r->textureLoader(ff);
    return l->load(e, this);
}

bool sad::Texture::load(const sad::String & filename, sad::Renderer * r)
{
    if (!r)
    {
        r = sad::Renderer::ref();
    }
    m_renderer = r;
    sad::String ff(filename.getExtension());
    char * f=const_cast<char *>(ff.data());
    while(*f) { *f=toupper(*f); ++f; }

    sad::imageformats::Loader * l = r->textureLoader(ff);
    if (l)
    {
        FILE * fl = fopen(filename.data(), "rb");
        if (fl)
        {
            bool result = l->load(fl, this);
            fclose(fl);
            return result;
        }
    }
    return false;
}

bool sad::Texture::load(const sad::WString & filename, sad::Renderer * r)
{
    if (!r)
    {
        r = sad::Renderer::ref();
    }
    m_renderer = r;
    char * tmp=new char[2*filename.length()+2];
    wcstombs(tmp,filename.data(),2*filename.length()+2);
    sad::String tt(tmp);
    delete[] tmp;
    return load(tt, r);
}

#endif

void sad::Texture::bind()
{
#ifndef TEXTURE_LOADER_TEST
    if (!OnGPU)
        upload();
    glBindTexture(GL_TEXTURE_2D, Id);
#endif
}

void sad::Texture::unload()
{
#ifndef TEXTURE_LOADER_TEST
    if (OnGPU)
        glDeleteTextures(1, &Id);
    OnGPU = false;
#endif
}


void sad::Texture::setAlpha(sad::uchar a) const
{
    assert(Bpp == 32);
    size_t buffersize = Width * Height * (Bpp / 8);
    sad::uchar* bufbegin = Buffer->buffer();
    for (unsigned int i = 3 ; i < buffersize; i += 4)
    {
        bufbegin[i] = 255 - a;
    }
}

void sad::Texture::setAlpha(sad::uchar a, const sad::Color & clr) const
{
    assert(Bpp == 32);
    size_t buffersize = Width * Height * (Bpp / 8);
    sad::uchar* bufbegin = Buffer->buffer();
    for (unsigned int i = 0 ; i < buffersize; i += 4)
    {
        bool matches = bufbegin[i  ] == clr.r()
                    && bufbegin[i+1] == clr.g()
                    && bufbegin[i+2] == clr.b();
        if (matches)
        {
            bufbegin[i + 3] = 255 - a;
        }
    }
}

void sad::Texture::setAlpha(sad::uchar a, const sad::Color & clr,const sad::Rect2D & rect) const
{
    sad::Rect2I tmp = sad::_(rect);
    for (int i=0;i<4;i++)
    {
        if (tmp[i].x() < 0) tmp[i].setX(0);
        if (tmp[i].y() < 0) tmp[i].setY(0);
        if (tmp[i].x() > static_cast<int>(width()-1)) tmp[i].setX(this->width()-1);
        if (tmp[i].y() > static_cast<int>(height()-1)) tmp[i].setY(this->height()-1);
    }
    int minx = static_cast<int>(tmp[0].x());
    int maxx = static_cast<int>(tmp[2].x());
    int miny = static_cast<int>(tmp[0].y());
    int maxy = static_cast<int>(tmp[2].y());
    if (maxx < minx)
    {
        std::swap(maxx, minx);
    }
    if (maxy < miny)
    {
        std::swap(maxy, miny);
    }
    for (int row = minx;row <= maxx; row++)
    {
        for (int col = miny;col <= maxy; col++)
        {
            sad::uchar * pix=this->pixel(row,col);
            bool redmatches   = clr.r() == pix[0];
            bool greenmatches = clr.g() == pix[1];
            bool bluematches  = clr.b() == pix[2];
            if (redmatches && greenmatches && bluematches)
            {
                setPixelAlpha(row, col, a);
            }
        }
    }
}


sad::Renderer * sad::Texture::renderer() const
{
    return m_renderer;
}

void sad::Texture::convertToPOTTexture()
{
    unsigned int  maxside = std::max(Width, Height);
    unsigned int  size = 1;
    while(size < maxside)
    {
        size =  size << 1;
    }
    // Copy old buffer
    sad::Texture::DefaultBuffer* buffer = new sad::Texture::DefaultBuffer();
    std::vector<sad::uchar>& data = buffer->Data;
    
    // Resize buffer and fill it with zero
    data.resize(size * size * Bpp / 8, 0);  
    std::fill_n(data.begin(), size * size * Bpp / 8, 0);

    // Copy data from old buffer
    size_t rowsize = Width * (Bpp / 8);
    for(unsigned int x = 0; x < Height; x++)
    {
        sad::uchar* row = this->Buffer->buffer() + rowsize * x;
        memcpy(&(data[0]) + (size * (Bpp / 8) * x), row, rowsize * sizeof(sad::uchar));
    }
    Width = size;
    Height = size;

    delete this->Buffer;
    this->Buffer = buffer;
}
