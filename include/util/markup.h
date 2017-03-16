/*! \file markup.h
     
     Describes a markup, which should be used in label for parsing XML-like markup
 */
#pragma once
#include "../sadvector.h"
#include "../sadhash.h"
#include "../sadstring.h"

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
struct Tag
{
    /*! A name of tag
     */
    sad::String Name;
    /*! A content of string
     */
    sad::String Content;

    /*! A list of attributes for tag
     */
    sad::Hash<sad::String, sad::String> Attributes;

    /*! A parent tag offset
     */
    size_t ParentOffset;

    /*! Constructs default tag
     */
    inline Tag() : ParentOffset(0)
    {

    }
};

/*! A document is a list of tags
 */
typedef sad::Vector<Tag> Document;

/*! Parses document for markup. Makes a non-hierarchical list of tags, with respect to nesting.
    \param[in] s string a string
    \return document
 */
sad::util::Markup::Document parseDocument(const sad::String& s);
};

}

}
