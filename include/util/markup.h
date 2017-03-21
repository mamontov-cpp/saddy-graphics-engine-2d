/*! \file markup.h
     
     Describes a markup, which should be used in label for parsing XML-like markup
 */
#pragma once
#include "../sadvector.h"
#include "../sadhash.h"
#include "../sadstring.h"
#include "../sadcolor.h"

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
    /*! A size in pixel, that should be used to render font
     */
    unsigned int Size;
    /*! A color, which shouldbe used to draw text
     */
    sad::AColor Color;
    /*! A font, that should be used to render text in command
     */
    sad::String Font;
    /*! A linespacing, that should be used for rendering
     */
    double Linespacing;
    /*! Describes, whether line spacing should be taken from font or just left here
     */
    bool LinespacingIsSet;
    /*! A content of string
     */
    sad::String Content;

    /*! Constructs new comand
     */
    inline Command() : Strikethrough(false), Underlined(false), Linespacing(0), LinespacingIsSet(false)
    {

    }
};

/*! A document is a list of tags
 */
typedef sad::Vector<sad::util::Markup::Command> Document;

/*! Parses document for markup. Makes a non-hierarchical list of tags, with respect to nesting.
    \param[in] s string a string
    \param[in] basic a basic command for parent
    \return document
 */
sad::util::Markup::Document parseDocument(
    const sad::String& s,
    const sad::util::Markup::Command& basic
);


/*! Parses tag for markup. Makes a non-hierarchical list of tags, with respect to nesting.
    \param[in] source a source tag
    \param[in] parent a parent command for base
    \return document
 */
sad::util::Markup::Document parseTag(
    const pugi::xml_node& source,
    const sad::util::Markup::Command& parent
);

/*! Parses bool value
    \param[in] value a value
    \param[in] parent a parent value
 */
bool parseBoolValue(const char* value, bool parent);

};

}

}
