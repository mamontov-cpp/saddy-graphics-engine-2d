#include "dukpp-03/context.h"
#include "dukpp-03/renderer.h"

#include <geometry2d.h>
#include <fuzzyequal.h>
#include <renderer.h>

#include <layouts/lengthvalue.h>
#include <layouts/serializablecell.h>

#include <cassert>

#define PERFORM_AND_ASSERT(X)   {bool b = ctx->eval(X); assert(b); }

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
}

