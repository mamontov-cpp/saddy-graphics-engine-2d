#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <cstring>
#define _INC_STDIO
#include "texture.h"
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

/*!
 * Tests sad::Texture
 */
struct SadTextureTest : tpunit::TestFixture
{
 public:
    SadTextureTest() : tpunit::TestFixture(
        TEST(SadTextureTest::testConvertToPot),
        TEST(SadTextureTest::testLoadSRGBA)    
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
        for(int i = 0; i < 36; i++)
        {
           c.Data << pixels[i];
        }
        c.convertToPOTTexture();

        ASSERT_TRUE(c.Width == 4);
        ASSERT_TRUE(c.Height == 4);
        ASSERT_TRUE(c.Data.size() == 64);

        sad::uchar newpixels[64] = 
        {
          12, 12, 12, 12,   12, 12, 12, 12,   12, 12, 12, 12,   0,  0,  0,  0,
          12, 12, 12, 12,   12, 12, 12, 12,   12, 12, 12, 12,   0,  0,  0,  0,
          12, 12, 12, 12,   12, 12, 12, 12,   12, 12, 12, 12,   0,  0,  0,  0,
           0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0,   0,  0,  0,  0         
        };
        for(int i = 0; i < 64; i++)
        {
           ASSERT_TRUE(c.Data[i] == newpixels[i]);
        }
    }
   
    void testLoadSRGBA()
    {
        sad::Texture c;
        ASSERT_TRUE( c.load("examples/test.srgba") ) ;
        ASSERT_TRUE( c.Bpp == 32 );
        ASSERT_TRUE( c.Width == 4 );
        ASSERT_TRUE( c.Height == 4 );
        ASSERT_TRUE( c.Data.size() == 64 );
        
        const sad::uchar tmp[8] = { '1', '2', '3', '4', '5', '6', '7', '8' }; 
        const sad::uchar* data = &(c.Data[0]);
        for(size_t i = 0; i < 8; i++)
        {
            ASSERT_TRUE(memcmp(data + i * 8, tmp, 8) == 0 );
        }
    }

} _sad_texture_test;
