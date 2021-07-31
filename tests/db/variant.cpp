#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "texturemappedfont.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "db/save.h"
#include "db/load.h"
#include "sadpair.h"
#pragma warning(pop)



/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct SadDbVariantTest : tpunit::TestFixture
{
 public:
   SadDbVariantTest() : tpunit::TestFixture(
       TEST(SadDbVariantTest::test),
       TEST(SadDbVariantTest::testObject),
       TEST(SadDbVariantTest::testVectorVectorAColor),
       TEST(SadDbVariantTest::testConstChar),
       TEST(SadDbVariantTest::testPairsTripletsQuadruplets),
       TEST(SadDbVariantTest::testVectorOfTriplets)
   ) {}

    void test()
    {
        sad::db::Variant v((int)22);

        ASSERT_TRUE(v.get<int>().value() == 22);
        ASSERT_TRUE(v.get<unsigned int>().value() == 22);

        v.set<int>(25);

        ASSERT_TRUE(v.get<int>().value() == 25);
    }

    void testObject()
    {
        sad::ClassMetaData * meta = sad::TextureMappedFont::globalMetaData();
        ASSERT_TRUE(meta != nullptr);

        sad::Font * font = new sad::TextureMappedFont();

        sad::db::Variant v(font);

        sad::Maybe<sad::Object *> test = v.get<sad::Object*>();
        ASSERT_TRUE(test.exists());
        ASSERT_TRUE(test.value() == (sad::Object *)font);

        ASSERT_TRUE(v.get<sad::Font*>().value() == font);
        delete font;
    }

    void testVectorVectorAColor()
    {
        sad::Vector<sad::Vector<sad::AColor> > v;

        v.push_back(sad::Vector<sad::AColor>());
        v[0].push_back(sad::AColor(5, 5, 5, 0));
        v[0].push_back(sad::AColor(5, 5, 5, 0));
        v[0].push_back(sad::AColor(5, 5, 5, 0));

        v.push_back(sad::Vector<sad::AColor>());
        v[1].push_back(sad::AColor(5, 5, 5, 0));
        v[1].push_back(sad::AColor(5, 5, 5, 0));
        v[1].push_back(sad::AColor(5, 5, 5, 0));

        sad::db::Variant k(v);

        picojson::value saved;
        saved = k.save();

        sad::Vector<sad::Vector<sad::AColor> > tmp;
        sad::db::Variant r(tmp);
        
        ASSERT_TRUE( r.load(saved) );

        sad::Maybe<sad::Vector<sad::Vector<sad::AColor> > > vk = r.get<sad::Vector<sad::Vector<sad::AColor> > >();
        ASSERT_TRUE(vk.exists());
        
        sad::Vector<sad::Vector<sad::AColor> > vk_value = vk.value();
        ASSERT_TRUE(vk_value.size() == 2);
    }
    
    void testConstChar()
    {
        sad::db::Variant a("ab");
        ASSERT_TRUE( a.get<sad::String>().value() == "ab" );
    }
    
    void testPairsTripletsQuadruplets()
    {
        sad::db::TypeName<sad::Pair<int, int> >::init();
        sad::String result1 = sad::db::TypeName<sad::Pair<int, int> >::name();
        ASSERT_TRUE( result1 == "sad::Pair<int,int>");
        
        sad::db::TypeName<sad::Triplet<int, float, double> >::init();
        sad::String result2 = sad::db::TypeName<sad::Triplet<int, float, double> >::name();
        ASSERT_TRUE( result2 == "sad::Triplet<int,float,double>");

        sad::db::TypeName<sad::Quadruplet<int, float, double, bool> >::init();
        sad::String result3 = sad::db::TypeName<sad::Quadruplet<int, float, double, bool> >::name();
        ASSERT_TRUE( result3 == "sad::Quadruplet<int,float,double,bool>");		
    }
    
    void testVectorOfTriplets()
    {
        sad::Vector<sad::Triplet<unsigned int, unsigned int, unsigned int> > v;

        v.push_back(sad::Triplet<unsigned int, unsigned int, unsigned int>(1,2,3));
        v.push_back(sad::Triplet<unsigned int, unsigned int, unsigned int>(4,5,6));
        
        sad::db::Variant k(v);

        picojson::value saved;
        saved = k.save();

        sad::Vector<sad::Triplet<unsigned int, unsigned int, unsigned int> > tmp;
        sad::db::Variant r(tmp);
        
        ASSERT_TRUE( r.load(saved) );

        sad::Maybe<sad::Vector<sad::Triplet<unsigned int, unsigned int, unsigned int> > > vk = r.get<sad::Vector<sad::Triplet<unsigned int, unsigned int, unsigned int> > >();
        ASSERT_TRUE(vk.exists());
        
        sad::Vector<sad::Triplet<unsigned int, unsigned int, unsigned int> > vk_value = vk.value();
        ASSERT_TRUE(vk_value.size() == 2);
        ASSERT_TRUE(vk_value[0].p1() == 1);
        ASSERT_TRUE(vk_value[0].p2() == 2);
        ASSERT_TRUE(vk_value[0].p3() == 3);

        ASSERT_TRUE(vk_value[1].p1() == 4);
        ASSERT_TRUE(vk_value[1].p2() == 5);
        ASSERT_TRUE(vk_value[1].p3() == 6);		
    }

} _sad_db_variant;
