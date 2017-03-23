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
    // Parse as just a string, that contains number
    if (data.isNumeric() == false)
    {
        return parentSize;
    }

    std::istringstream stream(data.c_str());
    unsigned int size = 0;
    stream >> size;
    return sad::Maybe<sad::util::Markup::FontSize>(sad::util::Markup::FontSize(size, type));
}

sad::Maybe<sad::util::Markup::LineSpacingSize> sad::util::Markup::parseLineSpacingSize(const sad::String& s, const sad::Maybe<sad::util::Markup::LineSpacingSize>& parentLineSpacing)
{
    // Perform substring
    sad::util::Markup::LineSpacingSizeType type = sad::util::Markup::MLST_PIXELS;
    sad::String data = s;
    if (s.size() > 2)
    {
        sad::String result = s.subString(s.length() - 2, 2);
        result.toLower();
        if (result == "px")
        {
            data = s.subString(0, s.length() - 2);
            type = sad::util::Markup::MLST_PIXELS;
        }
        if (result[1] == '%')
        {
            data = s.subString(0, s.length() - 1);
            type = sad::util::Markup::MLST_PERCENTS;
        }
    }
    else
    {
        if (s.size() == 1)
        {
            if (s[0] == '%')
            {
                // It's already incorrect, no reason to parse further
                return parentLineSpacing;
            }
        }
    }
    // Try parse line spacing size
    if (data.isNumeric() == false)
    {
        return parentLineSpacing;
    }

    std::istringstream stream(data.c_str());
    unsigned int size = 0;
    stream >> size;
    return sad::Maybe<sad::util::Markup::LineSpacingSize>(sad::util::Markup::LineSpacingSize(size, type));
}


sad::Maybe<sad::String>  sad::util::Markup::parseFont(const sad::String& s, const sad::Maybe<sad::String>& parentFont)
{
    if (s.length() != 0)
    {
        return sad::Maybe<sad::String>(s);
    }
    return parentFont;
}

unsigned char sad::util::Markup::parseHexChar(const char* s)
{
    char c = *s;
    if (c >= 'A' && c <= 'F')
    {
        return (c - 'A') + 10; 
    }
    if (c >= 'a' && c <= 'f')
    {
        return (c - 'a') + 10; 
    }
    if (c >= '0' && c <= '9')
    {
        return (c - '0'); 
    }
    return 0;
}

unsigned char sad::util::Markup::parseByte(const char* s)
{
    return sad::util::Markup::parseHexChar(s) * 16 + sad::util::Markup::parseHexChar(s + 1);
}

sad::Maybe<sad::AColor> sad::util::Markup::parseHexRGBA(const char *s)
{
    sad::Maybe<sad::AColor> result;
    if (!s)
    {
        return result;
    }
    int length = strlen(s);
    // Specification must be #RRGGBB or #RRGGBBAA
    if (length != 7 && length != 9)
    {
        return result;
    }
    // First character must be hex start
    if (*s != '#')
    {
        return result;
    }
    // Check if other characters are supported
    for (int i = 1; i < length; i++)
    {
        char c = s[i];
        bool isValidHexInUpperCase = (c >= 'A' && c <= 'F');
        bool isValidHexInLowerCase = (c >= 'a' && c <= 'f');
        bool isValidNumeric = (c >= '0' && c <= '9');
        if (!(isValidHexInUpperCase || isValidHexInLowerCase || isValidNumeric))
        {
            return result;
        }
    }
    
    unsigned char r = parseByte(s + 1);
    unsigned char g = parseByte(s + 3);
    unsigned char b = parseByte(s + 5);
    unsigned char a = 0;
    if (length == 9)
    {
        a = parseByte(s + 7);
    }
    result.setValue(sad::AColor(r,g,b,a));
    return result;
}

sad::Maybe<sad::AColor>  sad::util::Markup::parseColor(const char* s, sad::Maybe<sad::AColor> parentColor)
{
    if (!s)
    {
        return parentColor;
    }
    sad::Maybe<sad::AColor> result = parseHexRGBA(s);
    if (result.exists() == false)
    {
        result = sad::util::Markup::getColorFromTable(s);
    }
    return result.exists() ? result : parentColor;
}
