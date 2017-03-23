#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "matrix2x2.h"
#include "fuzzyequal.h"
#include "util/markup.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

struct MarkupTest : tpunit::TestFixture
{
public:
    MarkupTest() : tpunit::TestFixture(
        TEST(MarkupTest::testParseSize),
        TEST(MarkupTest::testParseLineSpacingSize),
        TEST(MarkupTest::testGetColorFromTable),
        TEST(MarkupTest::testParseHexRGBA),
        TEST(MarkupTest::testParseColor)
    ) {}


    void testParseSize()
    {
        sad::Maybe<sad::util::Markup::FontSize> empty;
        ASSERT_TRUE(sad::util::Markup::parseSize("", empty).exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseSize("p", empty).exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseSize("1", empty).value() == sad::util::Markup::FontSize(1, sad::util::Markup::MFZST_PIXELS));
        ASSERT_TRUE(sad::util::Markup::parseSize("322", empty).value() == sad::util::Markup::FontSize(322, sad::util::Markup::MFZST_PIXELS));
        ASSERT_TRUE(sad::util::Markup::parseSize("322p", empty).exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseSize("322px", empty).value() == sad::util::Markup::FontSize(322, sad::util::Markup::MFZST_PIXELS));
        ASSERT_TRUE(sad::util::Markup::parseSize("322pt", empty).value() == sad::util::Markup::FontSize(322, sad::util::Markup::MFZST_POINTS));
        ASSERT_TRUE(sad::util::Markup::parseSize("322xpx", empty).exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseSize("322xpt", empty).exists() == false);
    }

    void testParseLineSpacingSize()
    {
        sad::Maybe<sad::util::Markup::LineSpacingSize> empty;
        ASSERT_TRUE(sad::util::Markup::parseLineSpacingSize("", empty).exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseLineSpacingSize("p", empty).exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseLineSpacingSize("%", empty).exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseLineSpacingSize("1", empty).value() == sad::util::Markup::LineSpacingSize(1, sad::util::Markup::MLST_PIXELS));
        ASSERT_TRUE(sad::util::Markup::parseLineSpacingSize("322", empty).value() == sad::util::Markup::LineSpacingSize(322, sad::util::Markup::MLST_PIXELS));
        ASSERT_TRUE(sad::util::Markup::parseLineSpacingSize("322p", empty).exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseLineSpacingSize("322px", empty).value() == sad::util::Markup::LineSpacingSize(322, sad::util::Markup::MLST_PIXELS));
        ASSERT_TRUE(sad::util::Markup::parseLineSpacingSize("322%", empty).value() == sad::util::Markup::LineSpacingSize(322, sad::util::Markup::MLST_PERCENTS));
        ASSERT_TRUE(sad::util::Markup::parseLineSpacingSize("322xpx", empty).exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseLineSpacingSize("322x%", empty).exists() == false);
    }

    void testGetColorFromTable()
    {
        sad::Maybe<sad::AColor> r = sad::util::Markup::getColorFromTable("zomp");
        ASSERT_TRUE(r.exists());
        ASSERT_TRUE(r.value().r() == 57);
        ASSERT_TRUE(r.value().g() == 167);
        ASSERT_TRUE(r.value().b() == 142);
        
        r = sad::util::Markup::getColorFromTable("stupid non-existing color! I don\'t care for it anymore");
        ASSERT_TRUE(r.exists() == false);
    }

    
    void testParseHexRGBA()
    {
        ASSERT_TRUE(sad::util::Markup::parseHexRGBA("#").exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseHexRGBA("").exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseHexRGBA("aaaaaaa").exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseHexRGBA("#!pqrst").exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseHexRGBA("#!pqrstvx").exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseHexRGBA("#11aaff").exists());
        ASSERT_TRUE(sad::util::Markup::parseHexRGBA("#11aaff").value() == sad::AColor(17, 170, 255));
        ASSERT_TRUE(sad::util::Markup::parseHexRGBA("#FFFFFF").value() == sad::AColor(255, 255, 255));
        ASSERT_TRUE(sad::util::Markup::parseHexRGBA("#A0A0A0A0").value() == sad::AColor(160, 160, 160, 160));
        ASSERT_TRUE(sad::util::Markup::parseHexRGBA("#a0a0a0a0").value() == sad::AColor(160, 160, 160, 160));
    }

    void testParseColor()
    {
        sad::Maybe<sad::AColor> empty;
        ASSERT_TRUE(sad::util::Markup::parseColor("#", empty).exists() == false);
        ASSERT_TRUE(sad::util::Markup::parseColor("#A0A0A0", empty).exists());
        ASSERT_TRUE(sad::util::Markup::parseColor("#A0A0A0", empty).value() == sad::AColor(160, 160, 160));
        ASSERT_TRUE(sad::util::Markup::parseColor("#A0A0A0A0", empty).value() == sad::AColor(160, 160, 160, 160));
        ASSERT_TRUE(sad::util::Markup::parseColor("zomp", empty).exists());
        ASSERT_TRUE(sad::util::Markup::parseColor("zomp", empty).value() == sad::AColor(57, 167, 142));
    }

} test_markup;

