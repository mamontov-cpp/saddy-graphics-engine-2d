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
        if (wo && ho && rso && cso && valigno && haligno && childreno && stackingtypeo)
        {
            sad::Maybe<sad::layouts::LengthValue> maybeWidth = picojson::ValueToType<sad::layouts::LengthValue>::get(*wo);
            sad::Maybe<sad::layouts::LengthValue> maybeHeight = picojson::ValueToType<sad::layouts::LengthValue>::get(*ho);
            sad::Maybe<unsigned int> maybeRowSpan = picojson::ValueToType<unsigned int>::get(*rso);	
            sad::Maybe<unsigned int> maybeColSpan = picojson::ValueToType<unsigned int>::get(*cso);			
            sad::Maybe<unsigned int> maybeVAlign = picojson::ValueToType<unsigned int>::get(*valigno);	
            sad::Maybe<unsigned int> maybeHAlign = picojson::ValueToType<unsigned int>::get(*haligno);	
            sad::Maybe<sad::Vector<unsigned long long> > maybeChildren = picojson::ValueToType<sad::Vector<unsigned long long> >::get(*childreno);	
            sad::Maybe<unsigned int> maybeStackingType = picojson::ValueToType<unsigned int>::get(*stackingtypeo);	
            if (maybeWidth.exists() 
                && maybeHeight.exists() 
                && maybeRowSpan.exists() 
                && maybeColSpan.exists()
                && maybeVAlign.exists()
                && maybeHAlign.exists()
                && maybeChildren.exists()
                && maybeStackingType.exists())
            {
                result.setValue(sad::layouts::SerializableCell());
                result.mutableValue().Width = maybeWidth.value();
                result.mutableValue().Height = maybeHeight.value();
                result.mutableValue().RowSpan = maybeRowSpan.value();
                if (result.mutableValue().RowSpan < 1)
                {
                    result.mutableValue().RowSpan = 0;
                }
                result.mutableValue().ColSpan = maybeColSpan.value();
                if (result.mutableValue().ColSpan < 1)
                {
                    result.mutableValue().ColSpan = 0;
                }
                if (maybeVAlign.value() > static_cast<unsigned int>(sad::layouts::LVA_Right))
                {
                    maybeVAlign.setValue(static_cast<unsigned int>(sad::layouts::LVA_Right));
                }
                result.mutableValue().VAlign = static_cast<sad::layouts::VerticalAlignment>(maybeVAlign.value());

                if (maybeHAlign.value() > static_cast<unsigned int>(sad::layouts::LHA_Right))
                {
                    maybeHAlign.setValue(static_cast<unsigned int>(sad::layouts::LHA_Right));
                }
                result.mutableValue().HAlign = static_cast<sad::layouts::HorizontalAlignment>(maybeHAlign.value());
                result.mutableValue().Children = maybeChildren.mutableValue();

                if (maybeStackingType.value() > static_cast<unsigned int>(sad::layouts::LST_Vertical))
                {
                    maybeStackingType.setValue(static_cast<unsigned int>(sad::layouts::LST_Vertical));
                }
                result.mutableValue().StackingType = static_cast<sad::layouts::StackingType>(maybeStackingType.value());
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

