#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "db/dbvariant.h"
#include "db/dbobject.h"
#include "db/dbtable.h"
#include "db/dbdatabase.h"
#include "db/schema/schema.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbobjectfactory.h"
#include "db/dblink.h"

#include "renderer.h"

#include "mock3.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


/*!
 * Tests sad::db::Link
 */
struct SadDbLinkTest : tpunit::TestFixture
{
public:
    SadDbLinkTest() : tpunit::TestFixture(		
        TEST(SadDbLinkTest::test)
    ) { }
    
    void test()
    {
    
        sad::db::Database db;
        db.addTable("table", new sad::db::Table());
        
        Mock3 * mock = new Mock3();
        mock->setObjectName("test");
        mock->MajorId = 1;
        mock->MinorId = 1;
        db.table("table")->add(mock);
        
        sad::db::Link link;
        ASSERT_TRUE( link.get() == nullptr );
        
        link.setDatabase(&db);
        link.setTableName("table");
        
        ASSERT_TRUE( link.get() == nullptr );
        
        link.setMinorId(1);
        ASSERT_TRUE( link.get() != nullptr );
    
        sad::db::Link link2;
        link2.setTable(db.table("table"));
        link2.setName("test");
        ASSERT_TRUE( link2.get() != nullptr );
    }
    
} _sad_db_link_test;