#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include "resource/resourcefile.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


/*!
 * Tests sad::resource::Link
 */
struct SadResourceFileIdentifierTest : tpunit::TestFixture
{
 public:
    SadResourceFileIdentifierTest() : tpunit::TestFixture(
        TEST(SadResourceFileIdentifierTest::testParseIdentifierCommon),
        TEST(SadResourceFileIdentifierTest::testParseIdentifierInvalid),
        TEST(SadResourceFileIdentifierTest::testParseIdentifierValid)
    ) {}


    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testParseIdentifierCommon()
    {
        sad::resource::ResourceFileIdentifier ri;
        sad::resource::ResourceFileIdentifier::parse("t", ri);
        ASSERT_TRUE(ri.Valid);
        ASSERT_TRUE(ri.Type == sad::resource::ResourceFileType::RFT_FILE);
        ASSERT_TRUE(ri.FileName == "t");

        sad::resource::ResourceFileIdentifier::parse("tar", ri);
        ASSERT_TRUE(ri.Valid);
        ASSERT_TRUE(ri.Type == sad::resource::ResourceFileType::RFT_FILE);
        ASSERT_TRUE(ri.FileName == "tar");

        sad::resource::ResourceFileIdentifier::parse("D:/tar/1.tar", ri);
        ASSERT_TRUE(ri.Valid);
        ASSERT_TRUE(ri.Type == sad::resource::ResourceFileType::RFT_FILE);
        ASSERT_TRUE(ri.FileName == "D:/tar/1.tar");
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testParseIdentifierInvalid()
    {
        sad::resource::ResourceFileIdentifier ri;
        sad::resource::ResourceFileIdentifier::parse("tar7z:", ri);
        ASSERT_TRUE(ri.Valid == false);

        sad::resource::ResourceFileIdentifier::parse("tar7z:9", ri);
        ASSERT_TRUE(ri.Valid == false);

        sad::resource::ResourceFileIdentifier::parse("tar7z:9:", ri);
        ASSERT_TRUE(ri.Valid == false);

        sad::resource::ResourceFileIdentifier::parse("tar7z:9:/", ri);
        ASSERT_TRUE(ri.Valid == false);

        sad::resource::ResourceFileIdentifier::parse("tar7z:9://", ri);
        ASSERT_TRUE(ri.Valid == false);

        sad::resource::ResourceFileIdentifier::parse("tar7z:9://fail", ri);
        ASSERT_TRUE(ri.Valid == false);

        sad::resource::ResourceFileIdentifier::parse("tar7z:9://ftest.tar", ri);
        ASSERT_TRUE(ri.Valid == false);

        sad::resource::ResourceFileIdentifier::parse("tar7z:9://ftest.tar/", ri);
        ASSERT_TRUE(ri.Valid == false);
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testParseIdentifierValid()
    {
        sad::resource::ResourceFileIdentifier ri;
        sad::resource::ResourceFileIdentifier::parse("tar7z:9://ftest.tar/img.img", ri);
        ASSERT_TRUE(ri.Valid);
        ASSERT_TRUE(ri.Type == sad::resource::ResourceFileType::RFT_TAR7Z_INNER_FILE);
        ASSERT_TRUE(ri.ArchiveName == "ftest.tar");
        ASSERT_TRUE(ri.FileName == "img.img");

        sad::resource::ResourceFileIdentifier::parse("tar7z:15://ftest_22_33.tar/img.img/img.img", ri);
        ASSERT_TRUE(ri.Valid);
        ASSERT_TRUE(ri.Type == sad::resource::ResourceFileType::RFT_TAR7Z_INNER_FILE);
        ASSERT_TRUE(ri.ArchiveName == "ftest_22_33.tar");
        ASSERT_TRUE(ri.FileName == "img.img/img.img");

        sad::resource::ResourceFileIdentifier::parse("tar7z:15://tests/image.tar/png.png", ri);
        ASSERT_TRUE(ri.Valid);
        ASSERT_TRUE(ri.Type == sad::resource::ResourceFileType::RFT_TAR7Z_INNER_FILE);
        ASSERT_TRUE(ri.ArchiveName == "tests/image.tar");
        ASSERT_TRUE(ri.FileName == "png.png");
    }


} _sad_resource_file_identifier_test;