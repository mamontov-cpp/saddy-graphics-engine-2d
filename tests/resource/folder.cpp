#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "resource/folder.h"
#include "resource/resource.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

int _deleted = 0;

class Resource:public sad::resource::Resource
{
SAD_OBJECT;
public:
    Resource()
    {
    }
    virtual bool load(
        const sad::resource::ResourceFile & file,
        sad::Renderer * r,
        const picojson::value& options
    )
    {
        return true;
    }
    ~Resource()
    {
        ++_deleted;
    }
};

DECLARE_SOBJ_INHERITANCE(Resource, sad::resource::Resource);

/*!
 * Tests sad::resource::Folder
 */
struct SadFolderTest : tpunit::TestFixture
{
 public:
   SadFolderTest() : tpunit::TestFixture(
       TEST(SadFolderTest::testAddRemoveFolder),
       TEST(SadFolderTest::testAddRemoveResource),
       TEST(SadFolderTest::testReplaceResource),
       TEST(SadFolderTest::testFind)
   ) {}

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst 
   void testAddRemoveFolder()
   {
       sad::resource::Folder * folder = new sad::resource::Folder();
       sad::resource::Folder * fld1 = new sad::resource::Folder();
       sad::resource::Folder * fld2 = new sad::resource::Folder();

       folder->addFolder("test1/test2/test3", fld1);
       folder->addFolder("test1/test3", fld2);
       
       ASSERT_TRUE( folder->folder("test1/test2/test3") == fld1 );
       ASSERT_TRUE( folder->folder("test1/test3") == fld2 );

       ASSERT_TRUE( fld1->parent() != nullptr );
       ASSERT_TRUE( fld2->parent() != nullptr );

       folder->removeFolder("test1/test2/test3");
       folder->removeFolder("test1/test3");

       ASSERT_TRUE( folder->folder("test1/test2/test3") == nullptr );
       ASSERT_TRUE( folder->folder("test1/test3") == nullptr );

       delete folder;
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst 
   void testAddRemoveResource()
   {
       _deleted = 0;

       sad::resource::Folder * folder = new sad::resource::Folder();
       Resource * r1 = new Resource();
       Resource * r2 = new Resource();

       folder->addResource("test1/test2/test3.jpg", r1);
       folder->addResource("test1/test3.jpg", r2);

       ASSERT_TRUE( folder->resource("test1/test2/test3.jpg") == r1 );
       ASSERT_TRUE( folder->resource("test1/test3.jpg") == r2 );

       folder->removeResource("test1/test2/test3.jpg");
       folder->removeResource("test1/test3.jpg");

       ASSERT_TRUE( folder->resource("test1/test2/test3.jpg") == nullptr );
       ASSERT_TRUE( folder->resource("test1/test3.jpg") == nullptr );
       
       delete folder;
       ASSERT_TRUE(_deleted == 2);
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst 
   void testReplaceResource()
   {
       _deleted = 0;

       sad::resource::Folder * folder = new sad::resource::Folder();
       Resource * r1 = new Resource();
       Resource * r2 = new Resource();

       folder->addResource("test1/test2/test3", r1);

       ASSERT_TRUE( folder->resource("test1/test2/test3") == r1 );

       folder->replaceResource("test1/test2/test3", r2);
       ASSERT_TRUE(_deleted == 1);

       ASSERT_TRUE( folder->resource("test1/test2/test3") == r2 );
       
       delete folder;
       ASSERT_TRUE(_deleted == 2);
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst 
   void testFind()
   {
       sad::resource::Folder * folder = new sad::resource::Folder();
       Resource * r1 = new Resource();

       folder->addResource("test1/test2/test3", r1);

       ASSERT_TRUE( folder->folder("test1") != nullptr );
       ASSERT_TRUE( folder->folder("test1")->folder("test2") != nullptr );


       sad::Maybe<sad::String> result = folder->find(r1);
       ASSERT_TRUE( result.value() == "test1/test2/test3" );

       delete folder;
   }

} _sad_folder_test;
