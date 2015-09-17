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
#include "db/dbpopulatescenesfromdatabase.h"

#include "label.h"
#include "scene.h"

#include "renderer.h"

#include "mock3.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


class PopulateSceneFromDatabaseTest: public tpunit::TestFixture
{
public:
    PopulateSceneFromDatabaseTest() : tpunit::TestFixture(		
        TEST(PopulateSceneFromDatabaseTest::test)
    ) { }

    void test()
    {
        sad::Renderer r;
        sad::db::Database db;
        db.addTable("scenes", new sad::db::Table());
        db.addTable("scenenodes", new sad::db::Table());

        sad::Scene * s = new sad::Scene();
        s->MajorId = 1;
        s->MinorId = 1;
        s->setSceneLayer(2);
        db.table("scenes")->add(s);

        s = new sad::Scene();
        s->MajorId = 2;
        s->MinorId = 2;
        s->setSceneLayer(1);
        db.table("scenes")->add(s);

        s = new sad::Scene();
        s->MajorId = 3;
        s->MinorId = 3;
        s->setSceneLayer(0);
        db.table("scenes")->add(s);

        sad::Label* l = new sad::Label();
        l->MinorId = 1;
        l->MajorId  = 4;
        l->setCachedLayer(2);
        l->setCachedSceneId(1);
        db.table("scenenodes")->add(l);


        l = new sad::Label();
        l->MinorId = 2;
        l->MajorId  = 5;
        l->setCachedLayer(1);
        l->setCachedSceneId(1);
        db.table("scenenodes")->add(l);

        l = new sad::Label();
        l->MinorId = 3;
        l->MajorId  = 6;
        l->setCachedLayer(0);
        l->setCachedSceneId(1);
        db.table("scenenodes")->add(l);


        sad::db::populateScenesFromDatabase(&r, &db);


        ASSERT_TRUE(static_cast<sad::Scene*>(db.table("scenes")->queryByMinorId(1))->sceneLayer() == 2);
        ASSERT_TRUE(static_cast<sad::Scene*>(db.table("scenes")->queryByMinorId(2))->sceneLayer() == 1);
        ASSERT_TRUE(static_cast<sad::Scene*>(db.table("scenes")->queryByMinorId(3))->sceneLayer() == 0);
        ASSERT_TRUE(static_cast<sad::Scene*>(db.table("scenes")->queryByMinorId(1))->objectCount() == 3);

        ASSERT_TRUE(static_cast<sad::Label*>(db.table("scenenodes")->queryByMinorId(1))->cachedLayer() == 2);
        ASSERT_TRUE(static_cast<sad::Label*>(db.table("scenenodes")->queryByMinorId(2))->cachedLayer() == 1);
        ASSERT_TRUE(static_cast<sad::Label*>(db.table("scenenodes")->queryByMinorId(3))->cachedLayer() == 0);
    }

} _populate_scene_from_database_test;