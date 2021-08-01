#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include "resource/link.h"
#include "resource/resource.h"
#include "renderer.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

int _invoked = 0;

class LinkResource:public sad::resource::Resource
{
SAD_OBJECT;
public:
    LinkResource()
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
    ~LinkResource()
    {

    }
};


void invoke0()
{
    ++_invoked;
}

void invoke1(LinkResource * rk)
{
    ++_invoked;
}


DECLARE_SOBJ_INHERITANCE(LinkResource, sad::resource::Resource);


class MockLinkData
{
public:

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void invoke0()
    {
        ++_invoked;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void invokec0() const
    {
        ++_invoked;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void invoke1(LinkResource * rk)
    {
        ++_invoked;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void invokec1(LinkResource * rk) const
    {
        ++_invoked;
    }

} _mock_link;

/*!
 * Tests sad::resource::Link
 */
struct SadLinkTest : tpunit::TestFixture
{
 public:
   SadLinkTest() : tpunit::TestFixture(
       TEST(SadLinkTest::testCallbacks),
       TEST(SadLinkTest::testFetchMultipleTrees)
   ) {}

   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
   void invoke0()
   {
       ++_invoked;
   }

   // ReSharper disable once CppMemberFunctionMayBeStatic
   void invokec0() const
   {
       ++_invoked;
   }

   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
   void invoke1(LinkResource * rk)
   {
       ++_invoked;
   }

   // ReSharper disable once CppMemberFunctionMayBeStatic
   void invokec1(LinkResource * rk) const
   {
       ++_invoked;
   }

   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
   MockLinkData * data()
   {
       return &_mock_link;
   }

   // ReSharper disable once CppMemberFunctionMayBeStatic
   MockLinkData * const_data() const
   {
       return &_mock_link;
   }

   void testCallbacks()
   {
       sad::resource::Link<LinkResource> link;

       _invoked = 0;

       LinkResource * r1 = new LinkResource();
       LinkResource * r2 = new LinkResource();

       link.attach(r1);

       link.add(::invoke0);
       link.add(::invoke1);

       link.add(this, &SadLinkTest::invoke0);
       link.add(this, &SadLinkTest::invokec0);
       link.add(this, &SadLinkTest::invoke1);
       link.add(this, &SadLinkTest::invokec1);

       link.add(this, &SadLinkTest::data, &MockLinkData::invoke0);
       link.add(this, &SadLinkTest::data, &MockLinkData::invokec0);
       link.add(this, &SadLinkTest::data, &MockLinkData::invoke1);
       link.add(this, &SadLinkTest::data, &MockLinkData::invokec1);

       link.add(this, &SadLinkTest::const_data, &MockLinkData::invoke0);
       link.add(this, &SadLinkTest::const_data, &MockLinkData::invokec0);
       link.add(this, &SadLinkTest::const_data, &MockLinkData::invoke1);
       sad::util::PointerCallback<LinkResource> * toberemoved = 
            link.add(this, &SadLinkTest::const_data, &MockLinkData::invokec1);

       link.attach(r2);

       ASSERT_TRUE(_invoked == 14);
       
       link.removeCallback(toberemoved);

       link.attach(r1);

       ASSERT_TRUE(_invoked == 27);

       delete r1;
       delete r2;
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testFetchMultipleTrees()
   {
       sad::Renderer r;
       r.addTree("mylittletree", new sad::resource::Tree());
       r.tree("mylittletree")->root()->addResource("friendshipismagic/s1", new LinkResource());
       r.tree()->root()->addResource("1/1/1", new LinkResource());

       sad::resource::Link<LinkResource> l1;
       l1.setTree(&r, "mylittletree");
       l1.setPath("friendshipismagic/s1");
       ASSERT_TRUE(l1.resource() != nullptr);

       sad::resource::Link<LinkResource> l2;
       l2.setTree(&r, "");
       l2.setPath("1/1/1");
       ASSERT_TRUE(l2.resource() != nullptr);

   }

   
} _sad_link_test;
