#include "dukpp-03/context.h"
#include "dukpp-03/renderer.h"

#include <geometry2d.h>
#include <fuzzyequal.h>
#include <renderer.h>

#include <db/dbtable.h>

#include <layouts/lengthvalue.h>
#include <layouts/serializablecell.h>
#include <layouts/cell.h>
#include <layouts/grid.h>

#include <cassert>

#define PERFORM_AND_ASSERT(X)   {bool b = ctx->eval(X); assert(b); }

static sad::layouts::Grid::SearchResult make1()
{
    return sad::layouts::Grid::SearchResult(0,0);
}

static sad::layouts::Grid::SearchResult make2(size_t a1, size_t a2)
{
    return sad::layouts::Grid::SearchResult(a1, a2);
}

static void exposeCell(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();

    c->addMethod("getProperty", sad::dukpp03::bind_method::from(&sad::layouts::Cell::getProperty));
    c->addMethod("setProperty", sad::dukpp03::bind_method::from(&sad::layouts::Cell::setProperty));
    //c->addMethod("setProperty", sad::dukpp03::bind_expl::with_args_and_no_return_type2<sad::layouts::Cell,const sad::String&, const sad::db::Variant&>::from(&sad::layouts::Cell::setProperty));



    c->addAccessor("Rendered", sad::dukpp03::getter::from(&sad::layouts::Cell::Rendered), sad::dukpp03::setter::from(&sad::layouts::Cell::Rendered));
    c->addAccessor("Row", sad::dukpp03::getter::from(&sad::layouts::Cell::Row), sad::dukpp03::setter::from(&sad::layouts::Cell::Row));
    c->addAccessor("Col", sad::dukpp03::getter::from(&sad::layouts::Cell::Col), sad::dukpp03::setter::from(&sad::layouts::Cell::Col));

    c->setPrototypeFunction("sad.layouts.Cell");

    ctx->addClassBinding("sad::layouts::Cell", c);

    PERFORM_AND_ASSERT(
        "sad.layouts.Cell = function() {};"
    );
}

static void exposeGrid(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::layouts::Grid>("SadLayoutsGrid");

    c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setObjectName));
    c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::layouts::Grid::objectName));

    c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::layouts::Grid::MajorId), sad::dukpp03::setter::from(&sad::layouts::Grid::MajorId));
    c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::layouts::Grid::MinorId), sad::dukpp03::setter::from(&sad::layouts::Grid::MinorId));

    sad::layouts::Cell* (sad::layouts::Grid::*cell1)(unsigned int row, unsigned int col) = &sad::layouts::Grid::cell;
    sad::layouts::Cell* (sad::layouts::Grid::*cell2)(size_t pos) const = &sad::layouts::Grid::cell;

    c->addMethod("cell", sad::dukpp03::bind_method::from(cell1));
    c->addMethod("cell", sad::dukpp03::bind_method::from(cell2));

    c->addMethod("_setTreeName", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setTreeName));
    c->addMethod("renderer", sad::dukpp03::bind_method::from(&sad::layouts::Grid::renderer));
    c->addMethod("render", sad::dukpp03::bind_method::from(&sad::layouts::Grid::render));
    c->addMethod("renderWithColor", sad::dukpp03::bind_method::from(&sad::layouts::Grid::renderWithColor));
    c->addMethod("regions", sad::dukpp03::bind_method::from(&sad::layouts::Grid::getRegions));
    c->addMethod("getRegions", sad::dukpp03::bind_method::from(&sad::layouts::Grid::getRegions));

    c->addMethod("setArea", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setArea));
    c->addMethod("area", sad::dukpp03::bind_method::from(&sad::layouts::Grid::area));

    c->addMethod("setRows", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setRows));
    c->addMethod("rows", sad::dukpp03::bind_method::from(&sad::layouts::Grid::rows));
    
    c->addMethod("setColumns", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setColumns));
    c->addMethod("columns", sad::dukpp03::bind_method::from(&sad::layouts::Grid::columns));

    c->addMethod("setPaddingTop", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setPaddingTop));
    c->addMethod("setDefaultPaddingTop", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setDefaultPaddingTop));
    c->addMethod("paddingTop", sad::dukpp03::bind_method::from(&sad::layouts::Grid::paddingTop));

    c->addMethod("setPaddingBottom", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setPaddingBottom));
    c->addMethod("setDefaultPaddingBottom", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setDefaultPaddingBottom));
    c->addMethod("paddingBottom", sad::dukpp03::bind_method::from(&sad::layouts::Grid::paddingBottom));

    c->addMethod("setPaddingLeft", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setPaddingLeft));
    c->addMethod("setDefaultPaddingLeft", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setDefaultPaddingLeft));
    c->addMethod("paddingLeft", sad::dukpp03::bind_method::from(&sad::layouts::Grid::paddingLeft));

    c->addMethod("setPaddingRight", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setPaddingRight));
    c->addMethod("setDefaultPaddingRight", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setDefaultPaddingRight));
    c->addMethod("paddingRight", sad::dukpp03::bind_method::from(&sad::layouts::Grid::paddingRight));

    c->addMethod("setFixedWidth", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setFixedWidth));
    c->addMethod("fixedWidth", sad::dukpp03::bind_method::from(&sad::layouts::Grid::fixedWidth));

    c->addMethod("setFixedHeight", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setFixedHeight));
    c->addMethod("fixedHeight", sad::dukpp03::bind_method::from(&sad::layouts::Grid::fixedHeight));

    c->addMethod("setRenderColor", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setRenderColor));
    c->addMethod("renderColor", sad::dukpp03::bind_method::from(&sad::layouts::Grid::renderColor));

    c->addMethod("cells", sad::dukpp03::bind_method::from(&sad::layouts::Grid::cells));
    c->addMethod("setCells", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setCells));

    c->addMethod("moveBy", sad::dukpp03::bind_method::from(&sad::layouts::Grid::moveBy));
    
    sad::Maybe<sad::layouts::Grid::SearchResult> (sad::layouts::Grid::*find1)(sad::SceneNode* node) const = sad::layouts::Grid::find;
    sad::Maybe<sad::layouts::Grid::SearchResult> (sad::layouts::Grid::*find2)(unsigned long long major_id) const = sad::layouts::Grid::find;

    c->addMethod("find", sad::dukpp03::bind_method::from(find1));
    c->addMethod("find", sad::dukpp03::bind_method::from(find2));

    c->addMethod("merge", sad::dukpp03::bind_method::from(&sad::layouts::Grid::merge));
    c->addMethod("split", sad::dukpp03::bind_method::from(&sad::layouts::Grid::split));

    c->addMethod("update", sad::dukpp03::bind_method::from(&sad::layouts::Grid::update));
    c->addMethod("children", sad::dukpp03::bind_method::from(&sad::layouts::Grid::children));
    c->addMethod("childrenMajorIds", sad::dukpp03::bind_method::from(&sad::layouts::Grid::childrenMajorIds));
    c->addMethod("allocatedCellCount", sad::dukpp03::bind_method::from(&sad::layouts::Grid::allocatedCellCount));
    c->addMethod("setTable", sad::dukpp03::bind_method::from(&sad::layouts::Grid::setTable));


    c->setPrototypeFunction("SadLayoutsGrid");

    ctx->addClassBinding("sad::layouts::Grid", c);

    PERFORM_AND_ASSERT(
        "sad.layouts.Grid = SadLayoutsGrid;"
        "sad.layouts.Grid.prototype.setTreeName = function(o, a) {"
        "    if (typeof a == \"undefined\") this._setTreeName(o, \"\"); else this._setTreeName(o, a);"
        "}"
    );
}

void sad::dukpp03::exposeLayouts(sad::dukpp03::Context* ctx)
{
    PERFORM_AND_ASSERT(
        "sad.layouts = {};"

        "sad.layouts.HorizontalAlignment = {};"
        "sad.layouts.HorizontalAlignment.LHA_Left = 0;"
        "sad.layouts.HorizontalAlignment.LHA_Middle = 1;"
        "sad.layouts.HorizontalAlignment.LHA_Right = 2;"
        "sad.layouts.LHA_Left = 0;"
        "sad.layouts.LHA_Middle = 1;"
        "sad.layouts.LHA_Right = 2;"

        "sad.layouts.VerticalAlignment = {};"
        "sad.layouts.VerticalAlignment.LVA_Top = 0;"
        "sad.layouts.VerticalAlignment.LVA_Middle = 1;"
        "sad.layouts.VerticalAlignment.LVA_Bottom = 2;"
        "sad.layouts.LVA_Top = 0;"
        "sad.layouts.LVA_Middle = 1;"
        "sad.layouts.LVA_Bottom = 2;"

        "sad.layouts.StackingType = {};"
        "sad.layouts.StackingType.LST_Horizontal = 0;"
        "sad.layouts.StackingType.LST_Vertical = 1;"
        "sad.layouts.StackingType.LST_NoStacking = 2;"
        "sad.layouts.LST_Horizontal = 0;"
        "sad.layouts.LST_Vertical = 1;"
        "sad.layouts.LST_NoStacking = 2;"


        "sad.layouts.Unit = {};"
        "sad.layouts.Unit.LU_Auto = 0;"
        "sad.layouts.Unit.LU_Pixels = 1;"
        "sad.layouts.Unit.LU_Percents = 2;"
        "sad.layouts.LU_Auto = 0;"
        "sad.layouts.LU_Pixels = 1;"
        "sad.layouts.LU_Percents = 2;"
    );

    // Expose sad::layouts::LengthValue
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::layouts::LengthValue>("SadLayoutsLengthValue");
        c->addConstructor<sad::layouts::LengthValue, sad::layouts::Unit, double>("SadLayoutsLengthValue");

        c->addAccessor("Unit", sad::dukpp03::getter::from(&sad::layouts::LengthValue::Unit), sad::dukpp03::setter::from(&sad::layouts::LengthValue::Unit));
        c->addAccessor("Value", sad::dukpp03::getter::from(&sad::layouts::LengthValue::Value), sad::dukpp03::setter::from(&sad::layouts::LengthValue::Value));

        c->setPrototypeFunction("SadLayoutsLengthValue");

        ctx->addClassBinding("sad::layouts::LengthValue", c);


        PERFORM_AND_ASSERT(
            "sad.layouts.LengthValue = SadLayoutsLengthValue;"
            "sad.layouts.LengthValue.prototype.toString = function() {  var o = [\"sad::layouts::LU_Auto\", \"sad::layouts::LU_Pixels\", \"sad::layouts::LU_Percents\"]; return \"sad::layouts::LengthValue(\"  + o[this.Unit] + \",\"  + this.Value +  \")\"; }"
        );
    }


    // Expose sad::layouts::SerializableCell
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::layouts::SerializableCell>("SadLayoutsSerializableCell");

        c->addAccessor("Width", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::Width), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::Width));
        c->addAccessor("Height", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::Height), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::Height));

        c->addAccessor("RowSpan", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::RowSpan), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::RowSpan));
        c->addAccessor("ColSpan", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::ColSpan), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::ColSpan));

        c->addAccessor("VAlign", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::VAlign), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::VAlign));
        c->addAccessor("HAlign", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::HAlign), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::HAlign));
        
        c->addAccessor("Children", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::Children), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::Children));
        
        c->addAccessor("StackingType", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::StackingType), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::StackingType));
        c->addAccessor("PaddingTop", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::PaddingTop), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::PaddingTop));
        c->addAccessor("PaddingBottom", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::PaddingBottom), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::PaddingBottom));
        c->addAccessor("PaddingLeft", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::PaddingLeft), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::PaddingLeft));
        c->addAccessor("PaddingRight", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::PaddingRight), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::PaddingRight));

        c->addAccessor("AssignedArea", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::AssignedArea), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::AssignedArea));
        
        c->addAccessor("Row", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::Row), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::Row));
        c->addAccessor("Col", sad::dukpp03::getter::from(&sad::layouts::SerializableCell::Col), sad::dukpp03::setter::from(&sad::layouts::SerializableCell::Col));


        c->setPrototypeFunction("SadLayoutsSerializableCell");

        ctx->addClassBinding("sad::layouts::SerializableCell", c);


        PERFORM_AND_ASSERT(
            "sad.layouts.SerializableCell = SadLayoutsSerializableCell;"
            "sad.layouts.SerializableCell.prototype.toString = SadLayoutsSerializableCellToString"
        );
    }

    exposeGrid(ctx);
    exposeCell(ctx);

    {

        ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
        overload->add(sad::dukpp03::make_function::from(make1));
        overload->add(sad::dukpp03::make_function::from(make2));
        ctx->registerCallable("SadLayoutsGridSearchResultMake", overload);

        PERFORM_AND_ASSERT(
            "sad.layouts.Grid.SearchResult = {};"
            "sad.layouts.Grid.SearchResult.make = SadLayoutsGridSearchResultMake;"            
        );
    }
}

