#include "util/markup.h"
#include "3rdparty/pugixml/pugixml.hpp"

#include <cstring>

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

