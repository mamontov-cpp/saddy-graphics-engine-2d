#include "db/dbconversiontable.h"

#include "sadstring.h"
#include "sadpoint.h"
#include "sadrect.h"
#include "sadcolor.h"
#include "sadsize.h"
#include "object.h"
#include "sadcolor.h"
#include "sadrect.h"

#include "font.h"
#include "texturemappedfont.h"
#include "scenenode.h"
#include "label.h"
#include "formattedlabel.h"
#include "sprite2d.h"
#include "sprite3d.h"

#include <string>
#include "opticksupport.h"

sad::db::AbstractTypeConverter::~AbstractTypeConverter()
{
    PROFILER_EVENT;
    
}

sad::db::ConversionTable::ConversionTable()
{
    PROFILER_EVENT;
    declareImplicit<char, signed char>();
    declareImplicit<signed char, char>();

    declareImplicit<short, unsigned short>();
    declareImplicit<short, unsigned int>();
    declareImplicit<short, unsigned long>();
    declareImplicit<short, unsigned long long>();
    declareImplicit<short, int>();
    declareImplicit<short, long>();
    declareImplicit<short, long long>();

    declareImplicit<int, unsigned int>();
    declareImplicit<int, unsigned long>();
    declareImplicit<int, unsigned long long>();
    declareImplicit<int, long>();
    declareImplicit<int, long long>();
    
    declareImplicit<long, unsigned long>();
    declareImplicit<long, unsigned long long>();
    declareImplicit<long, long long>();

    declareImplicit<long long, unsigned long long>();

    declareImplicit<float, double>();

    declareImplicit<float, unsigned short>();
    declareImplicit<float, unsigned int>();
    declareImplicit<float, unsigned long>();
    declareImplicit<float, unsigned long long>();
    declareImplicit<float, short>();
    declareImplicit<float, int>();
    declareImplicit<float, long>();
    declareImplicit<float, long long>();

    declareImplicit<double, float>();

    declareImplicit<double, unsigned short>();
    declareImplicit<double, unsigned int>();
    declareImplicit<double, unsigned long>();
    declareImplicit<double, unsigned long long>();
    declareImplicit<double, short>();
    declareImplicit<double, int>();
    declareImplicit<double, long>();
    declareImplicit<double, long long>();

    declareImplicit<unsigned short, float>();
    declareImplicit<unsigned int, float>();
    declareImplicit<unsigned long, float>();
    declareImplicit<unsigned long long, float>();
    declareImplicit<short, float>();
    declareImplicit<int, float>();
    declareImplicit<long, float>();
    declareImplicit<long long, float>();

    declareImplicit<unsigned short, double>();
    declareImplicit<unsigned int, double>();
    declareImplicit<unsigned long, double>();
    declareImplicit<unsigned long long, double>();
    declareImplicit<short, double>();
    declareImplicit<int, double>();
    declareImplicit<long, double>();
    declareImplicit<long long, double>();
    
    add("sad::String", "std::string", new  sad::db::ConverterViaConstructor<sad::String, std::string>());
    add("std::string", "sad::String", new  sad::db::ConverterViaConstructor<std::string, sad::String>());

    declareIsSadObjectFlag<char>();
    declareIsSadObjectFlag<signed char>();
    declareIsSadObjectFlag<int>();
    declareIsSadObjectFlag<short>();
    declareIsSadObjectFlag<long>();
    declareIsSadObjectFlag<long long>();

    declareIsSadObjectFlag<unsigned char>();
    declareIsSadObjectFlag<unsigned int>();
    declareIsSadObjectFlag<unsigned short>();
    declareIsSadObjectFlag<unsigned long>();
    declareIsSadObjectFlag<unsigned long long>();

    declareIsSadObjectFlag<float>();
    declareIsSadObjectFlag<double>();
    declareIsSadObjectFlag<std::string>();
    declareIsSadObjectFlag<sad::String>();
    declareIsSadObjectFlag<sad::Color>();
    declareIsSadObjectFlag<sad::AColor>();
    declareIsSadObjectFlag<sad::Rect2D>();
    declareIsSadObjectFlag<sad::Rect2I>();
    declareIsSadObjectFlag<sad::Size2D>();
    declareIsSadObjectFlag<sad::Size2I>();

    declareIsSadObjectFlag<sad::Point2D>();
    declareIsSadObjectFlag<sad::Point2I>();
    declareIsSadObjectFlag<sad::Point3D>();
    declareIsSadObjectFlag<sad::Point3I>();

    declareIsSadObjectFlag<sad::Object>();
    declareIsSadObjectFlag<sad::Font>();
    declareIsSadObjectFlag<sad::TextureMappedFont>();
    declareIsSadObjectFlag<sad::SceneNode>();
    declareIsSadObjectFlag<sad::Label>();
    declareIsSadObjectFlag<sad::FormattedLabel>();
    declareIsSadObjectFlag<sad::Sprite2D>();
    declareIsSadObjectFlag<sad::Sprite3D>();
}

sad::db::ConversionTable::~ConversionTable()
{
    PROFILER_EVENT;
    sad::Hash<sad::String, sad::Hash<sad::String, sad::db::AbstractTypeConverter*> >::iterator it = m_converters.begin();
    for(; it != m_converters.end(); ++it)
    {
        sad::Hash<sad::String, sad::db::AbstractTypeConverter*> & value = it.value();
        for(sad::Hash<sad::String, sad::db::AbstractTypeConverter*>::iterator kit = value.begin();
            kit != value.end();
            ++kit)
        {
            delete kit.value();
        }
    }
}

void sad::db::ConversionTable::add(
    const sad::String & from, 
    const sad::String & to, 
    sad::db::AbstractTypeConverter * c
)
{
    PROFILER_EVENT;
    m_converters_lock.lock();
    sad::Hash<sad::String, sad::db::AbstractTypeConverter*> * h = nullptr;
    if (m_converters.contains(from) == false)
    {
        m_converters.insert(from, sad::Hash<sad::String, sad::db::AbstractTypeConverter*>());
    }
    h = &(m_converters[from]);
    if (h->contains(to))
    {
        delete (*h)[to];
    }
    h->insert(to, c);
    m_converters_lock.unlock();
}

sad::db::AbstractTypeConverter *   sad::db::ConversionTable::converter(
        const sad::String & from, 
        const sad::String & to
)
{
    PROFILER_EVENT;
    m_converters_lock.lock();
    sad::db::AbstractTypeConverter * result = nullptr;
    if (m_converters.contains(from))
    {
        sad::Hash<sad::String, sad::db::AbstractTypeConverter*> & h = m_converters[from];
        if (h.contains(to))
        {
            result = h[to];
        }
    }
    m_converters_lock.unlock();
    return result;
}

static sad::Mutex sad_db_conversion_table_lock;

sad::db::ConversionTable * sad::db::ConversionTable::ref()
{
    PROFILER_EVENT;
    if (sad::db::ConversionTable::m_instance == nullptr)
    {
        sad_db_conversion_table_lock.lock();
        if (sad::db::ConversionTable::m_instance == nullptr)
        {
            sad::db::ConversionTable::m_instance = new sad::db::ConversionTable();
            atexit(sad::db::ConversionTable::freeInstance);
        }
        sad_db_conversion_table_lock.unlock();
    }
    return sad::db::ConversionTable::m_instance;
}


void sad::db::ConversionTable::freeInstance()
{
    PROFILER_EVENT;
    delete sad::db::ConversionTable::m_instance;
}


sad::db::ConversionTable* sad::db::ConversionTable::m_instance = nullptr;
