#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>

#define USE_FREETYPE

#include "resource/link.h"
#include "resource/physicalfile.h"
#include "resource/tree.h"
#include "resource/resource.h"

#include "animations/animationsrotate.h"

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
struct SadAnimaionFileTest : tpunit::TestFixture
{
 public:
   SadAnimaionFileTest() : tpunit::TestFixture(
	   TEST(SadAnimaionFileTest::testCommon)
   ) {}

   void testCommon()
   {
	   sad::Renderer r;
	   sad::resource::Tree tree;
	   tree.setStoreLinks(true);
	   tree.setRenderer(&r);

	   sad::Vector<sad::resource::Error *> errors = tree.loadFromString(
		   "["
				"{"
					"\"type\"   : \"sad::animations::File\","
					"\"filename\": \"tests/animations/test.json\""
				"}"
			"]"
		);
	   int count = errors.size();
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);

	   sad::resource::Link<sad::animations::Rotate> l;
	   l.setPath("rotate");
	   l.setTree(&tree);
	   sad::animations::Rotate * old = l.get();
	   ASSERT_TRUE( old );

	   errors = tree.files()[0]->reload();
	   count = errors.size();
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);

	   sad::animations::Rotate * newo = l.get();
	   ASSERT_TRUE(newo);
	   ASSERT_TRUE(newo != old);
   }

} _sad_animation_file_test;
