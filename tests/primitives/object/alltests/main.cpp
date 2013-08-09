#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include "primitives/object.h"
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "scene.h"
#include "templates/ptrhash.hpp"
#include "templates/hstring.h"
#pragma warning(pop)

// Declare primary descendants
class DirectDescendant1: public sad::Object 
{
	SAD_OBJECT
	protected:
		int m_dir_desc_prot1;
	public:
	DirectDescendant1() { m_dir_desc_prot1 = 1;}
	int local() { return m_dir_desc_prot1;}
};
DECLARE_SOBJ(DirectDescendant1);
class DirectDescendant2: public sad::Object 
{
	SAD_OBJECT
	protected:
		int m_dir_dec_prot2;
	public:
	DirectDescendant2() { 
		m_dir_dec_prot2 = 2;
	}
	int local() { 
		return m_dir_dec_prot2;
	}
};
DECLARE_SOBJ(DirectDescendant2);
class DirectDescendant3: public sad::Object 
{
	SAD_OBJECT
	protected:
		int m_dir_dec_prot3;
	public:
	DirectDescendant3() { m_dir_dec_prot3 = 3; }
	int local() { return m_dir_dec_prot3;}
};
DECLARE_SOBJ(DirectDescendant3);
class DirectDescendant4: public sad::Object 
{
	SAD_OBJECT
	protected:
		int m_dir_dec_prot4;
	public:
	DirectDescendant4() { m_dir_dec_prot4 = 4;}
	int local() { return m_dir_dec_prot4;}

};
DECLARE_SOBJ(DirectDescendant4);

class InheritedFrom1: public DirectDescendant1 {
	SAD_OBJECT
	protected:
		int m_inh_from1;
	public:
	InheritedFrom1() { m_inh_from1 = 5; }
	int local() { return m_inh_from1;}
};

DECLARE_SOBJ_INHERITANCE(InheritedFrom1, 
                         DirectDescendant1);

class InheritedFrom2 : public InheritedFrom1, public DirectDescendant2
{
	SAD_OBJECT
	protected:
		int m_inh_from2;
	public:
	InheritedFrom2() { m_inh_from2 = 6; }
	int local() { return m_inh_from2;}
};

DECLARE_SOBJ_INHERITANCE2(InheritedFrom2, 
                          InheritedFrom1, 
						  DirectDescendant2);

class InheritedFrom3 : public InheritedFrom1, 
					   public DirectDescendant2,
					   public DirectDescendant3
{
	SAD_OBJECT
	protected:
		int m_inh_from3;
	public:
	InheritedFrom3() { m_inh_from3 = 7; }
	int local() { return m_inh_from3;}
};

DECLARE_SOBJ_INHERITANCE3(InheritedFrom3, 
                          InheritedFrom1, 
						  DirectDescendant2,
						  DirectDescendant3);

class InheritedFrom4 : public InheritedFrom1, 
					   public DirectDescendant2,
					   public DirectDescendant3,
					   public DirectDescendant4
{
	SAD_OBJECT
	protected:
		int m_inh_from4;
	public:
	InheritedFrom4() { m_inh_from4 = 8;}
	int local() { return m_inh_from4;}
};

DECLARE_SOBJ_INHERITANCE4(InheritedFrom4, 
                          InheritedFrom1, 
						  DirectDescendant2,
						  DirectDescendant3,
						  DirectDescendant4);

/*!
 * Tests object logic
 */
struct SadObjectTest : tpunit::TestFixture
{
 public:
   SadObjectTest() : tpunit::TestFixture(
	   BEFORE(SadObjectTest::init),
	   TEST(SadObjectTest::test_global_metadata),
	   TEST(SadObjectTest::test_can_do_upcast_to_top),
	   TEST(SadObjectTest::test_can_do_local_upcast),
	   TEST(SadObjectTest::test_successfull_downcasts),
	   TEST(SadObjectTest::test_ancestor_to_children_castfrom),
	   TEST(SadObjectTest::test_valid_casts),
	   TEST(SadObjectTest::test_inherited_from1),
	   TEST(SadObjectTest::test_inherited_from2),
	   TEST(SadObjectTest::test_inherited_from3),
	   TEST(SadObjectTest::test_inherited_from4),
	   TEST(SadObjectTest::test_fail_cast)
   ) {}
   /*! Cache, which stores objects by class
    */
   hst::ptrhash<hst::string, sad::Object> m_cache;
   /*! Inits a cache with objects
    */
   void init() 
   {
#define CACHE_TUPLE(CLASS)  m_cache.insert( #CLASS, new CLASS () ); 
#define CACHE_TUPLE2(CLASS)  m_cache.insert( #CLASS, (InheritedFrom1*)(new CLASS ()) ); 
	   CACHE_TUPLE(DirectDescendant1);
	   CACHE_TUPLE(DirectDescendant2);
	   CACHE_TUPLE(DirectDescendant3);
	   CACHE_TUPLE(DirectDescendant4);
	   CACHE_TUPLE(InheritedFrom1);
	   CACHE_TUPLE2(InheritedFrom2);
	   CACHE_TUPLE2(InheritedFrom3);
	   CACHE_TUPLE2(InheritedFrom4);
#undef  CACHE_TUPLE
#undef  CACHE_TUPLE2
   }

   /*! Tests global metadata workaround with name
    */
   void test_global_metadata() 
   {
	   ASSERT_EQUAL(DirectDescendant1::globalMetaData()->name(),
					"DirectDescendant1");
	   ASSERT_EQUAL(DirectDescendant2::globalMetaData()->name(),
				    "DirectDescendant2");
	   ASSERT_EQUAL(DirectDescendant3::globalMetaData()->name(),
				    "DirectDescendant3");
	   ASSERT_EQUAL(DirectDescendant4::globalMetaData()->name(),
				    "DirectDescendant4");
	   ASSERT_EQUAL(InheritedFrom1::globalMetaData()->name(),
				    "InheritedFrom1");	   
   }

   void test_can_do_upcast_to_top()
   {
	   ASSERT_TRUE( m_cache["DirectDescendant1"]->
					metaData()->canBeCastedTo("sad::Object"));
	   ASSERT_TRUE( m_cache["DirectDescendant2"]->
					metaData()->canBeCastedTo("sad::Object"));
	   ASSERT_TRUE( m_cache["DirectDescendant3"]->
					metaData()->canBeCastedTo("sad::Object"));
	   ASSERT_TRUE( m_cache["DirectDescendant4"]->
					metaData()->canBeCastedTo("sad::Object"));
	   ASSERT_TRUE( m_cache["InheritedFrom1"]->
					metaData()->canBeCastedTo("sad::Object"));
	   ASSERT_TRUE( m_cache["InheritedFrom2"]->
					metaData()->canBeCastedTo("sad::Object"));
	   ASSERT_TRUE( m_cache["InheritedFrom3"]->
					metaData()->canBeCastedTo("sad::Object"));
	   ASSERT_TRUE( m_cache["InheritedFrom4"]->
					metaData()->canBeCastedTo("sad::Object"));
	   // Non-connected
	   ASSERT_FALSE( m_cache["DirectDescendant2"]->
					metaData()->canBeCastedTo("DirectDescendant1"));
	   // Ancestor to child
	   ASSERT_FALSE( m_cache["DirectDescendant1"]->
					metaData()->canBeCastedTo("InheritedFrom1"));
   }

   void test_can_do_local_upcast()
   {
		ASSERT_TRUE( m_cache["InheritedFrom1"]->
					metaData()->canBeCastedTo("DirectDescendant1"));
		ASSERT_TRUE( m_cache["InheritedFrom2"]->
					metaData()->canBeCastedTo("DirectDescendant1"));
		ASSERT_TRUE( m_cache["InheritedFrom3"]->
					metaData()->canBeCastedTo("DirectDescendant1"));
		ASSERT_TRUE( m_cache["InheritedFrom4"]->
					metaData()->canBeCastedTo("DirectDescendant1"));
		ASSERT_TRUE( m_cache["InheritedFrom2"]->
					metaData()->canBeCastedTo("DirectDescendant2"));
		ASSERT_TRUE( m_cache["InheritedFrom3"]->
					metaData()->canBeCastedTo("DirectDescendant2"));
		ASSERT_TRUE( m_cache["InheritedFrom4"]->
					metaData()->canBeCastedTo("DirectDescendant2"));
		ASSERT_TRUE( m_cache["InheritedFrom3"]->
					metaData()->canBeCastedTo("DirectDescendant3"));
		ASSERT_TRUE( m_cache["InheritedFrom4"]->
					metaData()->canBeCastedTo("DirectDescendant3"));
		ASSERT_TRUE( m_cache["InheritedFrom4"]->
					metaData()->canBeCastedTo("DirectDescendant4"));
   }

   void test_successfull_downcasts()
   {
	   ASSERT_TRUE(m_cache["DirectDescendant1"]->
				   metaData()->canBeCastedFrom("InheritedFrom1"));
	   ASSERT_TRUE(m_cache["DirectDescendant1"]->
				   metaData()->canBeCastedFrom("InheritedFrom2"));
	   ASSERT_TRUE(m_cache["DirectDescendant1"]->
				   metaData()->canBeCastedFrom("InheritedFrom3"));
	   ASSERT_TRUE(m_cache["DirectDescendant1"]->
				   metaData()->canBeCastedFrom("InheritedFrom4"));
	   ASSERT_TRUE(m_cache["DirectDescendant2"]->
				   metaData()->canBeCastedFrom("InheritedFrom2"));
	   ASSERT_TRUE(m_cache["DirectDescendant2"]->
				   metaData()->canBeCastedFrom("InheritedFrom3"));
	   ASSERT_TRUE(m_cache["DirectDescendant2"]->
				   metaData()->canBeCastedFrom("InheritedFrom4"));
	   ASSERT_TRUE(m_cache["DirectDescendant3"]->
				   metaData()->canBeCastedFrom("InheritedFrom3"));
	   ASSERT_TRUE(m_cache["DirectDescendant3"]->
				   metaData()->canBeCastedFrom("InheritedFrom4"));
	   ASSERT_TRUE(m_cache["DirectDescendant4"]->
				   metaData()->canBeCastedFrom("InheritedFrom4"));
   }
   
   void test_ancestor_to_children_castfrom()
   {
		ASSERT_FALSE(m_cache["InheritedFrom1"]->
				     metaData()->canBeCastedFrom("DirectDescendant1"));
		ASSERT_FALSE(m_cache["InheritedFrom1"]->
				     metaData()->canBeCastedFrom("DirectDescendant2"));
		ASSERT_FALSE(m_cache["InheritedFrom1"]->
				     metaData()->canBeCastedFrom("DirectDescendant3"));
		ASSERT_FALSE(m_cache["InheritedFrom1"]->
				     metaData()->canBeCastedFrom("DirectDescendant4"));
		ASSERT_FALSE(m_cache["InheritedFrom2"]->
				     metaData()->canBeCastedFrom("DirectDescendant2"));
		ASSERT_FALSE(m_cache["InheritedFrom3"]->
				     metaData()->canBeCastedFrom("DirectDescendant3"));
		ASSERT_FALSE(m_cache["InheritedFrom4"]->
				     metaData()->canBeCastedFrom("DirectDescendant4"));
   }

   void test_valid_casts()
   {
	   ASSERT_EQUAL(m_cache["DirectDescendant1"]->
				    as<DirectDescendant1>()->local(), 1);
	   ASSERT_EQUAL(m_cache["DirectDescendant2"]->
				    as<DirectDescendant2>()->local(), 2);
	   ASSERT_EQUAL(m_cache["DirectDescendant3"]->
				    as<DirectDescendant3>()->local(), 3);
	   ASSERT_EQUAL(m_cache["DirectDescendant4"]->
				    as<DirectDescendant4>()->local(), 4);
   }

   void test_inherited_from1()
   {
	   ASSERT_EQUAL(m_cache["InheritedFrom1"]->
				    as<InheritedFrom1>()->local(), 5);
	   ASSERT_EQUAL(m_cache["InheritedFrom1"]->
				    as<DirectDescendant1>()->local(), 1);
   }

   void test_inherited_from2()
   {
	   sad::Object * testbase = m_cache["InheritedFrom2"];
	   InheritedFrom1 * testif1 = testbase->as<InheritedFrom1>();
	   InheritedFrom2 * testif2 = hst::checked_cast<InheritedFrom2>(testif1);
	   ASSERT_EQUAL(testif2->local(), 6);
	   ASSERT_EQUAL(testif1->local(), 5);
	   ASSERT_EQUAL(testbase->as<DirectDescendant1>()->local(), 1);
	   DirectDescendant2 * testdd2 = 
		   hst::checked_cast<DirectDescendant2>(testif2);								     
	   ASSERT_EQUAL(testdd2->local(), 2);
   }

   void test_inherited_from3()
   {
	   sad::Object * testbase = m_cache["InheritedFrom3"];
	   InheritedFrom1 * testif1 = testbase->as<InheritedFrom1>();
	   InheritedFrom3 * testif2 = hst::checked_cast<InheritedFrom3>(testif1);
	   ASSERT_EQUAL(testif2->local(), 7);
	   ASSERT_EQUAL(testif1->local(), 5);
	   ASSERT_EQUAL(testbase->as<DirectDescendant1>()->local(), 1);
	   DirectDescendant2 * testdd2 = 
		   hst::checked_cast<DirectDescendant2>(testif2);								     
	   ASSERT_EQUAL(testdd2->local(), 2);
	   DirectDescendant3 * testdd3 = 
		   hst::checked_cast<DirectDescendant3>(testif2);								     
	   ASSERT_EQUAL(testdd3->local(), 3);
   }

   void test_inherited_from4()
   {
	   sad::Object * testbase = m_cache["InheritedFrom4"];
	   InheritedFrom1 * testif1 = testbase->as<InheritedFrom1>();
	   InheritedFrom4 * testif2 = hst::checked_cast<InheritedFrom4>(testif1);
	   ASSERT_EQUAL(testif2->local(), 8);
	   ASSERT_EQUAL(testif1->local(), 5);
	   ASSERT_EQUAL(testbase->as<DirectDescendant1>()->local(), 1);
	   DirectDescendant2 * testdd2 = 
		   hst::checked_cast<DirectDescendant2>(testif2);								     
	   ASSERT_EQUAL(testdd2->local(), 2);
	   DirectDescendant3 * testdd3 = 
		   hst::checked_cast<DirectDescendant3>(testif2);								     
	   ASSERT_EQUAL(testdd3->local(), 3);
	   DirectDescendant4 * testdd4 = 
		   hst::checked_cast<DirectDescendant4>(testif2);								     
	   ASSERT_EQUAL(testdd4->local(), 4);
   }

   void test_fail_cast() 
   {
	    sad::Object * testbase = m_cache["DirectDescendant1"];
		try
		{
			InheritedFrom1 * i2 = testbase->as<InheritedFrom1>();
			// Expect exception
			ASSERT_TRUE( false ); 
		}
		catch(sad::InvalidCastException ex)
		{
			ASSERT_EQUAL(ex.fromName(), "DirectDescendant1");
			ASSERT_EQUAL(ex.toName() , "InheritedFrom1");
		}
   }

} test1;

int main(int argc, char ** argv)
{
   /**
    * Run all of the registered tpunit++ tests. Returns 0 if
    * all tests are successful, otherwise returns the number
    * of failing assertions.
    */
   int result = tpunit::Tests::Run();
   return result;
}