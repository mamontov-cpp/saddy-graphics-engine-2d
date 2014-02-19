#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "resource/link.h"
#include "resource/physicalfile.h"
#include "resource/tree.h"
#include "resource/resource.h"

#include "util/free.h"

#include "renderer.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


/*! Returns amount of errors of specified  type
	\return errors count
 */
int count_errors_of_type(
	const  sad::Vector<sad::resource::Error *> & errors, 
	const sad::String & type
)
{
	int result = 0;
	for(size_t i = 0; i < errors.size(); i++)
	{
		if (errors[i]->metaData()->canBeCastedTo(type))
		{
			result += 1;
		}
	}
	return result;
}

/*!
 * Tests sad::resource::Folder
 */
struct SadTreeTest : tpunit::TestFixture
{
 public:
   SadTreeTest() : tpunit::TestFixture(
	   TEST(SadTreeTest::testLoadFileNotExists),
	   TEST(SadTreeTest::testLoadEmptyFile)

   ) {}

   
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

   
} _sad_tree_test;
