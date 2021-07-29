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
    pugi::xml_document xml_document;
    sad::String string = s;
    string.replaceAllOccurrences("\r\n", "\n");
    string.replaceAllOccurrences("\r", "\n");
    xml_document.load_string(string.c_str(), pugi::parse_fragment);
    sad::util::Markup::DocumentLine doc =  sad::util::Markup::parseTag(xml_document, basic);
    
    // Split document by lines
    sad::util::Markup::DocumentLine tmp;
    for(size_t i = 0; i < doc.size(); i++)
    {
        sad::StringList content = doc[i].Content.split("\n", sad::String::KEEP_EMPTY_PARTS);
        if (doc[i].Content.endsWith("\n", 1))
        {
            content << "";
        }
        if (content.size() != 0)
        {

            if (content.size() == 1)
            {
                tmp << doc[i];
            }
            else
            {
                sad::util::Markup::Command cmd = doc[i];
                for(int i = 0; i < content.size() - 1; i++)
                {
                    cmd.Content = content[i];
                    tmp << cmd;
                    result << tmp;
                    tmp.clear();
                }
                cmd.Content = content[content.size() - 1];
                tmp << cmd;
            }
        }
    }
    if (tmp.size() != 0)
    {
        result << tmp;
    }
    
    // Remove empty commands
    for(size_t i = 0; i < result.size(); i++)
    {
        if (result[i].size() > 1)
        {
            for(size_t j = 0; j < result[i].size(); j++)
            {
                if (result[i][j].Content.length() == 0)
                {
                    result[i].removeAt(j);
                    --j;
                }
            }
        }
    }
    return result;
}

sad::util::Markup::DocumentLine sad::util::Markup::parseTag(
    const pugi::xml_node& source,
    const sad::util::Markup::Command& parent
)
{
    sad::util::Markup::Command my_command = parent;
    for (pugi::xml_attribute attr = source.first_attribute(); attr; attr = attr.next_attribute())
    {
        if (!strcmp(attr.name(), "underline"))
        {
            my_command.Underlined = sad::util::Markup::parseBoolValue(attr.value(), parent.Underlined);
        }
        if (!strcmp(attr.name(), "strikethrough"))
        {
            my_command.Strikethrough = sad::util::Markup::parseBoolValue(attr.value(), parent.Strikethrough);
        }
        if (!strcmp(attr.name(), "bold"))
        {
            my_command.Bold = sad::util::Markup::parseBoolValue(attr.value(), parent.Bold);
        }
        if (!strcmp(attr.name(), "italic"))
        {
            my_command.Italic = sad::util::Markup::parseBoolValue(attr.value(), parent.Italic);
        }
        if (!strcmp(attr.name(), "font"))
        {
            my_command.Font = sad::util::Markup::parseFont(attr.value(), parent.Font);
        }
        if (!strcmp(attr.name(), "size"))
        {
            my_command.Size = sad::util::Markup::parseSize(attr.value(), parent.Size);
        }
        if (!strcmp(attr.name(), "linespacing"))
        {
            my_command.Linespacing = sad::util::Markup::parseLineSpacingSize(attr.value(), parent.Linespacing);
        }
        if (!strcmp(attr.name(), "color"))
        {
            my_command.Color = sad::util::Markup::parseColor(attr.value(), parent.Color);
        }
    }
    sad::util::Markup::DocumentLine result;
    for (pugi::xml_node child = source.first_child(); child; child = child.next_sibling())
    {
        if (child.type() == pugi::node_pcdata)
        {
            sad::util::Markup::Command cmd = my_command;
            cmd.Content = child.text().get();
            result << cmd;
        }
        if (child.type() == pugi::node_element)
        {
            bool is_div = false;
            if (!strcmp(child.name(), "div"))
            {
                is_div = true;
            }
            sad::util::Markup::tryPushNextLine(is_div, result);
            result << sad::util::Markup::parseTag(child, my_command);
            sad::util::Markup::tryPushNextLine(is_div, result);
        }
    }
    return result;
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
    sad::util::Markup::FontSizeType type = sad::util::Markup::FontSizeType::MFZST_PIXELS;
    sad::String data = s;
    if (s.size() > 2)
    {
        sad::String result = s.subString(s.length() - 2, 2);
        result.toLower();
        if (result == "pt" || result == "px")
        {
            data = s.subString(0, s.length() - 2);
            type = (result == "pt") ? sad::util::Markup::FontSizeType::MFZST_POINTS : sad::util::Markup::FontSizeType::MFZST_PIXELS;
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
    if (s.size() >= 2)
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

void sad::util::Markup::tryPushNextLine(bool is_div, sad::util::Markup::DocumentLine& result)
{
    if (is_div)
    {
        if (result.size() != 0)
        {
            if (result[result.size() -1].Content.endsWith("\n", 1) == false)
            {
                result[result.size() -1].Content += "\n";
            }
        }
    }
}
