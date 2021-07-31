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
       TEST(SadGridTests::testNoStackingLayoutAndPadding),
       TEST(SadGridTests::testNoStackingLayoutAndPadding),
       TEST(SadGridTests::testSimpleMergeAndSplit),
       TEST(SadGridTests::testMergeOnTop),
       TEST(SadGridTests::testMergeOnMiddle),
       TEST(SadGridTests::testSplitOnTop),
       TEST(SadGridTests::testSplitOnMiddle),
       TEST(SadGridTests::testMergeCommon),
       TEST(SadGridTests::testSplitCommon)
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
       ASSERT_TRUE( grid->cell(0, 0) != nullptr );
       ASSERT_TRUE( grid->cell(1, 0) == nullptr );

       grid->setRows(2);

       ASSERT_TRUE( grid->cell(0, 0) != nullptr );
       ASSERT_TRUE( grid->cell(1, 0) != nullptr );

       grid->setRows(1);

       ASSERT_TRUE( grid->cell(0, 0) != nullptr );
       ASSERT_TRUE( grid->cell(1, 0) == nullptr );

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
       ASSERT_TRUE( grid->cell(0, 0) != nullptr );
       ASSERT_TRUE( grid->cell(0, 1) == nullptr );

       grid->setColumns(2);

       ASSERT_TRUE( grid->cell(0, 0) != nullptr );
       ASSERT_TRUE( grid->cell(0, 1) != nullptr );

       grid->setColumns(1);

       ASSERT_TRUE( grid->cell(0, 0) != nullptr );
       ASSERT_TRUE( grid->cell(0, 1) == nullptr );


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
        if (valign == sad::layouts::VerticalAlignment::LVA_Bottom)
        {
            firstrect = sad::Rect2D(5, 10, 105, 210);
        } 
        if (valign == sad::layouts::VerticalAlignment::LVA_Top)
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

        cell->setHorizontalAlignment(sad::layouts::HorizontalAlignment::LHA_Middle);
        cell->setVerticalAlignment(sad::layouts::VerticalAlignment::LVA_Middle);
        cell->setStackingType(sad::layouts::StackingType::LST_Horizontal);
        
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
        if (halign == sad::layouts::HorizontalAlignment::LHA_Left)
        {
            firstrect = sad::Rect2D(5, 310, 105, 510);
        } 
        if (halign == sad::layouts::HorizontalAlignment::LHA_Right)
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

        cell->setHorizontalAlignment(sad::layouts::HorizontalAlignment::LHA_Middle);
        cell->setVerticalAlignment(sad::layouts::VerticalAlignment::LVA_Middle);
        cell->setStackingType(sad::layouts::StackingType::LST_Vertical);
        
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
        if (halign == sad::layouts::HorizontalAlignment::LHA_Left)
        {
            x1 = 5; x2 = 105; 
        } 
        if (halign == sad::layouts::HorizontalAlignment::LHA_Right)
        {
            x1 = 105; x2 = 205;
        } 

        if (valign == sad::layouts::VerticalAlignment::LVA_Top)
        {
            y1 = 110; y2 = 310; 
        } 
        if (valign == sad::layouts::VerticalAlignment::LVA_Bottom)
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

        cell->setHorizontalAlignment(sad::layouts::HorizontalAlignment::LHA_Middle);
        cell->setVerticalAlignment(sad::layouts::VerticalAlignment::LVA_Middle);
        cell->setStackingType(sad::layouts::StackingType::LST_NoStacking);
        
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

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    /*! Test simple merge and split
     */
    void testSimpleMergeAndSplit()
    {
        sad::Sprite2D* sprite = new sad::Sprite2D("test", sad::Rect2D(0, 0, 800, 600), sad::Rect2D(400, 400, 800, 800));
        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(true);
        grid->setFixedHeight(true);
        grid->setRows(2);
        grid->setColumns(3);
        grid->setArea(sad::Rect2D(0, 0, 800, 800));
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

        grid->cell(1, 1)->addChild(sprite->MajorId);
        bool b = grid->merge(0, 0, 2, 2);
        ASSERT_TRUE( b ) ;
        unsigned long long cmi = grid->cell(0, 0)->childMajorId(0); 
        ASSERT_TRUE( cmi == sprite->MajorId );
        ASSERT_TRUE( grid->allocatedCellCount() == 3 );
        ASSERT_TRUE( grid->cell(0, 0)->rowSpan() == 2 );
        ASSERT_TRUE( grid->cell(0, 0)->colSpan() == 2 );
        ASSERT_TRUE( grid->cell(0, 2)->rowSpan() == 1 );
        ASSERT_TRUE( grid->cell(0, 2)->colSpan() == 1 );
        ASSERT_TRUE( grid->cell(1, 2)->rowSpan() == 1 );
        ASSERT_TRUE( grid->cell(1, 2)->colSpan() == 1 );

        b = grid->split(0, 0, 2, 2);
        ASSERT_TRUE( b ) ;
        ASSERT_TRUE( grid->cell(0, 0)->childMajorId(0) == sprite->MajorId );
        ASSERT_TRUE( grid->allocatedCellCount() == 6 );
        for(size_t i = 0; i < 2; i++)
        {
            for(size_t j = 0; j < 3; j++)
            {
                ASSERT_TRUE( grid->cell(i, j)->rowSpan() == 1 );
                ASSERT_TRUE( grid->cell(i, j)->colSpan() == 1 );        
            }
        }
    }
   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
   /*! Tests to sequential merges, when top first point of merge was on top of node
    */
   void testMergeOnTop()
   {
       const size_t tests = 8;
       size_t merges[tests][4] = {
           {0, 0, 4, 4},
           {0, 0, 6, 4},
           {0, 0, 4, 6},
           {0, 0, 6, 6},
           {0, 4, 4, 3},
           {0, 4, 4, 1},
           {0, 4, 6, 1},
           {0, 4, 6, 3}
       };
       sad::Vector< sad::Vector<sad::Rect2I > > results;

       sad::Vector<sad::Rect2I > firsttest;
       firsttest << sad::Rect2I(0, 0, 3, 2);
       firsttest << sad::Rect2I(0, 3, 2, 3);
       results << firsttest;

       sad::Vector<sad::Rect2I > secondtest;
       secondtest << sad::Rect2I(0, 0, 5, 2);
       secondtest << sad::Rect2I(0, 3, 2, 3);
       secondtest << sad::Rect2I(5, 3, 5, 3);
       results << secondtest;

       sad::Vector<sad::Rect2I > thirdtest;
       thirdtest << sad::Rect2I(0, 0, 3, 2);
       thirdtest << sad::Rect2I(0, 3, 2, 4);
       thirdtest << sad::Rect2I(0, 5, 3, 5);
       results << thirdtest;

       sad::Vector<sad::Rect2I > fourthtest;
       fourthtest << sad::Rect2I(0, 0, 5, 2);
       fourthtest << sad::Rect2I(0, 3, 2, 4);
       fourthtest << sad::Rect2I(5, 3, 5, 4);
       fourthtest << sad::Rect2I(0, 5, 5, 5);
       results << fourthtest;

       sad::Vector<sad::Rect2I > fifthtest;
       fifthtest << sad::Rect2I(0, 4, 2, 4);
       fifthtest << sad::Rect2I(0, 5, 3, 6);
       results << fifthtest;

       sad::Vector<sad::Rect2I > sixthtest;
       sixthtest << sad::Rect2I(0, 4, 2, 4);
       results << sixthtest;

       sad::Vector<sad::Rect2I > seventhtest;
       seventhtest << sad::Rect2I(0, 4, 2, 4);
       seventhtest << sad::Rect2I(5, 4, 5, 4);
       results << seventhtest;

       sad::Vector<sad::Rect2I > eighthtest;
       eighthtest << sad::Rect2I(0, 4, 2, 4);
       eighthtest << sad::Rect2I(0, 5, 5, 6);
       eighthtest << sad::Rect2I(5, 4, 5, 4);
       results << eighthtest;

       for(size_t test  = 0; test < tests; test++)
       {
            printf("Starting test: %u\n", test + 1);
            sad::layouts::Grid* grid = new sad::layouts::Grid();
            grid->setFixedWidth(true);
            grid->setFixedHeight(true);
            grid->setRows(7);
            grid->setColumns(7);
            grid->setArea(sad::Rect2D(0, 0, 800, 800));
            sad::db::Database* db = new sad::db::Database();
            sad::db::Table* tbl = new sad::db::Table();
            sad::Renderer r;
            r.addDatabase("", db);
            db->setRenderer(&r);
            tbl->setDatabase(db);
            db->addTable("", tbl);
            grid->setTable(tbl);
            tbl->add(grid);

            bool b = grid->merge(merges[test][0], merges[test][1], merges[test][2], merges[test][3]);
            ASSERT_TRUE( b ) ;
            ASSERT_TRUE( grid->allocatedCellCount() == (49 - merges[test][2] * merges[test][3]) + 1 );
            ASSERT_TRUE( grid->cell(merges[test][0], merges[test][1])->rowSpan() == merges[test][2] );
            ASSERT_TRUE( grid->cell(merges[test][0], merges[test][1])->colSpan() == merges[test][3] );
            for(size_t row = 0; row < 7; row++)
            {
                for(size_t col = 0; col < 7; col++)
                {
                    bool w = (row >= merges[test][0] && col >= merges[test][1] && row <= (merges[test][0] + merges[test][2]) && col <= (merges[test][1] + merges[test][3]));
                    if (!w)
                    {
                        ASSERT_TRUE( grid->cell(row, col)->rowSpan() == 1 );
                        ASSERT_TRUE( grid->cell(row, col)->colSpan() == 1 );
                    }
                }
            }

            b = grid->merge(3, 3, 2, 2);
            ASSERT_TRUE( b ) ;
            size_t count = 4;
            size_t addedcellcount = 1 + results[test].size();
            sad::layouts::Cell* cell  = grid->cell(3, 3);

            ASSERT_TRUE( cell->rowSpan() == 2 );
            ASSERT_TRUE( cell->colSpan() == 2 );
            for(size_t i = 0; i < results[test].size(); i++)
            {
                size_t row = results[test][i].p0().x();
                size_t col = results[test][i].p0().y();
                size_t row_span = results[test][i].p2().x() - results[test][i].p0().x() + 1;
                size_t col_span = results[test][i].p2().y() - results[test][i].p0().y() + 1;
                count += row_span * col_span;

                sad::layouts::Cell* pwcell  = grid->cell(row, col);
                if (pwcell->rowSpan() != row_span || pwcell->colSpan() != col_span)
                {
                    printf("Incorrect output: %u;%u;%u;%u\n", row, col, pwcell->rowSpan(), pwcell->colSpan());
                }
                ASSERT_TRUE( pwcell->rowSpan() == row_span );
                ASSERT_TRUE( pwcell->colSpan() == col_span );
            }
            ASSERT_TRUE( grid->allocatedCellCount() == (49 - count + addedcellcount ) );
            for(size_t row = 0; row < 7; row++)
            {
                for(size_t col = 0; col < 7; col++)
                {
                    bool w = false;
                    for(size_t i = 0; i < results[test].size(); i++)
                    {
                        size_t rowc = results[test][i].p0().x();
                        size_t colc = results[test][i].p0().y();
                        size_t row_span = results[test][i].p2().x() - results[test][i].p0().x() + 1;
                        size_t col_span = results[test][i].p2().y() - results[test][i].p0().y() + 1;
                        w = w || (row >= rowc && col >= colc && row <= (rowc + row_span - 1) && col <= (colc + col_span - 1));
                    }
                    w = w || (row >= 3 && col >= 3 && row <= 4 && col <= 4);
                    if (!w)
                    {
                        sad::layouts::Cell* pcell = grid->cell(row, col); 
                        ASSERT_TRUE( pcell->rowSpan() == 1 );
                        ASSERT_TRUE( pcell->colSpan() == 1 );
                    }
                }
            }
       }
   }

   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
   /*! Tests to sequential merges, when top first point of merge was on middle of second node
    */
   void testMergeOnMiddle()
   {
       const size_t tests = 12;
       size_t merges[tests][4] = {
           {3, 0, 1, 4},  //0
           {3, 0, 1, 6},  //1
           {3, 0, 4, 5},  //2
           {3, 0, 4, 6},  //3
           {3, 3, 2, 2},  //4
           {3, 3, 2, 4},  //5
           {3, 3, 1, 1},  //6
           {3, 4, 1, 1},  //7
           {4, 3, 1, 1},  //8
           {4, 4, 1, 1},  //9
           {4, 4, 3, 1},  //10
           {4, 4, 3, 3}   //11
       };
       sad::Vector< sad::Vector<sad::Rect2I > > results;

       sad::Vector<sad::Rect2I > firsttest;
       firsttest << sad::Rect2I(3, 0, 3, 2);
       results << firsttest;

       sad::Vector<sad::Rect2I > secondtest;
       secondtest << sad::Rect2I(3, 0, 3, 2);
       secondtest << sad::Rect2I(3, 5, 3, 5);
       results << secondtest;

       sad::Vector<sad::Rect2I > thirdtest;
       thirdtest << sad::Rect2I(3, 0, 6, 2);
       thirdtest << sad::Rect2I(5, 3, 6, 4);
       results << thirdtest;

       sad::Vector<sad::Rect2I > fourthtest;
       fourthtest << sad::Rect2I(3, 0, 6, 2);
       fourthtest << sad::Rect2I(5, 3, 6, 4);
       fourthtest << sad::Rect2I(3, 5, 6, 5);
       results << fourthtest;

       sad::Vector<sad::Rect2I > fifthtest;
       results << fifthtest;

       sad::Vector<sad::Rect2I > sixthtest;
       sixthtest << sad::Rect2I(3, 5, 4, 6);
       results << sixthtest;

       results << sad::Vector<sad::Rect2I >(); // 7
       results << sad::Vector<sad::Rect2I >(); // 8
       results << sad::Vector<sad::Rect2I >(); // 9
       results << sad::Vector<sad::Rect2I >(); // 10

       sad::Vector<sad::Rect2I > eleventhtest;
       eleventhtest << sad::Rect2I(5, 4, 6, 4);
       results << eleventhtest;

       sad::Vector<sad::Rect2I > twelfthtest;
       twelfthtest << sad::Rect2I(5, 4, 6, 4);
       twelfthtest << sad::Rect2I(4, 5, 6, 6);
       results << twelfthtest;

       for(size_t test  = 0; test < tests; test++)
       {
            printf("Starting test: %u\n", test + 1);
            sad::layouts::Grid* grid = new sad::layouts::Grid();
            grid->setFixedWidth(true);
            grid->setFixedHeight(true);
            grid->setRows(7);
            grid->setColumns(7);
            grid->setArea(sad::Rect2D(0, 0, 800, 800));
            sad::db::Database* db = new sad::db::Database();
            sad::db::Table* tbl = new sad::db::Table();
            sad::Renderer r;
            r.addDatabase("", db);
            db->setRenderer(&r);
            tbl->setDatabase(db);
            db->addTable("", tbl);
            grid->setTable(tbl);
            tbl->add(grid);

            bool b = grid->merge(merges[test][0], merges[test][1], merges[test][2], merges[test][3]);
            ASSERT_TRUE( b ) ;
            ASSERT_TRUE( grid->allocatedCellCount() == (49 - merges[test][2] * merges[test][3]) + 1 );
            ASSERT_TRUE( grid->cell(merges[test][0], merges[test][1])->rowSpan() == merges[test][2] );
            ASSERT_TRUE( grid->cell(merges[test][0], merges[test][1])->colSpan() == merges[test][3] );
            for(size_t row = 0; row < 7; row++)
            {
                for(size_t col = 0; col < 7; col++)
                {
                    bool w = (row >= merges[test][0] && col >= merges[test][1] && row <= (merges[test][0] + merges[test][2]) && col <= (merges[test][1] + merges[test][3]));
                    if (!w)
                    {
                        ASSERT_TRUE( grid->cell(row, col)->rowSpan() == 1 );
                        ASSERT_TRUE( grid->cell(row, col)->colSpan() == 1 );
                    }
                }
            }

            b = grid->merge(3, 3, 2, 2);
            ASSERT_TRUE( b ) ;
            size_t count = 4;
            size_t addedcellcount = 1 + results[test].size();
            sad::layouts::Cell* cell  = grid->cell(3, 3);

            ASSERT_TRUE( cell->rowSpan() == 2 );
            ASSERT_TRUE( cell->colSpan() == 2 );
            for(size_t i = 0; i < results[test].size(); i++)
            {
                size_t row = results[test][i].p0().x();
                size_t col = results[test][i].p0().y();
                size_t row_span = results[test][i].p2().x() - results[test][i].p0().x() + 1;
                size_t col_span = results[test][i].p2().y() - results[test][i].p0().y() + 1;
                count += row_span * col_span;

                sad::layouts::Cell* pwcell  = grid->cell(row, col);
                if (pwcell->rowSpan() != row_span || pwcell->colSpan() != col_span)
                {
                    printf("Incorrect output: %u;%u;%u;%u\n", row, col, pwcell->rowSpan(), pwcell->colSpan());
                }
                ASSERT_TRUE( pwcell->rowSpan() == row_span );
                ASSERT_TRUE( pwcell->colSpan() == col_span );
            }
            size_t ethalon = (49 - count + addedcellcount );
            if (grid->allocatedCellCount() != ethalon)
            {
                printf("Allocated %u cells, should %u\n", grid->allocatedCellCount(), ethalon);
            }
            ASSERT_TRUE( grid->allocatedCellCount() == ethalon );
            for(size_t row = 0; row < 7; row++)
            {
                for(size_t col = 0; col < 7; col++)
                {
                    bool w = false;
                    for(size_t i = 0; i < results[test].size(); i++)
                    {
                        size_t rowc = results[test][i].p0().x();
                        size_t colc = results[test][i].p0().y();
                        size_t row_span = results[test][i].p2().x() - results[test][i].p0().x() + 1;
                        size_t col_span = results[test][i].p2().y() - results[test][i].p0().y() + 1;
                        w = w || (row >= rowc && col >= colc && row <= (rowc + row_span - 1) && col <= (colc + col_span - 1));
                    }
                    w = w || (row >= 3 && col >= 3 && row <= 4 && col <= 4);
                    if (!w)
                    {
                        sad::layouts::Cell* pcell = grid->cell(row, col); 
                        ASSERT_TRUE( pcell->rowSpan() == 1 );
                        ASSERT_TRUE( pcell->colSpan() == 1 );
                    }
                }
            }
       }
   }

   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
   /*! Test sequential merge and split, when top first point of merge was on top of node
    */
   void testSplitOnTop()
   {
       const size_t tests = 8;
       size_t merges[tests][4] = {
           {0, 0, 4, 4},
           {0, 0, 6, 4},
           {0, 0, 4, 6},
           {0, 0, 6, 6},
           {0, 4, 4, 3},
           {0, 4, 4, 1},
           {0, 4, 6, 1},
           {0, 4, 6, 3}
       };
       sad::Vector< sad::Vector<sad::Rect2I > > results;

       sad::Vector<sad::Rect2I > firsttest;
       firsttest << sad::Rect2I(0, 0, 3, 2);
       firsttest << sad::Rect2I(0, 3, 2, 3);
       results << firsttest;

       sad::Vector<sad::Rect2I > secondtest;
       secondtest << sad::Rect2I(0, 0, 5, 2);
       secondtest << sad::Rect2I(0, 3, 2, 3);
       secondtest << sad::Rect2I(5, 3, 5, 3);
       results << secondtest;

       sad::Vector<sad::Rect2I > thirdtest;
       thirdtest << sad::Rect2I(0, 0, 3, 2);
       thirdtest << sad::Rect2I(0, 3, 2, 4);
       thirdtest << sad::Rect2I(0, 5, 3, 5);
       results << thirdtest;

       sad::Vector<sad::Rect2I > fourthtest;
       fourthtest << sad::Rect2I(0, 0, 5, 2);
       fourthtest << sad::Rect2I(0, 3, 2, 4);
       fourthtest << sad::Rect2I(5, 3, 5, 4);
       fourthtest << sad::Rect2I(0, 5, 5, 5);
       results << fourthtest;

       sad::Vector<sad::Rect2I > fifthtest;
       fifthtest << sad::Rect2I(0, 4, 2, 4);
       fifthtest << sad::Rect2I(0, 5, 3, 6);
       results << fifthtest;

       sad::Vector<sad::Rect2I > sixthtest;
       sixthtest << sad::Rect2I(0, 4, 2, 4);
       results << sixthtest;

       sad::Vector<sad::Rect2I > seventhtest;
       seventhtest << sad::Rect2I(0, 4, 2, 4);
       seventhtest << sad::Rect2I(5, 4, 5, 4);
       results << seventhtest;

       sad::Vector<sad::Rect2I > eighthtest;
       eighthtest << sad::Rect2I(0, 4, 2, 4);
       eighthtest << sad::Rect2I(0, 5, 5, 6);
       eighthtest << sad::Rect2I(5, 4, 5, 4);
       results << eighthtest;

       for(size_t test  = 0; test < tests; test++)
       {
            printf("Starting test: %u\n", test + 1);
            sad::layouts::Grid* grid = new sad::layouts::Grid();
            grid->setFixedWidth(true);
            grid->setFixedHeight(true);
            grid->setRows(7);
            grid->setColumns(7);
            grid->setArea(sad::Rect2D(0, 0, 800, 800));
            sad::db::Database* db = new sad::db::Database();
            sad::db::Table* tbl = new sad::db::Table();
            sad::Renderer r;
            r.addDatabase("", db);
            db->setRenderer(&r);
            tbl->setDatabase(db);
            db->addTable("", tbl);
            grid->setTable(tbl);
            tbl->add(grid);

            bool b = grid->merge(merges[test][0], merges[test][1], merges[test][2], merges[test][3]);
            ASSERT_TRUE( b ) ;
            ASSERT_TRUE( grid->allocatedCellCount() == (49 - merges[test][2] * merges[test][3]) + 1 );
            ASSERT_TRUE( grid->cell(merges[test][0], merges[test][1])->rowSpan() == merges[test][2] );
            ASSERT_TRUE( grid->cell(merges[test][0], merges[test][1])->colSpan() == merges[test][3] );
            for(size_t row = 0; row < 7; row++)
            {
                for(size_t col = 0; col < 7; col++)
                {
                    bool w = (row >= merges[test][0] && col >= merges[test][1] && row <= (merges[test][0] + merges[test][2]) && col <= (merges[test][1] + merges[test][3]));
                    if (!w)
                    {
                        ASSERT_TRUE( grid->cell(row, col)->rowSpan() == 1 );
                        ASSERT_TRUE( grid->cell(row, col)->colSpan() == 1 );
                    }
                }
            }

            b = grid->split(3, 3, 2, 2);
            ASSERT_TRUE( b ) ;
            size_t count = 4;
            size_t addedcellcount = 4 + results[test].size();

            for(size_t i = 0; i < results[test].size(); i++)
            {
                size_t row = results[test][i].p0().x();
                size_t col = results[test][i].p0().y();
                size_t row_span = results[test][i].p2().x() - results[test][i].p0().x() + 1;
                size_t col_span = results[test][i].p2().y() - results[test][i].p0().y() + 1;
                count += row_span * col_span;

                sad::layouts::Cell* pwcell  = grid->cell(row, col);
                if (pwcell->rowSpan() != row_span || pwcell->colSpan() != col_span)
                {
                    printf("Incorrect output: %u;%u;%u;%u\n", row, col, pwcell->rowSpan(), pwcell->colSpan());
                }
                ASSERT_TRUE( pwcell->rowSpan() == row_span );
                ASSERT_TRUE( pwcell->colSpan() == col_span );
            }
            ASSERT_TRUE( grid->allocatedCellCount() == (49 - count + addedcellcount ) );
            for(size_t row = 0; row < 7; row++)
            {
                for(size_t col = 0; col < 7; col++)
                {
                    bool w = false;
                    for(size_t i = 0; i < results[test].size(); i++)
                    {
                        size_t rowc = results[test][i].p0().x();
                        size_t colc = results[test][i].p0().y();
                        size_t row_span = results[test][i].p2().x() - results[test][i].p0().x() + 1;
                        size_t col_span = results[test][i].p2().y() - results[test][i].p0().y() + 1;
                        w = w || (row >= rowc && col >= colc && row <= (rowc + row_span - 1) && col <= (colc + col_span - 1));
                    }
                    if (!w)
                    {
                        sad::layouts::Cell* pcell = grid->cell(row, col); 
                        ASSERT_TRUE( pcell->rowSpan() == 1 );
                        ASSERT_TRUE( pcell->colSpan() == 1 );
                    }
                }
            }
       }
   }


   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
   /*! Test sequential merge and split, when top first point of merge was on middle of node
    */
   void testSplitOnMiddle()
   {
       const size_t tests = 12;
       size_t merges[tests][4] = {
           {3, 0, 1, 4},  //0
           {3, 0, 1, 6},  //1
           {3, 0, 4, 5},  //2
           {3, 0, 4, 6},  //3
           {3, 3, 2, 2},  //4
           {3, 3, 2, 4},  //5
           {3, 3, 1, 1},  //6
           {3, 4, 1, 1},  //7
           {4, 3, 1, 1},  //8
           {4, 4, 1, 1},  //9
           {4, 4, 3, 1},  //10
           {4, 4, 3, 3}   //11
       };
       sad::Vector< sad::Vector<sad::Rect2I > > results;

       sad::Vector<sad::Rect2I > firsttest;
       firsttest << sad::Rect2I(3, 0, 3, 2);
       results << firsttest;

       sad::Vector<sad::Rect2I > secondtest;
       secondtest << sad::Rect2I(3, 0, 3, 2);
       secondtest << sad::Rect2I(3, 5, 3, 5);
       results << secondtest;

       sad::Vector<sad::Rect2I > thirdtest;
       thirdtest << sad::Rect2I(3, 0, 6, 2);
       thirdtest << sad::Rect2I(5, 3, 6, 4);
       results << thirdtest;

       sad::Vector<sad::Rect2I > fourthtest;
       fourthtest << sad::Rect2I(3, 0, 6, 2);
       fourthtest << sad::Rect2I(5, 3, 6, 4);
       fourthtest << sad::Rect2I(3, 5, 6, 5);
       results << fourthtest;

       sad::Vector<sad::Rect2I > fifthtest;
       results << fifthtest;

       sad::Vector<sad::Rect2I > sixthtest;
       sixthtest << sad::Rect2I(3, 5, 4, 6);
       results << sixthtest;

       results << sad::Vector<sad::Rect2I >(); // 7
       results << sad::Vector<sad::Rect2I >(); // 8
       results << sad::Vector<sad::Rect2I >(); // 9
       results << sad::Vector<sad::Rect2I >(); // 10

       sad::Vector<sad::Rect2I > eleventhtest;
       eleventhtest << sad::Rect2I(5, 4, 6, 4);
       results << eleventhtest;

       sad::Vector<sad::Rect2I > twelfthtest;
       twelfthtest << sad::Rect2I(5, 4, 6, 4);
       twelfthtest << sad::Rect2I(4, 5, 6, 6);
       results << twelfthtest;

       for(size_t test  = 0; test < tests; test++)
       {
            printf("Starting test: %u\n", test + 1);
            sad::layouts::Grid* grid = new sad::layouts::Grid();
            grid->setFixedWidth(true);
            grid->setFixedHeight(true);
            grid->setRows(7);
            grid->setColumns(7);
            grid->setArea(sad::Rect2D(0, 0, 800, 800));
            sad::db::Database* db = new sad::db::Database();
            sad::db::Table* tbl = new sad::db::Table();
            sad::Renderer r;
            r.addDatabase("", db);
            db->setRenderer(&r);
            tbl->setDatabase(db);
            db->addTable("", tbl);
            grid->setTable(tbl);
            tbl->add(grid);

            bool b = grid->merge(merges[test][0], merges[test][1], merges[test][2], merges[test][3]);
            ASSERT_TRUE( b ) ;
            ASSERT_TRUE( grid->allocatedCellCount() == (49 - merges[test][2] * merges[test][3]) + 1 );
            ASSERT_TRUE( grid->cell(merges[test][0], merges[test][1])->rowSpan() == merges[test][2] );
            ASSERT_TRUE( grid->cell(merges[test][0], merges[test][1])->colSpan() == merges[test][3] );
            for(size_t row = 0; row < 7; row++)
            {
                for(size_t col = 0; col < 7; col++)
                {
                    bool w = (row >= merges[test][0] && col >= merges[test][1] && row <= (merges[test][0] + merges[test][2]) && col <= (merges[test][1] + merges[test][3]));
                    if (!w)
                    {
                        ASSERT_TRUE( grid->cell(row, col)->rowSpan() == 1 );
                        ASSERT_TRUE( grid->cell(row, col)->colSpan() == 1 );
                    }
                }
            }

            b = grid->split(3, 3, 2, 2);
            ASSERT_TRUE( b ) ;
            size_t count = 4;
            size_t addedcellcount = 4 + results[test].size();

            for(size_t i = 0; i < results[test].size(); i++)
            {
                size_t row = results[test][i].p0().x();
                size_t col = results[test][i].p0().y();
                size_t row_span = results[test][i].p2().x() - results[test][i].p0().x() + 1;
                size_t col_span = results[test][i].p2().y() - results[test][i].p0().y() + 1;
                count += row_span * col_span;

                sad::layouts::Cell* pwcell  = grid->cell(row, col);
                if (pwcell->rowSpan() != row_span || pwcell->colSpan() != col_span)
                {
                    printf("Incorrect output: %u;%u;%u;%u\n", row, col, pwcell->rowSpan(), pwcell->colSpan());
                }
                ASSERT_TRUE( pwcell->rowSpan() == row_span );
                ASSERT_TRUE( pwcell->colSpan() == col_span );
            }
            ASSERT_TRUE( grid->allocatedCellCount() == (49 - count + addedcellcount ) );
            for(size_t row = 0; row < 7; row++)
            {
                for(size_t col = 0; col < 7; col++)
                {
                    bool w = false;
                    for(size_t i = 0; i < results[test].size(); i++)
                    {
                        size_t rowc = results[test][i].p0().x();
                        size_t colc = results[test][i].p0().y();
                        size_t row_span = results[test][i].p2().x() - results[test][i].p0().x() + 1;
                        size_t col_span = results[test][i].p2().y() - results[test][i].p0().y() + 1;
                        w = w || (row >= rowc && col >= colc && row <= (rowc + row_span - 1) && col <= (colc + col_span - 1));
                    }
                    if (!w)
                    {
                        sad::layouts::Cell* pcell = grid->cell(row, col); 
                        ASSERT_TRUE( pcell->rowSpan() == 1 );
                        ASSERT_TRUE( pcell->colSpan() == 1 );
                    }
                }
            }
       }
   }

   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
   /*! Test common merge attempt
    */
   void testMergeCommon()
   {
        sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(true);
        grid->setFixedHeight(true);
        grid->setRows(6);
        grid->setColumns(6);
        grid->setArea(sad::Rect2D(0, 0, 800, 800));
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);
        grid->setTable(tbl);
        tbl->add(grid);

        grid->merge(0, 0, 3, 3);
        grid->merge(0, 3, 3, 3);
        grid->merge(3, 0, 3, 3);
        grid->merge(3, 3, 3, 3);

        ASSERT_TRUE( grid->allocatedCellCount() == 4 );

        sad::layouts::Cell* cell = grid->cell(0, 0);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 3 );

        cell = grid->cell(0, 3);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 3 );

        cell = grid->cell(3, 0);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 3 );

        cell = grid->cell(3, 3);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 3 );

        grid->merge(2, 2, 2, 2);

        ASSERT_TRUE( grid->allocatedCellCount() == 9 );

        cell = grid->cell(0, 0);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 2 );

        cell = grid->cell(0, 2);
        ASSERT_TRUE( cell->rowSpan() == 2 );
        ASSERT_TRUE( cell->colSpan() == 1 );

        cell = grid->cell(0, 3);
        ASSERT_TRUE( cell->rowSpan() == 2 );
        ASSERT_TRUE( cell->colSpan() == 1 );

        cell = grid->cell(0, 4);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 2 );

        cell = grid->cell(3, 0);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 2 );

        cell = grid->cell(4, 2);
        ASSERT_TRUE( cell->rowSpan() == 2 );
        ASSERT_TRUE( cell->colSpan() == 1 );

        cell = grid->cell(4, 3);
        ASSERT_TRUE( cell->rowSpan() == 2 );
        ASSERT_TRUE( cell->colSpan() == 1 );

        cell = grid->cell(3, 4);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 2 );

        cell = grid->cell(2, 2);
        ASSERT_TRUE( cell->rowSpan() == 2 );
        ASSERT_TRUE( cell->colSpan() == 2 );
   }
   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
   /*! Test common split attempt
    */
   void testSplitCommon()
   {
       sad::layouts::Grid* grid = new sad::layouts::Grid();
        grid->setFixedWidth(true);
        grid->setFixedHeight(true);
        grid->setRows(6);
        grid->setColumns(6);
        grid->setArea(sad::Rect2D(0, 0, 800, 800));
        sad::db::Database* db = new sad::db::Database();
        sad::db::Table* tbl = new sad::db::Table();
        sad::Renderer r;
        r.addDatabase("", db);
        db->setRenderer(&r);
        tbl->setDatabase(db);
        db->addTable("", tbl);
        grid->setTable(tbl);
        tbl->add(grid);

        grid->merge(0, 0, 3, 3);
        grid->merge(0, 3, 3, 3);
        grid->merge(3, 0, 3, 3);
        grid->merge(3, 3, 3, 3);

        ASSERT_TRUE( grid->allocatedCellCount() == 4 );

        sad::layouts::Cell* cell = grid->cell(0, 0);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 3 );

        cell = grid->cell(0, 3);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 3 );

        cell = grid->cell(3, 0);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 3 );

        cell = grid->cell(3, 3);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 3 );

        grid->split(2, 2, 2, 2);

        ASSERT_TRUE( grid->allocatedCellCount() == 12 );

        cell = grid->cell(0, 0);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 2 );

        cell = grid->cell(0, 2);
        ASSERT_TRUE( cell->rowSpan() == 2 );
        ASSERT_TRUE( cell->colSpan() == 1 );

        cell = grid->cell(0, 3);
        ASSERT_TRUE( cell->rowSpan() == 2 );
        ASSERT_TRUE( cell->colSpan() == 1 );

        cell = grid->cell(0, 4);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 2 );

        cell = grid->cell(3, 0);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 2 );

        cell = grid->cell(4, 2);
        ASSERT_TRUE( cell->rowSpan() == 2 );
        ASSERT_TRUE( cell->colSpan() == 1 );

        cell = grid->cell(4, 3);
        ASSERT_TRUE( cell->rowSpan() == 2 );
        ASSERT_TRUE( cell->colSpan() == 1 );

        cell = grid->cell(3, 4);
        ASSERT_TRUE( cell->rowSpan() == 3 );
        ASSERT_TRUE( cell->colSpan() == 2 );

        cell = grid->cell(2, 2);
        ASSERT_TRUE( cell->rowSpan() == 1 );
        ASSERT_TRUE( cell->colSpan() == 1 );

        cell = grid->cell(2, 3);
        ASSERT_TRUE( cell->rowSpan() == 1 );
        ASSERT_TRUE( cell->colSpan() == 1 );

        cell = grid->cell(3, 2);
        ASSERT_TRUE( cell->rowSpan() == 1 );
        ASSERT_TRUE( cell->colSpan() == 1 );

        cell = grid->cell(3, 3);
        ASSERT_TRUE( cell->rowSpan() == 1 );
        ASSERT_TRUE( cell->colSpan() == 1 );
   }

} _sad_grid_tests;
