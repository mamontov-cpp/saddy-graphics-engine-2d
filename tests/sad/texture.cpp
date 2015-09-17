#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
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
       TEST(SadTextureTest::testConvertToPot)
   ) {}
   
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

} _sad_texture_test;
