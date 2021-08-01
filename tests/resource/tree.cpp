#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>

#define USE_FREETYPE

#include "resource/link.h"
#include "resource/resourcefile.h"
#include "resource/tree.h"
#include "resource/resource.h"

#include "util/free.h"
#include "freetype/font.h"
#include "texture.h"
#include "texturemappedfont.h"
#include "sprite2d.h"

#include "renderer.h"

#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

#include "counterrorsoftype.h"

/*!
 * Tests sad::resource::Tree
 */
struct SadTreeTest : tpunit::TestFixture
{
 public:
   SadTreeTest() : tpunit::TestFixture(
       TEST(SadTreeTest::testLoadFileNotExists),
       TEST(SadTreeTest::testLoadEmptyFile),
       TEST(SadTreeTest::testLoadFileThatConsistsOfWhitespaces),
       TEST(SadTreeTest::testBinary),
       TEST(SadTreeTest::testParseError),
       TEST(SadTreeTest::testMalformed1),
       TEST(SadTreeTest::testMalformed2),
       TEST(SadTreeTest::testInvalidTree),
       TEST(SadTreeTest::testUnregisteredFileType),
       TEST(SadTreeTest::testAnonymousResource),
       TEST(SadTreeTest::testLoadingFailure),
       TEST(SadTreeTest::testResourceDuplicates),
       TEST(SadTreeTest::testValid),
       TEST(SadTreeTest::testValidMultiple),
       TEST(SadTreeTest::testArchiveAtlas),
       TEST(SadTreeTest::testArchiveAnimation),
       TEST(SadTreeTest::testArchiveFont),   
       TEST(SadTreeTest::testUnload),
       TEST(SadTreeTest::testUnload2),
       TEST(SadTreeTest::testReloadTexture),
       TEST(SadTreeTest::testReloadTextureMappedFont),
       TEST(SadTreeTest::testReloadFreetypeFont),
       TEST(SadTreeTest::testArchiveEntry),
       TEST(SadTreeTest::testReloadAtlasFromArchive),
       TEST(SadTreeTest::testReloadAnimationFromArchive)
   ) {}


    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst   
   void testLoadFileNotExists()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("doesnotexists.json");
        
       int count = count_errors_of_type(errors, "sad::resource::FileLoadError");
       sad::util::free(errors);
       ASSERT_TRUE(count == 1);
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testLoadEmptyFile()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/empty.json");
        
       int count = errors.size();
       sad::util::free(errors);
       ASSERT_TRUE(count == 0);    
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testLoadFileThatConsistsOfWhitespaces()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/totallyspaced.json");
        
       int count = errors.size();
       sad::util::free(errors);
       ASSERT_TRUE(count == 0);    
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testBinary()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/binary.json");
        
       int count = count_errors_of_type(errors, "sad::resource::JSONParseError");
       sad::util::free(errors);
       ASSERT_TRUE(count == 1);  
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testParseError()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/parseerror.json");
        
       int count = count_errors_of_type(errors, "sad::resource::JSONParseError");
       sad::util::free(errors);
       ASSERT_TRUE(count == 1);  
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testMalformed1()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/malformed1.json");
        
       int count = count_errors_of_type(errors, "sad::resource::MalformedResourceEntry");
       sad::util::free(errors);
       ASSERT_TRUE(count == 1);  
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testMalformed2()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/malformed2.json");
        
       int count = count_errors_of_type(errors, "sad::resource::MalformedResourceEntry");
       sad::util::free(errors);
       ASSERT_TRUE(count == 1);  
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testInvalidTree()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/invalidtree.json");
        
       int count = count_errors_of_type(errors, "sad::resource::MalformedResourceEntry");
       sad::util::free(errors);
       ASSERT_TRUE(count == 1);
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testUnregisteredFileType()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/unregisteredtype.json");
        
       int count = count_errors_of_type(errors, "sad::resource::UnregisteredFileType");
       sad::util::free(errors);
       ASSERT_TRUE(count == 1);    
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testAnonymousResource()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/anonymousresource.json");
        
       int count = count_errors_of_type(errors, "sad::resource::AnonymousResource");
       sad::util::free(errors);
       ASSERT_TRUE(count == 1);    
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testLoadingFailure()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/loadingfailure.json");
        
       int count = count_errors_of_type(errors, "sad::resource::ResourceLoadError");
       sad::util::free(errors);
       ASSERT_TRUE(count == 1);
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testResourceDuplicates()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/duplicates.json");
        
       int count = count_errors_of_type(errors, "sad::resource::ResourceAlreadyExists");
       sad::util::free(errors);
       ASSERT_TRUE(count == 1);    
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testValid()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       // In debug, sad::fretype::Factory fonts becomes in font
       tree.factory()->registerResource<sad::freetype::Font>();

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/valid.json");
        
       int count = errors.size();
       sad::util::free(errors);
       ASSERT_TRUE(count == 0);
       ASSERT_TRUE(tree.root()->resource("objects") != nullptr);
       ASSERT_TRUE(tree.root()->resource("myfont") != nullptr);
       ASSERT_TRUE(tree.root()->resource("emporium.ttf") != nullptr);
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testValidMultiple()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       // In debug, sad::fretype::Factory fonts becomes in font
       tree.factory()->registerResource<sad::freetype::Font>();

       sad::Vector<sad::resource::Error *> errors = tree.loadFromFile("tests/valid.json");
       sad::Vector<sad::resource::Error *> errors2 = tree.loadFromFile("tests/validmultiple.json");
        
       int count = errors.size();
       sad::util::free(errors);

       int count2 = errors2.size();
       sad::util::free(errors2);

       ASSERT_TRUE(count == 0);
       ASSERT_TRUE(count2 == 0);

       ASSERT_TRUE(tree.root()->resource("objects") != nullptr);
       ASSERT_TRUE(tree.root()->resource("myfont") != nullptr);
       ASSERT_TRUE(tree.root()->resource("emporium.ttf") != nullptr);
       ASSERT_TRUE(tree.root()->resource("objects2") != nullptr);
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst   
   void testArchiveAtlas()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       // In debug, sad::fretype::Factory fonts becomes in font
       tree.factory()->registerResource<sad::freetype::Font>();
       sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
           "["
                "{"
                    "\"type\"   : \"sad::resource::TextureAtlasFile\","
                    "\"filename\": \"tar7z:24://tests/resources/data.tar/srgba-atlas.json\""
                "}"
            "]"
        );
        
        int count = errors.size();
        sad::util::free(errors);
        ASSERT_TRUE(count == 0);
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst   
   void testArchiveAnimation()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       // In debug, sad::fretype::Factory fonts becomes in font
       tree.factory()->registerResource<sad::freetype::Font>();
       sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
           "["
                "{"
                    "\"type\"   : \"sad::animations::File\","
                    "\"filename\": \"tar7z:24://tests/resources/data.tar/test.json\""
                "}"
            "]"
        );
        
        int count = errors.size();
        sad::util::free(errors);
        ASSERT_TRUE(count == 0);   
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst   
   void testArchiveFont()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       // In debug, sad::fretype::Factory fonts becomes in font
       tree.factory()->registerResource<sad::freetype::Font>();
       sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
           "["
                "{"
                    "\"type\"   : \"sad::freetype::Font\","
                    "\"filename\": \"tar7z:24://tests/resources/data.tar/EMPORIUM.TTF\""
                "}"
            "]"
        );
        
        int count = errors.size();
        sad::util::free(errors);
        ASSERT_TRUE(count == 1);  
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testUnload()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       // In debug, sad::fretype::Factory fonts becomes in font
       tree.factory()->registerResource<sad::freetype::Font>();

       sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
           "["
                "{"
                    "\"type\"   : \"sad::Texture\","
                    "\"filename\": \"examples/game/objects.bmp\","
                    "\"name\"    : \"objects\""
                "},"
                "{"
                    "\"type\"   : \"sad::Texture\","
                    "\"filename\": \"examples/game/title.tga\","
                    "\"name\"    : \"objects2\""
                "}"
            "]"
        );
       int count = errors.size();
       sad::util::free(errors);
       ASSERT_TRUE(count == 0);
       ASSERT_TRUE(tree.root()->resource("objects") != nullptr);
       ASSERT_TRUE(tree.root()->resource("objects2") != nullptr);

       sad::resource::Link<sad::Texture> l1, l2;
       l1.setTree(&tree);
       l2.setTree(&tree);
       l1.setPath("objects");
       l2.setPath("objects");

       ASSERT_TRUE(l1.get() != nullptr);
       ASSERT_TRUE(l2.get() != nullptr);

       ASSERT_FALSE( tree.unload("examples/game/objects.bmp") );
       ASSERT_TRUE ( tree.unload("examples/game/title.tga") );
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testUnload2()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       // In debug, sad::fretype::Factory fonts becomes in font
       tree.factory()->registerResource<sad::freetype::Font>();

       sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
           "["
                "{"
                    "\"type\"   : \"sad::Texture\","
                    "\"filename\": \"examples/game/objects.bmp\","
                    "\"name\"    : \"objects\""
                "},"
                "{"
                    "\"type\"   : \"sad::Texture\","
                    "\"filename\": \"examples/game/objects.bmp\","
                    "\"name\"    : \"objects2\""
                "}"
            "]"
        );
       int count = errors.size();
       sad::util::free(errors);
       ASSERT_TRUE(count == 0);
       ASSERT_TRUE(tree.root()->resource("objects") != nullptr);
       ASSERT_TRUE(tree.root()->resource("objects2") != nullptr);

       sad::resource::Link<sad::Texture> l1, l2;
       l1.setTree(&tree);
       l2.setTree(&tree);
       l1.setPath("objects");
       l2.setPath("objects");

       ASSERT_TRUE(l1.get() != nullptr);
       ASSERT_TRUE(l2.get() != nullptr);

       ASSERT_FALSE( tree.unload(tree.root()->resource("objects")->file()) );
       ASSERT_TRUE ( tree.unload(tree.root()->resource("objects2")->file()) );
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testReloadTexture()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       // In debug, sad::fretype::Factory fonts becomes in font
       tree.factory()->registerResource<sad::freetype::Font>();

       sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
           "["
                "{"
                    "\"type\"   : \"sad::Texture\","
                    "\"filename\": \"examples/game/objects.bmp\","
                    "\"name\"    : \"objects\""
                "}"
            "]"
        );
       int count = errors.size();
       sad::util::free(errors);
       ASSERT_TRUE(count == 0);
       ASSERT_TRUE(tree.root()->resource("objects") != nullptr);

       sad::resource::Link<sad::Texture> l1;
       l1.setTree(&tree);
       l1.setPath("objects");

       sad::Texture * old = l1.get();
       ASSERT_TRUE(old != nullptr);

       rename("examples/game/objects.bmp", "examples/game/objects.bmp2");

       sad::Vector<sad::resource::Error *> result  = tree.root()->resource("objects")->file()->reload();
       count = result.size();
       sad::util::free(result);
       ASSERT_TRUE( count != 0 );

       rename("examples/game/objects.bmp2", "examples/game/objects.bmp");

       result  = tree.root()->resource("objects")->file()->reload();
       count = result.size();
       sad::util::free(result);
       ASSERT_TRUE( count == 0 );

       ASSERT_TRUE( l1.get() != old );
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testReloadTextureMappedFont()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       // In debug, sad::fretype::Factory fonts becomes in font
       tree.factory()->registerResource<sad::freetype::Font>();

       sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
           "["
                "{"
                    "\"type\"   : \"sad::TextureMappedFont\","
                    "\"filename\": \"examples/game/font\","
                    "\"name\"    : \"objects\""
                "}"
            "]"
        );
       int count = errors.size();
       sad::util::free(errors);
       ASSERT_TRUE(count == 0);
       ASSERT_TRUE(tree.root()->resource("objects") != nullptr);

       sad::resource::Link<sad::TextureMappedFont> l1;
       l1.setTree(&tree);
       l1.setPath("objects");

       sad::TextureMappedFont * old = l1.get();
       ASSERT_TRUE(old != nullptr);

       rename("examples/game/font.cfg", "examples/game/font.cfg2");

       sad::Vector<sad::resource::Error *> result  = tree.root()->resource("objects")->file()->reload();
       count = result.size();
       sad::util::free(result);
       ASSERT_TRUE( count != 0 );

       rename("examples/game/font.cfg2", "examples/game/font.cfg");

       result  = tree.root()->resource("objects")->file()->reload();
       count = result.size();
       sad::util::free(result);
       ASSERT_TRUE( count == 0 );

       ASSERT_TRUE( l1.get() != old );
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testReloadFreetypeFont()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       // In debug, sad::fretype::Factory fonts becomes in font
       tree.factory()->registerResource<sad::freetype::Font>();

       sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
           "["
                "{"
                    "\"type\"   : \"sad::freetype::Font\","
                    "\"filename\": \"ifaceed/EMPORIUM.TTF\","
                    "\"name\"    : \"objects\""
                "}"
            "]"
        );
       int count = errors.size();
       sad::util::free(errors);
       ASSERT_TRUE(count == 0);
       ASSERT_TRUE(tree.root()->resource("objects") != nullptr);

       sad::resource::Link<sad::freetype::Font> l1;
       l1.setTree(&tree);
       l1.setPath("objects");

       sad::freetype::Font * old = l1.get();
       ASSERT_TRUE(old != nullptr);

       sad::Vector<sad::resource::Error*> result  = tree.root()->resource("objects")->file()->reload();
       count = result.size();
       sad::util::free(result);
       ASSERT_TRUE( count == 0 );

       ASSERT_TRUE( l1.get() != old );
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testArchiveEntry()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       ASSERT_TRUE( tree.archiveEntry("tests/images/images.tar", "png.png") != nullptr) ;
       ASSERT_TRUE( tree.archiveEntry("111", "png.png") == nullptr) ;
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testReloadAtlasFromArchive()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       // In debug, sad::fretype::Factory fonts becomes in font
       tree.factory()->registerResource<sad::freetype::Font>();
       sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
           "["
                "{"
                    "\"type\"   : \"sad::resource::TextureAtlasFile\","
                    "\"filename\": \"tar7z:24://tests/resources/data.tar/srgba-atlas.json\""
                "}"
            "]"
        );
        
        int count = errors.size();
        sad::util::free(errors);
        ASSERT_TRUE(count == 0);

        errors  = tree.root()->resource("ace.png")->file()->reload();
        count = errors.size();
        sad::util::free(errors);
        ASSERT_TRUE( count == 0 );
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testReloadAnimationFromArchive()
   {
       sad::Renderer r;
       sad::resource::Tree tree;
       tree.setStoreLinks(true);
       tree.setRenderer(&r);
       // In debug, sad::fretype::Factory fonts becomes in font
       tree.factory()->registerResource<sad::freetype::Font>();
       sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
           "["
                "{"
                    "\"type\"   : \"sad::animations::File\","
                    "\"filename\": \"tar7z:24://tests/resources/data.tar/test.json\""
                "}"
            "]"
        );
        
        int count = errors.size();
        sad::util::free(errors);
        ASSERT_TRUE(count == 0);

        errors  = tree.root()->resource("rotate")->file()->reload();
        count = errors.size();
        sad::util::free(errors);
        ASSERT_TRUE( count == 0 );
   }

} _sad_tree_test;
