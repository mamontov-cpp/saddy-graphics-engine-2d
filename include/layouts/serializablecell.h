/*! \file serializablecell.h
    
    Describes a serializable proxy object for cell, to simplify serialization of cell
 */
#pragma once
#include "lengthvalue.h"
#include "verticalalignment.h"
#include "horizontalalignment.h"
#include "stackingtype.h"
#include "../sadvector.h"

#include "../db/save.h"
#include "../db/load.h"

namespace sad
{
    
namespace layouts
{

/*! A serializable proxy object for cell
 */
struct SerializableCell
{
    /*! Width of a cell
     */
    sad::layouts::LengthValue Width;
    /*! Height of a cell
     */
    sad::layouts::LengthValue Height;
    /*! How many rows does the cell span
     */
    unsigned int RowSpan;
    /*! How many rows does the column span
     */
    unsigned int ColSpan;
    /*! Vertical alignment for cell
     */
    sad::layouts::VerticalAlignment VAlign;
    /*! Horizontal alignment for cell
     */
    sad::layouts::HorizontalAlignment HAlign;
    /*! A list of children
     */
    sad::Vector<unsigned long long> Children;
     /*! A stacking type for a cell
     */
    sad::layouts::StackingType StackingType;
    /*! A top padding 
     */
    double PaddingTop;
    /*! A bottom padding 
     */
    double PaddingBottom;
    /*! A left padding 
     */
    double PaddingLeft;
    /*! A right padding 
     */
    double PaddingRight;
    /*! Assigned area to ensure that restoring will not fail locations
     */
    sad::Rect2D AssignedArea;
    /*! A row position
     */
    unsigned int Row;
    /*! A column position 
     */
    unsigned int Col;

    /*! Constructs default serializable cell
     */
    SerializableCell();
};


}

}

namespace sad
{

namespace db
{
/*! Specification for saving serializable cell values
 */
template<>
class Save<sad::layouts::SerializableCell>
{
public:
/*! Saves a value of specified type
    \param[in] ptr a value to be saved
 */
static picojson::value perform(void * ptr)
{
    if (!ptr)
        throw sad::db::InvalidPointer();
    sad::layouts::SerializableCell* cell = reinterpret_cast<sad::layouts::SerializableCell*>(ptr);

    picojson::value v(picojson::object_type, false);
    v.insert("width", sad::db::Save<sad::layouts::LengthValue>::perform(&(cell->Width)));
    v.insert("height", sad::db::Save<sad::layouts::LengthValue>::perform(&(cell->Height)));
    v.insert("row_span", sad::db::Save<unsigned int>::perform(&(cell->RowSpan)));
    v.insert("col_span", sad::db::Save<unsigned int>::perform(&(cell->ColSpan)));
    v.insert("valign", sad::db::Save<unsigned int>::perform(&(cell->VAlign)));
    v.insert("halign", sad::db::Save<unsigned int>::perform(&(cell->HAlign)));
    v.insert("children", sad::db::Save<sad::Vector<unsigned long long> >::perform(&(cell->Children)));
    v.insert("stacking_type", sad::db::Save<unsigned int >::perform(&(cell->StackingType)));
    v.insert("padding_top", sad::db::Save<double>::perform(&(cell->PaddingTop)));
    v.insert("padding_left", sad::db::Save<double>::perform(&(cell->PaddingLeft)));
    v.insert("padding_right", sad::db::Save<double>::perform(&(cell->PaddingRight)));
    v.insert("padding_bottom", sad::db::Save<double>::perform(&(cell->PaddingBottom)));
    v.insert("assigned_area", sad::db::Save<sad::Rect2D>::perform(&(cell->AssignedArea)));
    v.insert("row", sad::db::Save<unsigned int>::perform(&(cell->Row)));
    v.insert("col", sad::db::Save<unsigned int>::perform(&(cell->Col)));
    return v;
}

};

}

}


namespace picojson
{

/*! Tries to converts specific value to SerializableCell
 */
template<>   
class ValueToType<sad::layouts::SerializableCell>
{
public:
    /*! Tries to convert a picojson::value to pair
        \param[in] v value
        \return a result (with value if any)
     */
    static sad::Maybe<sad::layouts::SerializableCell> get(const picojson::value & v)
    {
        sad::Maybe<sad::layouts::SerializableCell> result;
        picojson::value const * wo = picojson::get_property(v, "width");
        picojson::value const * ho = picojson::get_property(v, "height");
        picojson::value const * rso = picojson::get_property(v, "row_span");		
        picojson::value const * cso = picojson::get_property(v, "col_span");	
        picojson::value const * valigno = picojson::get_property(v, "valign");	
        picojson::value const * haligno = picojson::get_property(v, "halign");	
        picojson::value const * childreno = picojson::get_property(v, "children");	
        picojson::value const * stackingtypeo = picojson::get_property(v, "stacking_type");	
        picojson::value const * pto = picojson::get_property(v, "padding_top");
        picojson::value const * pbo = picojson::get_property(v, "padding_bottom");
        picojson::value const * plo = picojson::get_property(v, "padding_left");
        picojson::value const * pro = picojson::get_property(v, "padding_right");
        picojson::value const * aao = picojson::get_property(v, "assigned_area");
        picojson::value const * rowo = picojson::get_property(v, "row");
        picojson::value const * colo = picojson::get_property(v, "col");
        if (wo && ho && rso && cso && valigno && haligno && childreno && stackingtypeo && pto && pbo && plo && pro && aao && rowo && colo)
        {
            sad::Maybe<sad::layouts::LengthValue> maybeWidth = picojson::ValueToType<sad::layouts::LengthValue>::get(*wo);
            sad::Maybe<sad::layouts::LengthValue> maybeHeight = picojson::ValueToType<sad::layouts::LengthValue>::get(*ho);
            sad::Maybe<unsigned int> maybeRowSpan = picojson::ValueToType<unsigned int>::get(*rso);	
            sad::Maybe<unsigned int> maybeColSpan = picojson::ValueToType<unsigned int>::get(*cso);			
            sad::Maybe<unsigned int> maybeVAlign = picojson::ValueToType<unsigned int>::get(*valigno);	
            sad::Maybe<unsigned int> maybeHAlign = picojson::ValueToType<unsigned int>::get(*haligno);
            sad::Maybe<unsigned int> maybeRowO = picojson::ValueToType<unsigned int>::get(*rowo);
            sad::Maybe<unsigned int> maybeColO = picojson::ValueToType<unsigned int>::get(*colo);
            sad::Maybe<sad::Vector<unsigned long long> > maybeChildren = picojson::ValueToType<sad::Vector<unsigned long long> >::get(*childreno);	
            sad::Maybe<unsigned int> maybeStackingType = picojson::ValueToType<unsigned int>::get(*stackingtypeo);	

            sad::Maybe<double> maybePaddingTop = picojson::ValueToType<double>::get(*pto);	
            sad::Maybe<double> maybePaddingBottom = picojson::ValueToType<double>::get(*pbo);	
            sad::Maybe<double> maybePaddingLeft = picojson::ValueToType<double>::get(*plo);	
            sad::Maybe<double> maybePaddingRight = picojson::ValueToType<double>::get(*pro);	

            sad::Maybe<sad::Rect2D> maybeAssignedArea = picojson::ValueToType<sad::Rect2D>::get(*aao);	
            if (maybeWidth.exists() 
                && maybeHeight.exists() 
                && maybeRowSpan.exists() 
                && maybeColSpan.exists()
                && maybeVAlign.exists()
                && maybeHAlign.exists()
                && maybeChildren.exists()
                && maybeStackingType.exists()
                && maybePaddingTop.exists()
                && maybePaddingBottom.exists()
                && maybePaddingLeft.exists()
                && maybePaddingRight.exists()
                && maybeAssignedArea.exists()
                && maybeRowO.exists()
                && maybeColO.exists())
            {
                result.setValue(sad::layouts::SerializableCell());
                result.mutableValue().Width = maybeWidth.value();
                result.mutableValue().Height = maybeHeight.value();
                result.mutableValue().RowSpan = maybeRowSpan.value();
                if (result.mutableValue().RowSpan < 1)
                {
                    result.mutableValue().RowSpan = 1;
                }
                result.mutableValue().ColSpan = maybeColSpan.value();
                if (result.mutableValue().ColSpan < 1)
                {
                    result.mutableValue().ColSpan = 1;
                }
                if (maybeVAlign.value() > static_cast<unsigned int>(sad::layouts::VerticalAlignment::LVA_Bottom))
                {
                    maybeVAlign.setValue(static_cast<unsigned int>(sad::layouts::VerticalAlignment::LVA_Bottom));
                }
                result.mutableValue().VAlign = static_cast<sad::layouts::VerticalAlignment>(maybeVAlign.value());

                if (maybeHAlign.value() > static_cast<unsigned int>(sad::layouts::HorizontalAlignment::LHA_Right))
                {
                    maybeHAlign.setValue(static_cast<unsigned int>(sad::layouts::HorizontalAlignment::LHA_Right));
                }
                result.mutableValue().HAlign = static_cast<sad::layouts::HorizontalAlignment>(maybeHAlign.value());
                result.mutableValue().Children = maybeChildren.mutableValue();

                if (maybeStackingType.value() > static_cast<unsigned int>(sad::layouts::StackingType::LST_NoStacking))
                {
                    maybeStackingType.setValue(static_cast<unsigned int>(sad::layouts::StackingType::LST_NoStacking));
                }
                result.mutableValue().StackingType = static_cast<sad::layouts::StackingType>(maybeStackingType.value());

                result.mutableValue().PaddingTop = maybePaddingTop.value();
                result.mutableValue().PaddingBottom = maybePaddingBottom.value();
                result.mutableValue().PaddingLeft = maybePaddingLeft.value();
                result.mutableValue().PaddingRight = maybePaddingRight.value();
                
                result.mutableValue().AssignedArea = maybeAssignedArea.value();

                result.mutableValue().Row = maybeRowO.value();
                result.mutableValue().Col = maybeColO.value();
            }
        }
        return result;
    }
};

}

namespace sad
{

namespace db
{

DECLARE_DB_LOAD_FOR_TYPE(sad::layouts::SerializableCell)

}

}

