#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "db/dbfield.h"
#include "db/dbobject.h"
#include "db/dbmethodpair.h"
#include "mock.h"
#include "mock2.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbstoredproperty.h"
#include "db/dbstoredpropertyfactory.h"
#include "db/custom/customobject.h"
#include "db/custom/customschema.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct SadDbPropertyTest : tpunit::TestFixture
{
 public:
   SadDbPropertyTest() : tpunit::TestFixture(
       TEST(SadDbPropertyTest::testField),
       TEST(SadDbPropertyTest::testField_check),
       TEST(SadDbPropertyTest::testMethodPair),
       TEST(SadDbPropertyTest::testMethodPair_check),
       TEST(SadDbPropertyTest::testMarkAsDefault_valid),
       TEST(SadDbPropertyTest::testMarkAsDefault_replace),
       TEST(SadDbPropertyTest::testMarkAsDefault_wrongtype),
       TEST(SadDbPropertyTest::testMarkAsDefault_nullptr),
       TEST(SadDbPropertyTest::testSadStringToStdString),
       TEST(SadDbPropertyTest::testStdStringToSadString)	   
   ) {}

    int test;

    void testField()
    {
        Mock m;
        
        sad::db::Property * test1 = new sad::db::Field<Mock, int>(&Mock::m_id);
        sad::db::Variant  k;
        test1->set(&m, sad::db::Variant(3));
        test1->get(&m,k);
        ASSERT_TRUE(k.get<int>().value() == 3);

        delete test1;
    }

    void testField_check()
    {
        sad::db::Property * test1 = new sad::db::Field<Mock, int>(&Mock::m_id);
        {
            picojson::value v(picojson::object_type, false);
            v.insert("key", picojson::value(22.0));
            ASSERT_TRUE( test1->check("key", v) );
            ASSERT_FALSE( test1->check("key2", v) );
        }
        {
            picojson::value v(picojson::object_type, false);
            v.insert("key", picojson::value("truly non-integral value"));
            ASSERT_FALSE( test1->check("key", v) );
        }

        delete test1;
    }

    void processProperty(Mock &m, sad::db::Property * prop)
    {       
        test++;

        prop->set(&m, test);
        sad::db::Variant k;
        prop->get(&m, k);
        ASSERT_TRUE(k.get<int>().value() == test);

        delete prop;
    }

    void testMethodPair()
    {
        Mock m;

        test = 3;

        sad::db::Property * p = new sad::db::MethodPair<Mock, int>(&Mock::id, &Mock::setId);
        processProperty(m, p);

        p = new sad::db::MethodPair<Mock, int>(&Mock::id_c, &Mock::setId);
        processProperty(m, p);

        p = new sad::db::MethodPair<Mock, int>(&Mock::id_r, &Mock::setId);
        processProperty(m, p);

        p = new sad::db::MethodPair<Mock, int>(&Mock::id_rc, &Mock::setId);
        processProperty(m, p);

        p = new sad::db::MethodPair<Mock, int>(&Mock::id_cr, &Mock::setId);
        processProperty(m, p);

        p = new sad::db::MethodPair<Mock, int>(&Mock::id_crc, &Mock::setId);
        processProperty(m, p);

        p = new sad::db::MethodPair<Mock, int>(&Mock::id, &Mock::setIdC);
        processProperty(m, p);

        p = new sad::db::MethodPair<Mock, int>(&Mock::id, &Mock::setIdR);
        processProperty(m, p);

        p = new sad::db::MethodPair<Mock, int>(&Mock::id, &Mock::setIdRC);
        processProperty(m, p);

        p = new sad::db::MethodPair<Mock, int>(&Mock::id, &Mock::setIdCR);
        processProperty(m, p);

        p = new sad::db::MethodPair<Mock, int>(&Mock::id, &Mock::setIdCRC);
        processProperty(m, p);
    }

    void testMethodPair_check()
    {
        sad::db::Property * p = new sad::db::MethodPair<Mock, int>(&Mock::id, &Mock::setId);

        {
            picojson::value v(picojson::object_type, false);
            v.insert("key", picojson::value(22.0));
            ASSERT_TRUE( p->check("key", v) );
            ASSERT_FALSE( p->check("key2", v) );
        }
        {
            picojson::value v(picojson::object_type, false);
            v.insert("key", picojson::value("truly non-integral value"));
            ASSERT_FALSE( p->check("key", v) );
        }

        delete p;
    }
    
    void testMarkAsDefault_valid()
    {
        sad::db::Property * p = new sad::db::Field<Mock, int>(&Mock::m_id);
        ASSERT_TRUE( p->makeNonRequiredWithDefaultValue(new sad::db::Variant(0)) );
        ASSERT_TRUE( p->hasDefaultValue() );
        ASSERT_TRUE( p->defaultValue() != nullptr );
                
        delete p;
    }
    
    void testMarkAsDefault_replace()
    {
        sad::db::Property * p = new sad::db::Field<Mock, int>(&Mock::m_id);
        ASSERT_TRUE( p->makeNonRequiredWithDefaultValue(new sad::db::Variant(0)) );
        ASSERT_TRUE( p->hasDefaultValue() );
        ASSERT_TRUE( p->defaultValue() != nullptr );

        ASSERT_TRUE( p->makeNonRequiredWithDefaultValue(new sad::db::Variant(3)) );
        ASSERT_TRUE( p->hasDefaultValue() );
        ASSERT_TRUE( p->defaultValue()->get<int>().value() == 3 );
        
        delete p;
    }
    
    void testMarkAsDefault_wrongtype()
    {
        sad::db::Property * p = new sad::db::Field<Mock, int>(&Mock::m_id);
        sad::db::Variant* v = new sad::db::Variant(sad::String("33"));
        ASSERT_TRUE( p->makeNonRequiredWithDefaultValue(v) == false);
        ASSERT_TRUE( p->hasDefaultValue() == false );
        ASSERT_TRUE( p->defaultValue() == nullptr );
        delete v;               
        delete p;
    }
    
    void testMarkAsDefault_nullptr()
    {
        sad::db::Property * p = new sad::db::Field<Mock, int>(&Mock::m_id);
        ASSERT_TRUE( p->makeNonRequiredWithDefaultValue(nullptr) == false);
        ASSERT_TRUE( p->hasDefaultValue() == false );
        ASSERT_TRUE( p->defaultValue() == nullptr );
        delete p;
    }
    
    void testSadStringToStdString()
    {
        sad::db::custom::Object o;
        o.schema()->add("prop", new sad::db::StoredProperty<std::string>());
        o.setProperty("prop", sad::String("test"));
        ASSERT_TRUE(  o.getProperty<std::string>("prop").value() == "test" );
    }
    
    void testStdStringToSadString()
    {
        sad::db::custom::Object o;
        o.schema()->add("prop", new sad::db::StoredProperty<sad::String>());
        o.setProperty("prop", std::string("test"));
        ASSERT_TRUE(  o.getProperty<sad::String>("prop").value() == "test" );		
    }

} _sad_property;