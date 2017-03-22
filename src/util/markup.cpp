#include "util/markup.h"
#include "3rdparty/pugixml/pugixml.hpp"

#include <cstring>
#include <sstream>
#include <cstdlib>

sad::util::Markup::Document sad::util::Markup::parseDocument(
    const sad::String& s,
    const sad::util::Markup::Command& basic
)
{
    sad::util::Markup::Document result;
    pugi::xml_document doc;
    doc.load_string(s.c_str(), pugi::parse_fragment);
    for (pugi::xml_node child = doc.first_child(); child; child = child.next_sibling())
    {
        if (child.type() == pugi::node_pcdata)
        {
            sad::util::Markup::Command cmd = basic;
            cmd.Content = child.text().get();
        }
        if (child.type() == pugi::node_element)
        {
            result << sad::util::Markup::parseTag(child, basic);
        }
    }
    return result;
}

sad::util::Markup::Document sad::util::Markup::parseTag(
    const pugi::xml_node& source,
    const sad::util::Markup::Command& parent
)
{
    return sad::util::Markup::Document();
}


bool sad::util::Markup::parseBoolValue(const char* value, bool parent)
{
    if (!value)
    {
        return parent;
    }
    if (!strcmp(value, "true"))
    {
        return true;
    }
    if (!strcmp(value, "false"))
    {
        return false;
    }
    return parent;
}

sad::Maybe<sad::util::Markup::FontSize> sad::util::Markup::parseSize(const sad::String& s, const sad::Maybe<sad::util::Markup::FontSize>& parentSize)
{
    // Perform substring
    sad::util::Markup::FontSizeType type = sad::util::Markup::MFZST_PIXELS;
    sad::String data = s;
    if (s.size() > 2)
    {
        sad::String result = s.subString(s.length() - 2, 2);
        result.toLower();
        if (result == "pt" || result == "px")
        {
            data = s.subString(0, s.length() - 2);
            type = (result == "pt") ? sad::util::Markup::MFZST_POINTS : sad::util::Markup::MFZST_PIXELS;
        }
    }
    // Parse as just a string
    if (data.length() == 0)
    {
        return parentSize;
    }

    for (size_t i = 0; i < data.length(); i++)
    {
        if (data[i] < '0' || data[i] > '9')
        {
            return parentSize;
        }
    }

    std::istringstream stream(data.c_str());
    unsigned int size = 0;
    stream >> size;
    return sad::Maybe<sad::util::Markup::FontSize>(sad::util::Markup::FontSize(size, type));
}


sad::Maybe<sad::String>  sad::util::Markup::parseFont(const sad::String& s, const sad::Maybe<sad::String>& parentFont)
{
    if (s.length() != 0)
    {
        return sad::Maybe<sad::String>(s);
    }
    return parentFont;
}