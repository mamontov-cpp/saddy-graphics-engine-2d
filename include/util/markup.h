/*! \file markup.h
     
     Describes a markup, which should be used in label for parsing XML-like markup
 */
#pragma once
#include "../sadvector.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "../sadhash.h"
#include "../sadstring.h"
#include "../sadcolor.h"
#include "../maybe.h"

namespace pugi
{
class xml_node;

}

namespace sad
{

namespace util
{

/*!  A main markup structure 
 */	
struct Markup
{

/*! Font size type for markup
 */
enum FontSizeType
{
    MFZST_POINTS = 0, //!< Size specified in points
    MFZST_PIXELS = 1  //!< Size specified in pixels
};

/*! A linespacing size type for markup
 */
enum LineSpacingSizeType
{
    MLST_PERCENTS = 0, //!< Size specified in percents
    MLST_PIXELS = 1,   //!< Size specified in pixels
};

/*! A font size, used in markup
 */
struct FontSize
{
    /*! A real size
     */
    unsigned int Size;
    /*! A type of font size
     */
    sad::util::Markup::FontSizeType Type;

    /*! A default constructor
     */
    inline FontSize() : Size(0), Type(sad::util::Markup::FontSizeType::MFZST_PIXELS)
    {

    }

    /*! Constructs new size
        \param[in] sz size
        \param[in] type a type
     */
    inline FontSize(unsigned int sz, sad::util::Markup::FontSizeType type) : Size(sz), Type(type)
    {

    }

    /*! A comparator for sizes
        \param[in] sz size
        \return whether those are equal
     */
    inline bool operator==(const sad::util::Markup::FontSize & sz) const
    {
        return Size == sz.Size && Type == sz.Type;
    }
};


/*! A line-spacing size, used in markup
 */
struct LineSpacingSize
{
    /*! A real size
     */
    unsigned int Size;
    /*! A type of line-spacing size
     */
    sad::util::Markup::LineSpacingSizeType Type;

    /*! A default constructor
     */
    inline LineSpacingSize() : Size(0), Type(sad::util::Markup::MLST_PIXELS)
    {

    }

    /*! Constructs new size
        \param[in] sz size
        \param[in] type a type
     */
    inline LineSpacingSize(unsigned int sz, sad::util::Markup::LineSpacingSizeType type) : Size(sz), Type(type)
    {

    }

    /*! A comparator for sizes
        \param[in] sz size
        \return whether those are equal
     */
    inline bool operator==(const sad::util::Markup::LineSpacingSize & sz) const
    {
        return Size == sz.Size && Type == sz.Type;
    }
};

/*! A tag element, that is being parsed
 */
struct Command
{
    /*! Whether text should be strikethrough
     */
    bool Strikethrough;
    /*! Whether text should be underlined
     */
    bool Underlined;
    /*! Whether text font should emulate bold
     */
    bool Bold;
    /*! Whether text font should emulate italic font
    */
    bool Italic;
    /*! A size of font, used to render font
     */
    sad::Maybe<sad::util::Markup::FontSize> Size;
    /*! A color, which shouldbe used to draw text
     */
    sad::Maybe<AColor> Color;
    /*! A font, that should be used to render text in command
     */
    sad::Maybe<sad::String> Font;
    /*! A linespacing, that should be used for rendering
     */
    sad::Maybe<sad::util::Markup::LineSpacingSize> Linespacing;
    /*! A content of string
     */
    sad::String Content;
    /*! A width parameter for command (filled by rendered object)
     */
    float Width;
    /*! An ascender for markup command (filled by rendered object)
     */
    float Ascender;
    /*! A computed linespacing markup command (filled by rendered object)
     */
    float LineSpacingValue;

    /*! Constructs new comand
     */
    inline Command() : Strikethrough(false), Underlined(false), Bold(false), Italic(false), Width(0), Ascender(0), LineSpacingValue(0)
    {

    }
};

/*! A document-line is a list of tags
 */
typedef sad::Vector<sad::util::Markup::Command> DocumentLine;

/*! A document is a list of tags organized line-by-line
 */
typedef sad::Vector<sad::util::Markup::DocumentLine> Document;

/*! Parses document for markup. Makes a non-hierarchical list of tags, with respect to nesting.
    \param[in] s string a string
    \param[in] basic a basic command for parent
    \return document
 */
static sad::util::Markup::Document parseDocument(
    const sad::String& s,
    const sad::util::Markup::Command& basic
);


/*! Parses tag for markup. Makes a non-hierarchical list of tags, with respect to nesting.
    \param[in] source a source tag
    \param[in] parent a parent command for base
    \return document
 */
static sad::util::Markup::DocumentLine parseTag(
    const pugi::xml_node& source,
    const sad::util::Markup::Command& parent
);

/*! Parses bool value
    \param[in] value a value
    \param[in] parent a parent value
 */
static bool parseBoolValue(const char* value, bool parent);

/*! Parses font size
    \param[in] s string
    \param[in] parentSize a size in parent tag
 */
static sad::Maybe<sad::util::Markup::FontSize> parseSize(const sad::String& s, const sad::Maybe<sad::util::Markup::FontSize>& parentSize);

/*! Parses font line-spacing size
    \param[in] s string
    \param[in] parentLineSpacing a line-spacing in parent tag
 */
static sad::Maybe<sad::util::Markup::LineSpacingSize> parseLineSpacingSize(const sad::String& s, const sad::Maybe<sad::util::Markup::LineSpacingSize>& parentLineSpacing);

/*! Parses font 
    \param[in] s string
    \param[in] parentFont a font in parent tag
*/
static sad::Maybe<sad::String> parseFont(const sad::String& s, const sad::Maybe<sad::String>& parentFont);

/*! Tries to get color from table
    \param[in] s color name
    \return parsed color
 */
static sad::Maybe<sad::AColor> getColorFromTable(const char* s);

/*! Parses hex. String, specified by pointer MUST HAVE valid characters and be two characters
    \param[in] s string
    \return byte char
 */
static unsigned char parseHexChar(const char* s);

/*! Parses byte. String, specified by pointer MUST HAVE valid characters and be two characters
    \param[in] s string
    \return byte char
 */
static unsigned char parseByte(const char* s);

/*! Parses color in hexadecimal RGBA format. Color format string must be  "#RRGGBBAA" or "#RRGGBB"
    \param[in] s string
    \return a color or nothing if failed to parse
 */
static sad::Maybe<sad::AColor> parseHexRGBA(const char *s);

/*! Parses color value. Color format string must be  "#RRGGBBAA" or "#RRGGBB" or color name
    \param[in] s string
    \param[in] parentColor a parent color value
    \return result
 */
static sad::Maybe<sad::AColor> parseColor(const char* s, sad::Maybe<sad::AColor> parentColor);

/*! Tries to push next line if is_div specified
    \param[in] is_div flag
    \param[in,out] result document line
 */
static void tryPushNextLine(bool is_div, sad::util::Markup::DocumentLine& result);

};

}

}
