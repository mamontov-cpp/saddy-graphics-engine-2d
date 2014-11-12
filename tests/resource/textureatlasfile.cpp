#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>

#define USE_FREETYPE

#include "resource/link.h"
#include "resource/physicalfile.h"
#include "resource/tree.h"
#include "resource/resource.h"

#include "util/free.h"

#include "renderer.h"
#include "sprite2d.h"
#include "fuzzyequal.h"

#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

#include "counterrorsoftype.h"

/*!
 * Tests sad::resource::TextureAtlasFile
 */
struct SadTextureAtlasFileTest : tpunit::TestFixture
{
 public:
   SadTextureAtlasFileTest() : tpunit::TestFixture(
	   TEST(SadTextureAtlasFileTest::testLoadDoesNotExists),
	   TEST(SadTextureAtlasFileTest::testLoadBinary),
	   TEST(SadTextureAtlasFileTest::testLoadEmpty),
	   TEST(SadTextureAtlasFileTest::testLoadPartiallyValidSyntax),
	   TEST(SadTextureAtlasFileTest::testLoadMalformed1),
	   TEST(SadTextureAtlasFileTest::testLoadMalformed2),
	   TEST(SadTextureAtlasFileTest::testLoadMalformed3),
	   TEST(SadTextureAtlasFileTest::testLoadMalformed4),
	   TEST(SadTextureAtlasFileTest::testLoadMalformed5),
	   TEST(SadTextureAtlasFileTest::testLoadMalformed6),
	   TEST(SadTextureAtlasFileTest::testLoadMalformed7),
	   TEST(SadTextureAtlasFileTest::testLoadMalformed8),
	   TEST(SadTextureAtlasFileTest::testLoadResourceFail),
	   TEST(SadTextureAtlasFileTest::testLoadValid),
	   TEST(SadTextureAtlasFileTest::testLoadResourceExists),
	   TEST(SadTextureAtlasFileTest::testReloadValid),
	   TEST(SadTextureAtlasFileTest::testReloadSourceJSONIsAbsent),
	   TEST(SadTextureAtlasFileTest::testReloadSourceResourceIsAbsent),
	   TEST(SadTextureAtlasFileTest::testReloadCannotDeleteReferenced),
	   TEST(SadTextureAtlasFileTest::testReloadAddedResource),
	   TEST(SadTextureAtlasFileTest::testReloadReplacedResource)
   ) {}

   void testLoadDoesNotExists()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"doesnotexists.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::FileLoadError");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testLoadBinary()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons_binary.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::JSONParseError");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testLoadEmpty()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons_empty.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::EmptyTextureAtlas");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testLoadPartiallyValidSyntax()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons_partiallyvalid.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::JSONParseError");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testLoadMalformed1()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons_malformed1.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::MalformedResourceEntry");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testLoadMalformed2()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons_malformed2.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::MalformedResourceEntry");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testLoadMalformed3()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons_malformed3.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::MalformedResourceEntry");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testLoadMalformed4()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons_malformed4.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::MalformedResourceEntry");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testLoadMalformed5()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons_malformed5.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::MalformedResourceEntry");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testLoadMalformed6()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons_malformed6.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::MalformedResourceEntry");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testLoadMalformed7()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons_malformed7.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::MalformedResourceEntry");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testLoadMalformed8()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons_malformed8.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::MalformedResourceEntry");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testLoadResourceFail()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons_resourcefail.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::ResourceLoadError");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testLoadValid()
   {	   
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);
	
	   ASSERT_TRUE(tree.root()->resource("icons") != NULL);
	   ASSERT_TRUE(tree.root()->resource("1") != NULL);
   }

   void testLoadResourceExists()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::Texture\","
					"\"filename\": \"examples/game/objects.bmp\","
					"\"name\"    : \"icons\""
				"},"
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"}"
			"]"
		);
	   int count = count_errors_of_type(errors, "sad::resource::ResourceAlreadyExists");
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 1);
   }

   void testReloadValid()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);
	
	   sad::resource::Link<sad::Sprite2D::Options> l;
	   l.setPath("1");
	   l.setTree(&tree);
	   sad::Sprite2D::Options * old = l.get();

	   sad::resource::Link<sad::Texture> l2;
	   l2.setPath("2");
	   l2.setTree(&tree);
	   sad::Texture * test = l2.get();

	   ASSERT_TRUE(old != NULL);
	   ASSERT_TRUE(test == NULL);


	   errors = tree.root()->resource("1")->file()->reload();
	   count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);

	   ASSERT_TRUE(old != l.get());
   }

   void testReloadSourceJSONIsAbsent()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);
	
	   sad::resource::Link<sad::Sprite2D::Options> l;
	   l.setPath("1");
	   l.setTree(&tree);
	   sad::Sprite2D::Options * old = l.get();;

	   ASSERT_TRUE(old != NULL);


	   rename("tests/icons.json", "tests/icons.json.tmp");

	   errors = tree.root()->resource("1")->file()->reload();
	   count = count_errors_of_type(errors, "sad::resource::FileLoadError");
	   sad::util::free(errors);

	   rename("tests/icons.json.tmp", "tests/icons.json");
	   ASSERT_TRUE(count == 1);
   }

   void testReloadSourceResourceIsAbsent()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);
	
	   sad::resource::Link<sad::Sprite2D::Options> l;
	   l.setPath("1");
	   l.setTree(&tree);
	   sad::Sprite2D::Options * old = l.get();;

	   ASSERT_TRUE(old != NULL);


	   rename("tests/icons.png", "tests/icons.png.tmp");

	   errors = tree.root()->resource("1")->file()->reload();
	   count = count_errors_of_type(errors, "sad::resource::ResourceLoadError");
	   sad::util::free(errors);

	   rename("tests/icons.png.tmp", "tests/icons.png");
	   ASSERT_TRUE(count == 1);
   }

   void testReloadCannotDeleteReferenced()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);
	
	   sad::resource::Link<sad::Sprite2D::Options> l;
	   l.setPath("1");
	   l.setTree(&tree);
	   sad::Sprite2D::Options * old = l.get();;

	   ASSERT_TRUE(old != NULL);


	   rename("tests/icons.json", "tests/icons.json.tmp");
	   rename("tests/icons_deleted.json", "tests/icons.json");

	   errors = tree.root()->resource("1")->file()->reload();
	   count = count_errors_of_type(errors, "sad::resource::CannotDeleteReferencedResource");
	   sad::util::free(errors);

	   rename("tests/icons.json", "tests/icons_deleted.json");
	   rename("tests/icons.json.tmp", "tests/icons.json");
	   ASSERT_TRUE(count == 1);
   }

   void testReloadAddedResource()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);
	
	   sad::resource::Link<sad::Sprite2D::Options> l;
	   l.setPath("1");
	   l.setTree(&tree);
	   sad::Sprite2D::Options * old = l.get();

	   ASSERT_TRUE(old != NULL);


	   rename("tests/icons.json", "tests/icons.json.tmp");
	   rename("tests/icons_added.json", "tests/icons.json");

	   errors = tree.root()->resource("1")->file()->reload();
	   count = (int)(errors.size());
	   sad::util::free(errors);

	   rename("tests/icons.json", "tests/icons_added.json");
	   rename("tests/icons.json.tmp", "tests/icons.json");
	   ASSERT_TRUE(count == 0);

	   sad::resource::Link<sad::Sprite2D::Options> l2;
	   l2.setPath("7");
	   l2.setTree(&tree);
	   ASSERT_TRUE(l2.get() != NULL);
   }

   void testReloadReplacedResource()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"}"
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);
	
	   sad::resource::Link<sad::Sprite2D::Options> l;
	   l.setPath("1");
	   l.setTree(&tree);
	   sad::Sprite2D::Options * old = l.get();

	   ASSERT_TRUE(old != NULL);


	   rename("tests/icons.json", "tests/icons.json.tmp");
	   rename("tests/icons_replaced.json", "tests/icons.json");

	   errors = tree.root()->resource("1")->file()->reload();
	   count = (int)(errors.size());
	   sad::util::free(errors);

	   rename("tests/icons.json", "tests/icons_replaced.json");
	   rename("tests/icons.json.tmp", "tests/icons.json");
	   ASSERT_TRUE(count == 0);

	   double h = l.get()->Rectangle.height();
	   ASSERT_TRUE( sad::is_fuzzy_equal(h, 500.0) );
   }

} /*_sad_texture_atlas_file_test*/;
