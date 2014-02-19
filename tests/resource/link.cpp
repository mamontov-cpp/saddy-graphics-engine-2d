#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "resource/link.h"
#include "resource/resource.h"
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
		const sad::resource::PhysicalFile & file,
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

	void invoke0()
	{
		++_invoked;
	}

	void invokec0() const
	{
		++_invoked;
	}

	void invoke1(LinkResource * rk)
	{
		++_invoked;
	}

	void invokec1(LinkResource * rk) const
	{
		++_invoked;
	}

} _mock_link;

/*!
 * Tests sad::resource::Folder
 */
struct SadLinkTest : tpunit::TestFixture
{
 public:
   SadLinkTest() : tpunit::TestFixture(
	   TEST(SadLinkTest::testCallbacks)
   ) {}

   void invoke0()
   {
	   ++_invoked;
   }

   void invokec0() const
   {
	   ++_invoked;
   }

   void invoke1(LinkResource * rk)
   {
	   ++_invoked;
   }

   void invokec1(LinkResource * rk) const
   {
	   ++_invoked;
   }

   MockLinkData * data()
   {
	   return &_mock_link;
   }

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

   
} _sad_folder_test;
