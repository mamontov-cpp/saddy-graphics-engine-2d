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
        TEST(MarkupTest::testParseSize)
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

} test_markup;

