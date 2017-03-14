#include "sadhash.h"

// ===============================  std::hash ===============================

size_t std::hash<sad::String>::operator()(const sad::String& o) const
{
    std::hash<std::string> h;
    return h(o);
}

size_t std::hash<sad::WString>::operator()(const sad::WString& o) const
{
    std::hash<std::wstring> h;
    return h(o);
}


// ===============================================================================