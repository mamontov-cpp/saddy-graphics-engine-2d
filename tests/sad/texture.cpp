#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <cstring>
#define _INC_STDIO
#include "texture.h"
#include <renderer.h>
#include <log/log.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#define TAR7Z_SADDY
#include "3rdparty/tar7z/include/archive.h"
#include "3rdparty/tar7z/include/reader.h"
#pragma warning(pop)

/*!
 * Tests sad::Texture
 */
struct SadTextureTest : tpunit::TestFixture
{
 public:
    SadTextureTest() : tpunit::TestFixture(
        TEST(SadTextureTest::testConvertToPot),
        TEST(SadTextureTest::testLoadSRGBA),
        TEST(SadTextureTest::testLoadTGA24Compressed),
        TEST(SadTextureTest::testLoadTGA32Compressed),
        TEST(SadTextureTest::testLoadTGA32CompressedSmall),
        TEST(SadTextureTest::testLoadTGA32Uncompressed),
        TEST(SadTextureTest::testLoadBMP),
        TEST(SadTextureTest::testLoadPNG),
        TEST(SadTextureTest::testLoadSR3G3B2),
        TEST(SadTextureTest::testLoadSR5G6B5),
        TEST(SadTextureTest::testLoadSR4G4B4A4),
        TEST(SadTextureTest::testTarLoadTGA24Compressed),
        TEST(SadTextureTest::testTarLoadTGA32Compressed),
        TEST(SadTextureTest::testTarLoadTGA32CompressedSmall),
        TEST(SadTextureTest::testTarLoadTGA32Uncompressed),
        TEST(SadTextureTest::testTarLoadBMP),
        TEST(SadTextureTest::testTarLoadPNG),
        TEST(SadTextureTest::testTarLoadSRGBA),
        TEST(SadTextureTest::testTarLoadSR3G3B2),
        TEST(SadTextureTest::testTarLoadSR5G6B5),
        TEST(SadTextureTest::testTarLoadSR4G4B4A4),
        TEST(SadTextureTest::testTextureUploadError)
    ) {}
   
    // Test converting texture to POT-texture
    void testConvertToPot()
    {
        sad::uchar pixels[36] = 
        {
          12, 12, 12, 12,   12, 12, 12, 12,   12, 12, 12, 12,
          12, 12, 12, 12,   12, 12, 12, 12,   12, 12, 12, 12,
          12, 12, 12, 12,   12, 12, 12, 12,   12, 12, 12, 12
        };

        sad::Texture c;
        c.Bpp = 32;
        c.Width = 3;
        c.Height = 3;
        sad::Vector<sad::uchar>* data = &(reinterpret_cast<sad::Texture::DefaultBuffer*>(c.Buffer)->Data);
        for(int i = 0; i < 36; i++)
        {
           (*data) << pixels[i];
        }
        c.convertToPOTTexture();

        data = &(reinterpret_cast<sad::Texture::DefaultBuffer*>(c.Buffer)->Data);
        ASSERT_TRUE(c.Width == 4);
        ASSERT_TRUE(c.Height == 4);
        ASSERT_TRUE(data->size() == 64);

        sad::uchar newpixels[64] = 
        {
          12, 12, 12, 12,   12, 12, 12, 12,   12, 12, 12, 12,   0,  0,  0,  0,
          12, 12, 12, 12,   12, 12, 12, 12,   12, 12, 12, 12,   0,  0,  0,  0,
          12, 12, 12, 12,   12, 12, 12, 12,   12, 12, 12, 12,   0,  0,  0,  0,
           0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0,   0,  0,  0,  0         
        };
        for(int i = 0; i < 64; i++)
        {
           ASSERT_TRUE((*data)[i] == newpixels[i]);
        }
    }
   
    void testLoadSRGBA()
    {
        sad::Texture c;
        ASSERT_TRUE( c.load("examples/test.srgba") ) ;
        ASSERT_TRUE( c.Bpp == 32 );
        ASSERT_TRUE( c.Width == 4 );
        ASSERT_TRUE( c.Height == 4 );
        sad::Vector<sad::uchar>& vdata = reinterpret_cast<sad::Texture::DefaultBuffer*>(c.Buffer)->Data;
        ASSERT_TRUE( vdata.size() == 64 );
        
        const sad::uchar tmp[8] = { '1', '2', '3', '4', '5', '6', '7', '8' }; 
        const sad::uchar* data = &(vdata[0]);
        for(size_t i = 0; i < 8; i++)
        {
            ASSERT_TRUE(memcmp(data + i * 8, tmp, 8) == 0 );
        }
    }
    
    void testLoadTGA24Compressed()
    {
        sad::Texture c;
        ASSERT_TRUE( c.load("tests/images/tga24_compressed.tga") ) ;
    }

    void testLoadTGA32Compressed()
    {
        sad::Texture c;
        ASSERT_TRUE( c.load("tests/images/tga32_compressed_small.tga") ) ;
    }

    void testLoadTGA32CompressedSmall()
    {
        sad::Texture c;
        ASSERT_TRUE( c.load("tests/images/tga32_compressed.tga") ) ;
    }

    void testLoadTGA32Uncompressed()
    {
        sad::Texture c;
        ASSERT_TRUE( c.load("tests/images/tga32_uncompressed.tga") ) ;
    }
    
    void testLoadBMP()
    {
        sad::Texture c;
        ASSERT_TRUE( c.load("tests/images/bmp.bmp") ) ;
    }

    void testLoadPNG()
    {
        sad::Texture c;
        ASSERT_TRUE( c.load("tests/images/png.png") ) ;
    }

    void testLoadSR3G3B2()
    {
        sad::Texture c;
        ASSERT_TRUE( c.load("tests/images/nice.sr3g3b2") ) ;
    }

    void testLoadSR5G6B5()
    {
        sad::Texture c;
        ASSERT_TRUE( c.load("tests/images/nice.sr5g6b5") ) ;
    }

    void testLoadSR4G4B4A4()
    {
        sad::Texture c;
        ASSERT_TRUE( c.load("tests/images/nice.sr4g4b4a4") ) ;
    }
    
    void testTarLoadTGA24Compressed()
    {
        sad::Texture c;
        tar7z::Archive a;
        tar7z::Reader r;
        ASSERT_TRUE( r.read("tests/images/images.tar", a) == tar7z::T7ZE_OK );
        tar7z::Entry* e = a.file("tga24_compressed.tga");
        ASSERT_TRUE(e);
        ASSERT_TRUE( c.load(e) );
    }
    
    void testTarLoadTGA32Compressed()
    {
        sad::Texture c;
        tar7z::Archive a;
        tar7z::Reader r;
        ASSERT_TRUE( r.read("tests/images/images.tar", a) == tar7z::T7ZE_OK );
        tar7z::Entry* e = a.file("tga32_compressed.tga");
        ASSERT_TRUE(e);
        ASSERT_TRUE( c.load(e) );
    }
    
    void testTarLoadTGA32CompressedSmall()
    {
        sad::Texture c;
        tar7z::Archive a;
        tar7z::Reader r;
        ASSERT_TRUE( r.read("tests/images/images.tar", a) == tar7z::T7ZE_OK );
        tar7z::Entry* e = a.file("tga32_compressed_small.tga");
        ASSERT_TRUE(e);
        ASSERT_TRUE( c.load(e) );
    }
    
    void testTarLoadTGA32Uncompressed()
    {
        sad::Texture c;
        tar7z::Archive a;
        tar7z::Reader r;
        ASSERT_TRUE( r.read("tests/images/images.tar", a) == tar7z::T7ZE_OK );
        tar7z::Entry* e = a.file("tga32_uncompressed.tga");
        ASSERT_TRUE(e);
        ASSERT_TRUE( c.load(e) );
    }
    
    void testTarLoadBMP()
    {
        sad::Texture c;
        tar7z::Archive a;
        tar7z::Reader r;
        ASSERT_TRUE( r.read("tests/images/images.tar", a) == tar7z::T7ZE_OK );
        tar7z::Entry* e = a.file("bmp.bmp");
        ASSERT_TRUE(e);
        ASSERT_TRUE( c.load(e) );
    }

    void testTarLoadPNG()
    {
        sad::Texture c;
        tar7z::Archive a;
        tar7z::Reader r;
        ASSERT_TRUE( r.read("tests/images/images.tar", a) == tar7z::T7ZE_OK );
        tar7z::Entry* e = a.file("png.png");
        ASSERT_TRUE(e);
        ASSERT_TRUE( c.load(e) );
    }

    void testTarLoadSRGBA()
    {
        sad::Texture c;
        tar7z::Archive a;
        tar7z::Reader r;
        ASSERT_TRUE( r.read("tests/images/images.tar", a) == tar7z::T7ZE_OK );
        tar7z::Entry* e = a.file("nice.srgba");
        ASSERT_TRUE(e);
        ASSERT_TRUE( c.load(e) );
    }

    void testTarLoadSR3G3B2()
    {
        sad::Texture c;
        tar7z::Archive a;
        tar7z::Reader r;
        ASSERT_TRUE( r.read("tests/images/images.tar", a) == tar7z::T7ZE_OK );
        tar7z::Entry* e = a.file("nice.sr3g3b2");
        ASSERT_TRUE(e);
        ASSERT_TRUE( c.load(e) );
    }

    void testTarLoadSR5G6B5()
    {
        sad::Texture c;
        tar7z::Archive a;
        tar7z::Reader r;
        ASSERT_TRUE( r.read("tests/images/images.tar", a) == tar7z::T7ZE_OK );
        tar7z::Entry* e = a.file("nice.sr5g6b5");
        ASSERT_TRUE(e);
        ASSERT_TRUE( c.load(e) );
    }

    void testTarLoadSR4G4B4A4()
    {
        sad::Texture c;
        tar7z::Archive a;
        tar7z::Reader r;
        ASSERT_TRUE( r.read("tests/images/images.tar", a) == tar7z::T7ZE_OK );
        tar7z::Entry* e = a.file("nice.sr4g4b4a4");
        ASSERT_TRUE(e);
        ASSERT_TRUE( c.load(e) );
    }

    void testTextureUploadError()
    {
        class RecoverableLogTarget : public sad::log::Target
        {
        public:
            RecoverableLogTarget()
            {

            }

            virtual void receive(const sad::log::Message & message)
            {
                m_messages.push_back(message.message());
                if (message.file())
                {
                    m_files.push_back(message.file());
                }
                else
                {
                    m_files.push_back("");
                }
            }

            const sad::Vector<sad::String>& messages() const
            {
                return m_messages;
            }

            const sad::Vector<sad::String>& files() const
            {
                return m_files;
            }
        protected:
            sad::Vector<sad::String> m_messages;
            sad::Vector<sad::String> m_files;
        };
        sad::Renderer r;
        RecoverableLogTarget* t = new RecoverableLogTarget();
        r.log()->addTarget(t);
        sad::uchar pixels[16] =
        {
            12, 12, 12, 12,   12, 12, 12, 12,
            12, 12, 12, 12,   12, 12, 12, 12,
        };

        sad::Texture c;
        c.Bpp = 32;
        c.Width = 2;
        c.Height = 2;
        sad::Vector<sad::uchar>* data = &(reinterpret_cast<sad::Texture::DefaultBuffer*>(c.Buffer)->Data);
        for (int i = 0; i < 16; i++)
        {
            (*data) << pixels[i];
        }
        c.setRenderer(&r);
        c.upload();
        const sad::Vector<sad::String>& messages = t->messages();
        const sad::Vector<sad::String>& files = t->files();
        ASSERT_TRUE(messages.size() > 0);
        bool found_unhandled_error = false;
        for (size_t i = 0; i < files.size(); i++)
        {
            if (files[i].getOccurence("texture.cpp") >= 0)
            {
                if (messages[i].length() >  0)
                {
                    found_unhandled_error = true;
                }
            }
        }
        ASSERT_TRUE(found_unhandled_error);
    }

} _sad_texture_test;
