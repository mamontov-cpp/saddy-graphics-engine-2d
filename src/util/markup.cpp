#include "util/markup.h"

/*! A state, when working with local text
 */
enum MarkupParserState
{
    INSIDE_TEXT = 0,
    TAG_NAME_SEARCH = 1,
    INSIDE_TAG_NAME = 2,
    INSIDE_TAG_ATTRIBUTE_NAME = 3,
    INSIDE_TAG_ATTRIBUTE_EQUAL = 4,
    INSIDE_TAG_ATTRIBUTE_VALUE = 5
};

sad::util::Markup::Document sad::util::Markup::parseDocument(const sad::String& s)
{
    sad::util::Markup::Document result;
    result << sad::util::Markup::Tag();
    int parent_offset = 0;
    int tag_start = 0;
    sad::String buffer_name;
    MarkupParserState state = INSIDE_TEXT;
    for (size_t i = 0; i < s.size(); i++)
    {
        bool handled = false;
        switch (state)
        {
        case INSIDE_TEXT: {
                if (s[i] != '<')
                {
                    result[parent_offset].Content << s[i];
                }
                else
                {
                    tag_start = i;
                    state = TAG_NAME_SEARCH;
                }
                break;
            }
        case TAG_NAME_SEARCH: {
                if (s[i] != '>')
                {
                    if (s[i] != ' ' && s[i] != '\t')
                    {
                        buffer_name.clear();
                        buffer_name << s[i];
                        state = INSIDE_TAG_NAME;
                    }
                }
                else
                {
                    result[parent_offset].Content.substr(tag_start, i - tag_start + 1);
                    state = INSIDE_TEXT;
                }
                break;
            }
        case INSIDE_TAG_NAME: {
                if (s[i] == '>')
                {

                }
            }
        } 		
    }
    // End of string

    return result;
}