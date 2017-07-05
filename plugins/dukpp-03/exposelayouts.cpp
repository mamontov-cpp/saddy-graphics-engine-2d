#include "dukpp-03/context.h"
#include "dukpp-03/renderer.h"

#include <geometry2d.h>
#include <fuzzyequal.h>
#include <renderer.h>

#include <db/dbdatabase.h>
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

template<typename _ReturnType>
struct acc
{

template<_ReturnType (sad::layouts::Cell::*Field)>
static _ReturnType get(sad::layouts::Cell* c)
{
    return c->*Field;
}


template<_ReturnType(sad::layouts::Cell::*Field)>
static void set(sad::layouts::Cell* c, _ReturnType v)
{
    c->*Field = v;
}


};


static void exposeCell(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();

    c->addMethod("getProperty", sad::dukpp03::bind_method::from(&sad::layouts::Cell::getProperty));
    {
        std::function<void(sad::layouts::Cell*, const sad::String&, const sad::db::Variant&)> lambda1 = [](sad::layouts::Cell* c, const sad::String& name, const sad::db::Variant& v) {
            c->setProperty(name, v);
        };
        c->addMethod("setProperty", sad::dukpp03::bind_lambda::from(lambda1));
        c->addMethod("setProperty", sad::dukpp03::bind_method::from(&sad::layouts::Cell::setProperty));
    }

    c->addMethod("setGrid", sad::dukpp03::bind_method::from(&sad::layouts::Cell::setGrid));
    {
        std::function<sad::layouts::SerializableCell(sad::layouts::Cell*)> lambda = [](sad::layouts::Cell* c) {
            sad::layouts::SerializableCell cell;
            c->toSerializable(cell);
            return cell;
        };
        c->addMethod("toSerializable", sad::dukpp03::bind_lambda::from(lambda));
    }
    c->addMethod("fromSerializable", sad::dukpp03::bind_method::from(&sad::layouts::Cell::fromSerializable));
    c->addMethod("update", sad::dukpp03::bind_method::from(&sad::layouts::Cell::update));
    {
        std::function<void(sad::layouts::Cell*, const sad::layouts::LengthValue&)> lambda = [](sad::layouts::Cell* c, const sad::layouts::LengthValue& v) {
            c->setWidth(v);
        };
        c->addMethod("setWidth", sad::dukpp03::bind_method::from(&sad::layouts::Cell::setWidth));
        c->addMethod("setWidth", sad::dukpp03::bind_lambda::from(lambda));
    }
    c->addMethod("width", sad::dukpp03::bind_method::from(&sad::layouts::Cell::width));
    {
        std::function<void(sad::layouts::Cell*, const sad::layouts::LengthValue&)> lambda = [](sad::layouts::Cell* c, const sad::layouts::LengthValue& v) {
            c->setHeight(v);
        };
        c->addMethod("setHeight", sad::dukpp03::bind_method::from(&sad::layouts::Cell::setHeight));
        c->addMethod("setHeight", sad::dukpp03::bind_lambda::from(lambda));
    }
    c->addMethod("height", sad::dukpp03::bind_method::from(&sad::layouts::Cell::height));

    c->addMethod("rowSpan", sad::dukpp03::bind_method::from(&sad::layouts::Cell::rowSpan));
    c->addMethod("colSpan", sad::dukpp03::bind_method::from(&sad::layouts::Cell::colSpan));

    {
        std::function<void(sad::layouts::Cell*, const sad::layouts::VerticalAlignment)> lambda = [](sad::layouts::Cell* c, const sad::layouts::VerticalAlignment v) {
            c->setVerticalAlignment(v);
        };
        c->addMethod("setVerticalAlignment", sad::dukpp03::bind_method::from(&sad::layouts::Cell::setVerticalAlignment));
        c->addMethod("setVerticalAlignment", sad::dukpp03::bind_lambda::from(lambda));
    }
    c->addMethod("verticalAlignment", sad::dukpp03::bind_method::from(&sad::layouts::Cell::verticalAlignment));

    {
        std::function<void(sad::layouts::Cell*, const sad::layouts::HorizontalAlignment)> lambda = [](sad::layouts::Cell* c, const sad::layouts::HorizontalAlignment v) {
            c->setHorizontalAlignment(v);
        };
        c->addMethod("setHorizontalAlignment", sad::dukpp03::bind_method::from(&sad::layouts::Cell::setHorizontalAlignment));
        c->addMethod("setHorizontalAlignment", sad::dukpp03::bind_lambda::from(lambda));
    }
    c->addMethod("horizontalAlignment", sad::dukpp03::bind_method::from(&sad::layouts::Cell::horizontalAlignment));


    {
        std::function<void(sad::layouts::Cell*, const sad::layouts::StackingType)> lambda = [](sad::layouts::Cell* c, const sad::layouts::StackingType v) {
            c->setStackingType(v);
        };
        c->addMethod("setStackingType", sad::dukpp03::bind_method::from(&sad::layouts::Cell::setStackingType));
        c->addMethod("setStackingType", sad::dukpp03::bind_lambda::from(lambda));
    }
    c->addMethod("stackingType", sad::dukpp03::bind_method::from(&sad::layouts::Cell::stackingType));

    {
        std::function<void(sad::layouts::Cell*, double)> lambda = [](sad::layouts::Cell* c, double v) {
            c->setPaddingTop(v);
        };
        c->addMethod("setPaddingTop", sad::dukpp03::bind_method::from(&sad::layouts::Cell::setPaddingTop));
        c->addMethod("setPaddingTop", sad::dukpp03::bind_lambda::from(lambda));
    }
    c->addMethod("paddingTop", sad::dukpp03::bind_method::from(&sad::layouts::Cell::paddingTop));

    {
        std::function<void(sad::layouts::Cell*, double)> lambda = [](sad::layouts::Cell* c, double v) {
            c->setPaddingBottom(v);
        };
        c->addMethod("setPaddingBottom", sad::dukpp03::bind_method::from(&sad::layouts::Cell::setPaddingBottom));
        c->addMethod("setPaddingBottom", sad::dukpp03::bind_lambda::from(lambda));
    }
    c->addMethod("paddingBottom", sad::dukpp03::bind_method::from(&sad::layouts::Cell::paddingBottom));

    {
        std::function<void(sad::layouts::Cell*, double)> lambda = [](sad::layouts::Cell* c, double v) {
            c->setPaddingLeft(v);
        };
        c->addMethod("setPaddingLeft", sad::dukpp03::bind_method::from(&sad::layouts::Cell::setPaddingLeft));
        c->addMethod("setPaddingLeft", sad::dukpp03::bind_lambda::from(lambda));
    }
    c->addMethod("paddingLeft", sad::dukpp03::bind_method::from(&sad::layouts::Cell::paddingLeft));

    {
        std::function<void(sad::layouts::Cell*, double)> lambda = [](sad::layouts::Cell* c, double v) {
            c->setPaddingRight(v);
        };
        c->addMethod("setPaddingRight", sad::dukpp03::bind_method::from(&sad::layouts::Cell::setPaddingRight));
        c->addMethod("setPaddingRight", sad::dukpp03::bind_lambda::from(lambda));
    }
    c->addMethod("paddingRight", sad::dukpp03::bind_method::from(&sad::layouts::Cell::paddingRight));

    {
        std::function<void(sad::layouts::Cell*, const sad::Vector<sad::SceneNode*>&, bool)> lambda1 = [](sad::layouts::Cell* c, const sad::Vector<sad::SceneNode*>& v, bool b) {
            c->setChildren(v, b);
        };
        std::function<void(sad::layouts::Cell*, const sad::Vector<sad::SceneNode*>&)> lambda2 = [](sad::layouts::Cell* c, const sad::Vector<sad::SceneNode*>& v) {
            c->setChildren(v);
        };
        std::function<void(sad::layouts::Cell*, const sad::Vector<unsigned long long>&, bool)> lambda3 = [](sad::layouts::Cell* c, const sad::Vector<unsigned long long>& v, bool b) {
            c->setChildren(v, b);
        };
        std::function<void(sad::layouts::Cell*, const sad::Vector<unsigned long long>&)> lambda4 = [](sad::layouts::Cell* c, const sad::Vector<unsigned long long>& v) {
            c->setChildren(v);
        };


        c->addMethod("setChildren", sad::dukpp03::bind_lambda::from(lambda1));
        c->addMethod("setChildren", sad::dukpp03::bind_lambda::from(lambda2));
        c->addMethod("setChildren", sad::dukpp03::bind_lambda::from(lambda3));
        c->addMethod("setChildren", sad::dukpp03::bind_lambda::from(lambda4));
    }
    c->addMethod("children", sad::dukpp03::bind_method::from(&sad::layouts::Cell::children));
    c->addMethod("childrenMajorIds", sad::dukpp03::bind_method::from(&sad::layouts::Cell::childrenMajorIds));

    {
        std::function<void(sad::layouts::Cell*, sad::SceneNode*, bool)> lambda1 = [](sad::layouts::Cell* c, sad::SceneNode* v, bool b) {
            c->addChild(v, b);
        };
        std::function<void(sad::layouts::Cell*, sad::SceneNode*)> lambda2 = [](sad::layouts::Cell* c, sad::SceneNode* v) {
            c->addChild(v);
        };
        std::function<void(sad::layouts::Cell*, unsigned long long, bool)> lambda3 = [](sad::layouts::Cell* c, unsigned long long v, bool b) {
            c->addChild(v, b);
        };
        std::function<void(sad::layouts::Cell*, unsigned long long)> lambda4 = [](sad::layouts::Cell* c, unsigned long long v) {
            c->addChild(v);
        };


        c->addMethod("addChild", sad::dukpp03::bind_lambda::from(lambda1));
        c->addMethod("addChild", sad::dukpp03::bind_lambda::from(lambda2));
        c->addMethod("addChild", sad::dukpp03::bind_lambda::from(lambda3));
        c->addMethod("addChild", sad::dukpp03::bind_lambda::from(lambda4));
    }

    {
        std::function<void(sad::layouts::Cell*, unsigned int, sad::SceneNode*, bool)> lambda1 = [](sad::layouts::Cell* c, unsigned int pos, sad::SceneNode* v, bool b) {
            c->insertChild(pos, v, b);
        };
        std::function<void(sad::layouts::Cell*, unsigned int, sad::SceneNode*)> lambda2 = [](sad::layouts::Cell* c, unsigned int pos, sad::SceneNode* v) {
            c->insertChild(pos, v);
        };
        std::function<void(sad::layouts::Cell*, unsigned int, unsigned long long, bool)> lambda3 = [](sad::layouts::Cell* c, unsigned int pos, unsigned long long v, bool b) {
            c->insertChild(pos, v, b);
        };
        std::function<void(sad::layouts::Cell*, unsigned int, unsigned long long)> lambda4 = [](sad::layouts::Cell* c, unsigned int pos, unsigned long long v) {
            c->insertChild(pos, v);
        };


        c->addMethod("insertChild", sad::dukpp03::bind_lambda::from(lambda1));
        c->addMethod("insertChild", sad::dukpp03::bind_lambda::from(lambda2));
        c->addMethod("insertChild", sad::dukpp03::bind_lambda::from(lambda3));
        c->addMethod("insertChild", sad::dukpp03::bind_lambda::from(lambda4));
    }

    {
        std::function<void(sad::layouts::Cell*, unsigned int)> lambda = [](sad::layouts::Cell* c, unsigned int v) {
            c->removeChild(v);
        };
        c->addMethod("removeChild", sad::dukpp03::bind_method::from(&sad::layouts::Cell::removeChild));
        c->addMethod("removeChild", sad::dukpp03::bind_lambda::from(lambda));
    }

    {
        std::function<void(sad::layouts::Cell*)> lambda = [](sad::layouts::Cell* c) {
            c->clear();
        };
        c->addMethod("clear", sad::dukpp03::bind_method::from(&sad::layouts::Cell::clear));
        c->addMethod("clear", sad::dukpp03::bind_lambda::from(lambda));
    }

    {
        std::function<void(sad::layouts::Cell*, unsigned int, unsigned int)> lambda = [](sad::layouts::Cell* c, unsigned int v1, unsigned int v2) {
            c->swapChildren(v1, v2);
        };
        c->addMethod("swapChildren", sad::dukpp03::bind_method::from(&sad::layouts::Cell::swapChildren));
        c->addMethod("swapChildren", sad::dukpp03::bind_lambda::from(lambda));
    }

    c->addMethod("childrenCount", sad::dukpp03::bind_method::from(&sad::layouts::Cell::childrenCount));
    c->addMethod("child", sad::dukpp03::bind_method::from(&sad::layouts::Cell::child));
    c->addMethod("childMajorId", sad::dukpp03::bind_method::from(&sad::layouts::Cell::childMajorId));
    
    sad::Maybe<size_t>(sad::layouts::Cell::*find1)(sad::SceneNode*) const = &sad::layouts::Cell::find;
    c->addMethod("find", sad::dukpp03::bind_method::from(find1));

    sad::Maybe<size_t>(sad::layouts::Cell::*find2)(unsigned long long) const = &sad::layouts::Cell::find;
    c->addMethod("find", sad::dukpp03::bind_method::from(find2));

    c->addMethod("database", sad::dukpp03::bind_method::from(&sad::layouts::Cell::database));
    c->addMethod("setDatabase", sad::dukpp03::bind_method::from(&sad::layouts::Cell::setDatabase));
    c->addMethod("moveBy", sad::dukpp03::bind_method::from(&sad::layouts::Cell::moveBy));

    c->addMethod("preferredSize", sad::dukpp03::bind_method::from(&sad::layouts::Cell::preferredSize));
    c->addMethod("computedSize", sad::dukpp03::bind_method::from(&sad::layouts::Cell::computedSize));
    c->addMethod("maximalSize", sad::dukpp03::bind_method::from(&sad::layouts::Cell::maximalSize));

    c->addMethod("grid", sad::dukpp03::bind_method::from(&sad::layouts::Cell::grid));


    {
        std::function<sad::Rect2D(sad::layouts::Cell*)> f1 = acc<sad::Rect2D>::get<&sad::layouts::Cell::AssignedArea>;
        std::function<void(sad::layouts::Cell*, sad::Rect2D)> f2 = acc<sad::Rect2D>::set<&sad::layouts::Cell::AssignedArea>;
        c->addAccessor("AssignedArea", sad::dukpp03::bind_lambda::from(f1), sad::dukpp03::bind_lambda::from(f2));
    }
    {
        std::function<bool(sad::layouts::Cell*)> f1 = acc<bool>::get<&sad::layouts::Cell::Rendered>;
        std::function<void(sad::layouts::Cell*, bool)> f2 = acc<bool>::set<&sad::layouts::Cell::Rendered>;
        c->addAccessor("Rendered", sad::dukpp03::bind_lambda::from(f1), sad::dukpp03::bind_lambda::from(f2));
    }
    {
        std::function<unsigned int(sad::layouts::Cell*)> f1 = acc<unsigned int>::get<&sad::layouts::Cell::Row>;
        std::function<void(sad::layouts::Cell*, unsigned int)> f2 = acc<unsigned int>::set<&sad::layouts::Cell::Row>;
        c->addAccessor("Row", sad::dukpp03::bind_lambda::from(f1), sad::dukpp03::bind_lambda::from(f2));
    }
    {
        std::function<unsigned int(sad::layouts::Cell*)> f1 = acc<unsigned int>::get<&sad::layouts::Cell::Col>;
        std::function<void(sad::layouts::Cell*, unsigned int)> f2 = acc<unsigned int>::set<&sad::layouts::Cell::Col>;
        c->addAccessor("Col", sad::dukpp03::bind_lambda::from(f1), sad::dukpp03::bind_lambda::from(f2));
    }
    
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
    
    sad::Maybe<sad::layouts::Grid::SearchResult> (sad::layouts::Grid::*find1)(sad::SceneNode* node) const = &sad::layouts::Grid::find;
    sad::Maybe<sad::layouts::Grid::SearchResult> (sad::layouts::Grid::*find2)(unsigned long long major_id) const = &sad::layouts::Grid::find;

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

