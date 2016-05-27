#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <iostream>
#include "texturemappedfont.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "3rdparty/format/format.h"
#include "db/save.h"
#include "layouts/grid.h"
#include "sprite2d.h"
#include "renderer.h"
#include "db/dbdatabase.h"
#include "db/dbtable.h"
#include "fuzzyequal.h"
#pragma warning(pop)



/*!
 * Tests building grid via adding an objects to it
 */
struct SadGridTests : tpunit::TestFixture
{
 public:
   SadGridTests() : tpunit::TestFixture(
       TEST(SadGridTests::testSetRows),
       TEST(SadGridTests::testSetColumns),
       TEST(SadGridTests::testAddOneSpriteInOneCellTable),
       TEST(SadGridTests::testAddAndRemoveOneSpriteInOneCellTable),
       TEST(SadGridTests::testAddOneSpriteInTwoRowsTable),
       TEST(SadGridTests::testAddOneSpriteInTwoColumnsTable),
       TEST(SadGridTests::testAddTwoSpritesInFourRowsTable),
       TEST(SadGridTests::testAddTwoSpritesInFourColumnsTable),
       TEST(SadGridTests::testAddTwoSpritesInThreeRowsFourColumnsTable),
       TEST(SadGridTests::testAddTwoSpritesExceedingFixedWidthOfTable),
       TEST(SadGridTests::testLayoutingWhenFixedWidthAndHeightAndContentIsLesser),
       TEST(SadGridTests::testHorizontalLayoutAndPadding),
       TEST(SadGridTests::testVerticalLayoutAndPadding),
       TEST(SadGridTests::testNoStackingLayoutAndPadding)
   ) {}
   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
   void testSetRows()
   {
       sad::layouts::Grid* grid = new sad::layouts::Grid(); 
       grid->setFixedWidth(false);
       grid->setFixedHeight(false);
       grid->setRows(1);
       grid->setColumns(1);
       ASSERT_TRUE( grid->cell(0, 0) != NULL );
       ASSERT_TRUE( grid->cell(1, 0) == NULL );

       grid->setRows(2);

       ASSERT_TRUE( grid->cell(0, 0) != NULL );
       ASSERT_TRUE( grid->cell(1, 0) != NULL );

       grid->setRows(1);

       ASSERT_TRUE( grid->cell(0, 0) != NULL );
       ASSERT_TRUE( grid->cell(1, 0) == NULL );

       delete grid;
   }
   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
   void testSetColumns()
   {
       sad::layouts::Grid* grid = new sad::layouts::Grid(); 
       grid->setFixedWidth(false);
       grid->setFixedHeight(false);
       grid->setRows(1);
       grid->setColumns(1);
       ASSERT_TRUE( grid->cell(0, 0) != NULL );
       ASSERT_TRUE( grid->cell(0, 1) == NULL );

       grid->setColumns(2);

       ASSERT_TRUE( grid->cell(0, 0) != NULL );
       ASSERT_TRUE( grid->cell(0, 1) != NULL );

       grid->setColumns(1);

       ASSERT_TRUE( grid->cell(0, 0) != NULL );
       ASSERT_TRUE( grid->cell(0, 1) == NULL );


       grid->setRows(4);
       grid->setColumns(5);

       grid->setColumns(3);

       ASSERT_TRUE( grid->allocatedCellCount() == 12 );

       delete grid;
   }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    /*! A simple basic use case: a sprite is being added to a grid with non-fixed width
        and non-fixed height table with one cell
     */
    void testAddOneSpriteInOneCellTable()
    {
        sad::Sprite2D* sprite = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 800, 800));
        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(false);
        grid->setFixedHeight(false);
        grid->setRows(1);
        grid->setColumns(1);
        grid->setArea(sad::Rect2D(800, 800, 800, 800));
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);

        sprite->setTable(tbl);
        grid->setTable(tbl);

        tbl->add(sprite);
        tbl->add(grid);

        grid->cell(0, 0)->addChild(sprite->MajorId);

        sad::Rect2D rct = grid->area();
        if (!sad::equal(rct, sad::Rect2D(800, 400, 1200, 800)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }

        rct = grid->cell(0,0)->AssignedArea;

        if (!sad::equal(rct, sad::Rect2D(800, 400, 1200, 800)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }

        rct = sprite->area();
        if (!sad::equal(rct, sad::Rect2D(800, 400, 1200, 800)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    /*! A simple basic use case: a sprite is being added to a grid with non-fixed width
        and non-fixed height table with one cell and then removed. Grid should restore it's initial zero size
     */
    void testAddAndRemoveOneSpriteInOneCellTable()
    {
        sad::Sprite2D* sprite = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 800, 800));
        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(false);
        grid->setFixedHeight(false);
        grid->setRows(1);
        grid->setColumns(1);
        grid->setArea(sad::Rect2D(800, 800, 800, 800));
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);

        sprite->setTable(tbl);
        grid->setTable(tbl);

        tbl->add(sprite);
        tbl->add(grid);

        grid->cell(0, 0)->addChild(sprite->MajorId);
        grid->cell(0,0)->removeChild(0);
        sad::Rect2D rct = grid->area();
        if (!sad::equal(rct, sad::Rect2D(800, 800, 800, 800)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }

        rct = grid->cell(0,0)->AssignedArea;

        if (!sad::equal(rct, sad::Rect2D(800, 800, 800, 800)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    /*! A simple basic use case: a sprite is being added to a grid with fixed width
        and height. Grid should allow a sprite to be placed in grid and fill a cell
     */
    void testAddOneSpriteInTwoRowsTable()
    {
        sad::Sprite2D* sprite = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 700));
        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(true);
        grid->setFixedHeight(true);
        grid->setRows(2);
        grid->setColumns(1);
        grid->setArea(sad::Rect2D(0, 0, 200, 400));
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);

        sprite->setTable(tbl);
        grid->setTable(tbl);

        tbl->add(sprite);
        tbl->add(grid);

        grid->cell(0, 0)->addChild(sprite->MajorId);
        sad::Rect2D rct = grid->cell(0,0)->AssignedArea;
        if (!sad::equal(rct, sad::Rect2D(0, 100, 200, 400)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }

        rct = grid->cell(1,0)->AssignedArea;
        if (!sad::equal(rct, sad::Rect2D(0, 0, 200, 100)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    /*! A simple basic use case: a sprite is being added to a grid with fixed width
        and height. Grid should allow a sprite to be placed in grid and fill a cell
     */
   void testAddOneSpriteInTwoColumnsTable()
   {
        sad::Sprite2D* sprite = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 700, 600));
        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(true);
        grid->setFixedHeight(true);
        grid->setRows(1);
        grid->setColumns(2);
        grid->setArea(sad::Rect2D(0, 0, 400, 200));
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);

        sprite->setTable(tbl);
        grid->setTable(tbl);

        tbl->add(sprite);
        tbl->add(grid);

        grid->cell(0, 0)->addChild(sprite->MajorId);
        sad::Rect2D rct = grid->cell(0,0)->AssignedArea;
        if (!sad::equal(rct, sad::Rect2D(0, 0, 300, 200)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }

        rct = grid->cell(0,1)->AssignedArea;
        if (!sad::equal(rct, sad::Rect2D(300, 0, 400, 200)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }       
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    /*! A simple basic use case: a two sprites is being added to a grid with fixed width
        and height. Grid should allow a sprite to be placed in grid and fill a cell
     */
   void testAddTwoSpritesInFourRowsTable()
   {
        sad::Sprite2D* sprite1 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 700));
        sad::Sprite2D* sprite2 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 700));
        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(true);
        grid->setFixedHeight(true);
        grid->setRows(4);
        grid->setColumns(1);
        grid->setArea(sad::Rect2D(0, 0, 200, 800));
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);

        sprite1->setTable(tbl);
        sprite2->setTable(tbl);
        grid->setTable(tbl);

        tbl->add(sprite1);
        tbl->add(sprite2);
        tbl->add(grid);

        grid->cell(0, 0)->addChild(sprite1->MajorId);
        grid->cell(2, 0)->addChild(sprite2->MajorId);

        sad::Rect2D rct = grid->cell(0,0)->AssignedArea;
        if (!sad::equal(rct, sad::Rect2D(0, 500, 200, 800)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }

        rct = grid->cell(1,0)->AssignedArea;
        if (!sad::equal(rct, sad::Rect2D(0, 400, 200, 500)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }

        rct = grid->cell(2,0)->AssignedArea;
        if (!sad::equal(rct, sad::Rect2D(0, 100, 200, 400)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }

        rct = grid->cell(3, 0)->AssignedArea;
        if (!sad::equal(rct, sad::Rect2D(0, 0, 200, 100)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    /*! A simple basic use case: a two sprites is being added to a grid with fixed width
        and height. Grid should allow a sprite to be placed in grid and fill a cell
     */
   void testAddTwoSpritesInFourColumnsTable()
   {
        sad::Sprite2D* sprite1 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 700, 600));
        sad::Sprite2D* sprite2 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 700, 600));
        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(true);
        grid->setFixedHeight(true);
        grid->setRows(1);
        grid->setColumns(4);
        grid->setArea(sad::Rect2D(0, 0, 800, 200));
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);

        sprite1->setTable(tbl);
        sprite2->setTable(tbl);
        grid->setTable(tbl);

        tbl->add(sprite1);
        tbl->add(sprite2);
        tbl->add(grid);

        grid->cell(0, 0)->addChild(sprite1->MajorId);
        grid->cell(0, 2)->addChild(sprite2->MajorId);

        sad::Rect2D rct = grid->cell(0,0)->AssignedArea;
        if (!sad::equal(rct, sad::Rect2D(0, 0, 300, 200)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }

        rct = grid->cell(0,1)->AssignedArea;
        if (!sad::equal(rct, sad::Rect2D(300, 0, 400, 200)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }

        rct = grid->cell(0,2)->AssignedArea;
        if (!sad::equal(rct, sad::Rect2D(400, 0, 700, 200)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }

        rct = grid->cell(0,3)->AssignedArea;
        if (!sad::equal(rct, sad::Rect2D(700, 0, 800, 200)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
   }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    /*! A simple basic use case: a two sprites is being added to a grid with fixed width
        and height. Grid should allow a sprite to be placed in grid and fill a cell
     */
   void testAddTwoSpritesInThreeRowsFourColumnsTable()
   {
        sad::Sprite2D* sprite1 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 600));
        sad::Sprite2D* sprite2 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 600));
        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(true);
        grid->setFixedHeight(true);
        grid->setRows(3);
        grid->setColumns(4);
        grid->setArea(sad::Rect2D(0, 0, 600, 500));
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);

        sprite1->setTable(tbl);
        sprite2->setTable(tbl);
        grid->setTable(tbl);

        tbl->add(sprite1);
        tbl->add(sprite2);
        tbl->add(grid);

        grid->cell(0, 0)->addChild(sprite1->MajorId);
        grid->cell(2, 2)->addChild(sprite2->MajorId);

        double widths[4] = {200, 100, 200, 100};
        double heights[3] = {200, 100, 200};

        for(size_t row = 0; row < 3; row++)
        {
            for(size_t col = 0; col < 4; col++)
            {
                sad::Rect2D rct = grid->cell(row, col)->AssignedArea;
                if (!sad::is_fuzzy_equal(rct.width(), widths[col]) || !sad::is_fuzzy_equal(rct.height(), heights[row]))
                {
                    std::cout << str(fmt::Format("Cell {0}:{1} resulted in {2};{3}\n") << row << col << rct.width() << rct.height());
                    ASSERT_TRUE( false );
                }
            }
        }

   }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    /*! A simple basic use case: a two sprites is being added to a grid with fixed width
        and exceed it. Somehow, they should be layouted in center if no another alignment specified
     */   
   void testAddTwoSpritesExceedingFixedWidthOfTable()
   {
        sad::Sprite2D* sprite1 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 600));
        sad::Sprite2D* sprite2 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 600));
        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(true);
        grid->setFixedHeight(true);
        grid->setRows(1);
        grid->setColumns(1);
        grid->setArea(sad::Rect2D(0, 0, 200, 200));
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);

        sprite1->setTable(tbl);
        sprite2->setTable(tbl);
        grid->setTable(tbl);

        tbl->add(sprite1);
        tbl->add(sprite2);
        tbl->add(grid);

        grid->cell(0, 0)->addChild(sprite1->MajorId);
        grid->cell(0, 0)->addChild(sprite2->MajorId);

        sad::Rect2D rct = sprite1->area();
        if (!sad::equal(rct, sad::Rect2D(-100, 0, 100, 200)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }

        rct = sprite2->area();
        if (!sad::equal(rct, sad::Rect2D(100, 0, 300, 200)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
   }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    /*! A case, when size of grid exceeds size of it's content and it has fixed width and height.
        In that case, grid should not change it's area
     */ 
   void testLayoutingWhenFixedWidthAndHeightAndContentIsLesser()
   {
        sad::Sprite2D* sprite1 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 600));
        sad::Sprite2D* sprite2 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 600));
        sad::Sprite2D* sprite3 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 600));
        sad::Sprite2D* sprite4 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 600));

        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(true);
        grid->setFixedHeight(true);
        grid->setRows(2);
        grid->setColumns(2);
        grid->setArea(sad::Rect2D(0, 0, 1000, 1000));
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);

        sprite1->setTable(tbl);
        sprite2->setTable(tbl);
        sprite3->setTable(tbl);
        sprite4->setTable(tbl);
        grid->setTable(tbl);

        tbl->add(sprite1);
        tbl->add(sprite2);
        tbl->add(grid);

        grid->cell(0, 0)->addChild(sprite1->MajorId);
        grid->cell(0, 1)->addChild(sprite2->MajorId);
        grid->cell(1, 0)->addChild(sprite3->MajorId);
        grid->cell(1, 1)->addChild(sprite4->MajorId);

        sad::Rect2D rct = grid->area();
        ASSERT_TRUE( sad::is_fuzzy_equal(rct.width(), 1000) );
        ASSERT_TRUE( sad::is_fuzzy_equal(rct.height(), 1000) );
   }
   /*! Checks horizontal layout correctness in next test
       \param[in] grid a grid
       \param[in] sprite1 a first sprite
       \param[in] sprite2 a second sprite
    */
   static void checkHorizontalLayout(sad::layouts::Grid* grid, sad::Sprite2D* sprite1, sad::Sprite2D* sprite2)
   {
        sad::Rect2D firstrect(5, 60, 105, 260);
        sad::layouts::VerticalAlignment valign = grid->cell(0, 0)->verticalAlignment();
        if (valign == sad::layouts::LVA_Bottom)
        {
            firstrect = sad::Rect2D(5, 10, 105, 210);
        } 
        if (valign == sad::layouts::LVA_Top)
        {
            firstrect = sad::Rect2D(5, 110, 105, 310);
        } 

        sad::Rect2D rct = grid->area();
        if (!sad::equal(rct, sad::Rect2D(0, 0, 315, 315)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
        rct = sprite1->area();
        if (!sad::equal(rct, firstrect))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
        rct = sprite2->area();
        if (!sad::equal(rct, sad::Rect2D(105, 10, 305, 310)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
   }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    /*! Test a horizontal layout and padding
     */  
   void testHorizontalLayoutAndPadding()
   {
        sad::Sprite2D* sprite1 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 500, 600));
        sad::Sprite2D* sprite2 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 700));
        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(false);
        grid->setFixedHeight(false);
        grid->setRows(1);
        grid->setColumns(1);
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);

        sprite1->setTable(tbl);
        sprite2->setTable(tbl);
        grid->setTable(tbl);

        tbl->add(sprite1);
        tbl->add(sprite2);
        tbl->add(grid);

        sad::layouts::Cell* cell = grid->cell(0, 0);
        cell->addChild(sprite1->MajorId);
        cell->addChild(sprite2->MajorId);

        cell->setHorizontalAlignment(sad::layouts::LHA_Middle);
        cell->setVerticalAlignment(sad::layouts::LVA_Middle);
        cell->setStackingType(sad::layouts::LST_Horizontal);
        
        cell->setPaddingLeft(5);
        cell->setPaddingRight(10);
        cell->setPaddingTop(5);
        cell->setPaddingBottom(10);

        grid->setArea(sad::Rect2D(0, 0, 315, 315));

        for(size_t halign = 0; halign < 3; halign++)
        {
            for(size_t valign = 0; valign < 3; valign++)
            {
                sad::layouts::HorizontalAlignment hhalign = static_cast<sad::layouts::HorizontalAlignment>(halign);
                sad::layouts::VerticalAlignment vvalign = static_cast<sad::layouts::VerticalAlignment>(valign);

                cell->setHorizontalAlignment(hhalign);
                cell->setVerticalAlignment(vvalign);
        
                checkHorizontalLayout(grid, sprite1, sprite2);
            }            
        }       
   }

   /*! Checks vertical layout correctness in next test
       \param[in] grid a grid
       \param[in] sprite1 a first sprite
       \param[in] sprite2 a second sprite
    */
   static void checkVerticallLayout(sad::layouts::Grid* grid, sad::Sprite2D* sprite1, sad::Sprite2D* sprite2)
   {
        // 100x200
        // 200x300
        sad::Rect2D firstrect(55, 310, 155, 510);
        sad::layouts::HorizontalAlignment halign = grid->cell(0, 0)->horizontalAlignment();
        if (halign == sad::layouts::LHA_Left)
        {
            firstrect = sad::Rect2D(5, 310, 105, 510);
        } 
        if (halign == sad::layouts::LHA_Right)
        {
            firstrect = sad::Rect2D(105, 310, 205, 510);
        } 

        sad::Rect2D rct = grid->area();
        if (!sad::equal(rct, sad::Rect2D(0, 0, 215, 515)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
        rct = sprite1->area();
        if (!sad::equal(rct, firstrect))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
        rct = sprite2->area();
        if (!sad::equal(rct, sad::Rect2D(5, 10, 205, 310)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
   }

   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
   /*! Test a vertical layout and padding
    */  
   void testVerticalLayoutAndPadding()
   {
        sad::Sprite2D* sprite1 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 500, 600));
        sad::Sprite2D* sprite2 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 700));
        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(false);
        grid->setFixedHeight(false);
        grid->setRows(1);
        grid->setColumns(1);
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);

        sprite1->setTable(tbl);
        sprite2->setTable(tbl);
        grid->setTable(tbl);

        tbl->add(sprite1);
        tbl->add(sprite2);
        tbl->add(grid);

        sad::layouts::Cell* cell = grid->cell(0, 0);
        cell->addChild(sprite1->MajorId);
        cell->addChild(sprite2->MajorId);

        cell->setHorizontalAlignment(sad::layouts::LHA_Middle);
        cell->setVerticalAlignment(sad::layouts::LVA_Middle);
        cell->setStackingType(sad::layouts::LST_Vertical);
        
        cell->setPaddingLeft(5);
        cell->setPaddingRight(10);
        cell->setPaddingTop(5);
        cell->setPaddingBottom(10);

        grid->setArea(sad::Rect2D(0, 0, 215, 515));

        for(size_t halign = 0; halign < 3; halign++)
        {
            for(size_t valign = 0; valign < 3; valign++)
            {
                sad::layouts::HorizontalAlignment hhalign = static_cast<sad::layouts::HorizontalAlignment>(halign);
                sad::layouts::VerticalAlignment vvalign = static_cast<sad::layouts::VerticalAlignment>(valign);

                cell->setHorizontalAlignment(hhalign);
                cell->setVerticalAlignment(vvalign);
        
                checkVerticallLayout(grid, sprite1, sprite2);
            }            
        }       
   }

   /*! Checks no stacking layout correctness in next test
       \param[in] grid a grid
       \param[in] sprite1 a first sprite
       \param[in] sprite2 a second sprite
    */
   static void checkNoStackinglLayout(sad::layouts::Grid* grid, sad::Sprite2D* sprite1, sad::Sprite2D* sprite2)
   {
        // 100x200
        // 200x300
        double x1 = 55, x2 = 155, y1 = 60, y2 = 260;
        sad::layouts::HorizontalAlignment halign = grid->cell(0, 0)->horizontalAlignment();
        sad::layouts::VerticalAlignment valign = grid->cell(0, 0)->verticalAlignment();
        if (halign == sad::layouts::LHA_Left)
        {
            x1 = 5; x2 = 105; 
        } 
        if (halign == sad::layouts::LHA_Right)
        {
            x1 = 105; x2 = 205;
        } 

        if (valign == sad::layouts::LVA_Top)
        {
            y1 = 110; y2 = 310; 
        } 
        if (valign == sad::layouts::LVA_Bottom)
        {
            y1 = 10; y2 = 210;
        } 
        sad::Rect2D firstrect(x1, y1, x2, y2);
        sad::Rect2D rct = grid->area();
        if (!sad::equal(rct, sad::Rect2D(0, 0, 215, 315)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
        rct = sprite1->area();
        if (!sad::equal(rct, firstrect))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
        rct = sprite2->area();
        if (!sad::equal(rct, sad::Rect2D(5, 10, 205, 310)))
        {
            std::cout << str(fmt::Format("Resulted in {0};{1};{2};{3}\n") << rct[0].x() << rct[0].y() << rct.width() << rct.height());
            ASSERT_TRUE( false );
        }
   }


   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
   /*! Test a no stacking layout and padding
    */  
   void testNoStackingLayoutAndPadding()
   {
        sad::Sprite2D* sprite1 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 500, 600));
        sad::Sprite2D* sprite2 = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 600, 700));
        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(false);
        grid->setFixedHeight(false);
        grid->setRows(1);
        grid->setColumns(1);
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);

        sprite1->setTable(tbl);
        sprite2->setTable(tbl);
        grid->setTable(tbl);

        tbl->add(sprite1);
        tbl->add(sprite2);
        tbl->add(grid);

        sad::layouts::Cell* cell = grid->cell(0, 0);
        cell->addChild(sprite1->MajorId);
        cell->addChild(sprite2->MajorId);

        cell->setHorizontalAlignment(sad::layouts::LHA_Middle);
        cell->setVerticalAlignment(sad::layouts::LVA_Middle);
        cell->setStackingType(sad::layouts::LST_NoStacking);
        
        cell->setPaddingLeft(5);
        cell->setPaddingRight(10);
        cell->setPaddingTop(5);
        cell->setPaddingBottom(10);

        grid->setArea(sad::Rect2D(0, 0, 215, 315));

        for(size_t halign = 0; halign < 3; halign++)
        {
            for(size_t valign = 0; valign < 3; valign++)
            {
                sad::layouts::HorizontalAlignment hhalign = static_cast<sad::layouts::HorizontalAlignment>(halign);
                sad::layouts::VerticalAlignment vvalign = static_cast<sad::layouts::VerticalAlignment>(valign);

                cell->setHorizontalAlignment(hhalign);
                cell->setVerticalAlignment(vvalign);
        
                checkNoStackinglLayout(grid, sprite1, sprite2);
            }            
        }       
   }

} _sad_grid_tests;
