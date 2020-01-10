/*! \file   label.h
    

    Contains a definition of simple label node, that can be added to scene
*/
#include "scene.h"
#include "font.h"
#include "sadrect.h"
#include "3rdparty/format/format.h"
#include "timer.h"
#include "resource/link.h"
#include "util/markup.h"
#include "util/fontcache.h"
#include "sadmutex.h"
#pragma once

namespace sad
{

namespace os
{
class GLFontGeometries;
}
class Renderer;
class FontShaderFunction;
/*! \class Label
    
    Class, that renders a simple multiline text on scene.
    Note, that width of text is unbounded and it grows on right direction and
    down from set rendering point
*/
class Label: public sad::SceneNode
{
SAD_OBJECT
public:
    /*! The strategy for text overflow, which should be applied,
        when text overflows a line with one word in case that BreakText parameter set to LBT_BREAK_WORD or
        maximal line width is non-zero and BreakText parameter is set to LBT_NORMAL.
     */
    enum OverflowStrategy
    {
        LOS_VISIBLE = 0,   //!< A line should be rendered, even if it has length, greater than maximal line width
        LOS_HIDDEN =  1,   //!< A line should be cut, to maximal line width
        LOS_ELLIPSIS = 2,  //!< An ellipsis position for overflow strategy
    };
    /*! Whether we should break lines in case, that maximal line width is set
     */
    enum BreakText
    {
        LBT_NORMAL = 0,     //!< A text should be rendered as one line, unless line feed is occured,
        LBT_BREAK_WORD = 1  //!< A text should be split into lines in case, if line hash length greater then maximal line width. In case it's zero, behaves like LBT_NORMAL
    };
    /*! Defines where suspension point should be placed in case that overflow strategy is LOS_ELLIPSIS
     */ 
    enum TextEllipsisPosition
    {
        LTEP_BEGIN = 0,   //!< In the beginning of string
        LTEP_MIDDLE  = 1, //!< In the middle of string
        LTEP_END = 2      //!< In the end of string
    };
    /*! A parameters for formatted row
     */
    struct  FormattedRowMetrics
    {
        float Ascender;          //!< An ascender part
        float LineSpacingValue;  //!< A descender part
        float Width;             //!< A real with of row
        
        inline FormattedRowMetrics() : Ascender(0), LineSpacingValue(0), Width(0)
        {

        }
    };
    /*! Creates a default broken sprite at (0,0) and no string
     */
    Label();
    /*! Creates a simple label with font size of 20px and angle of zero.
        Default color is black.
        \param[in] font  a font, which label is being rendered with
        \param[in] point an upper-left point, where text starts
        \param[in] string  a string, which is being rendered
     */
    Label(
        sad::Font *  font,
        const sad::Point2D  & point,
        const sad::String & string
    );
    /*! Creates a simple label with font size of 20px and angle of zero.
        Default color is black.
        \param[in] font  a font, which label is being rendered with
        \param[in] point an upper-left point, where text starts
        \param[in] string  a string, which is being rendered
        \param[in] tree a tree name, where font is stored
     */
    Label(
        const sad::String &  font,
        const sad::Point2D  & point,
        const sad::String & string,
        const sad::String & tree = ""
    );
    /*! Sets links resources to a treename and renderer
        \param[in] r renderer
        \param[in] treename a name for tree
     */
    virtual void setTreeName(sad::Renderer* r, const sad::String& treename);
    /*! Fills vector of regions with region of label
        \param[out] r a vector of regions
     */
    virtual void regions(sad::Vector<sad::Rect2D> & r);
    /*! A basic schema for object
        \return a schema 
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
    /*! Renders a string of text inside of label
     */
    virtual void render();
    /*! Called, when renderer for scene is changed
     */
    virtual void rendererChanged();
    /*! Sets non-rotated renderable area
        \param[in] r non-rotated renderable area
     */
    void setArea(const sad::Rect2D & r);
    /*! Returns non-rotated renderable area for a label
        \return non-rotated renderable area
     */
    sad::Rect2D area() const;
    /*! Returns a bounding region for a label
        \return bounding region part
     */
    virtual sad::Rect2D region() const; 
    /*! Returns a font for label
        \return font, which is label is being rendered with
     */
    inline sad::Font * font() const
    {
        return m_font.get();
    }
    /*! Sets a font, that label is being rendered with          
        \param[in] font a font
     */
    void setFont(sad::Font * font);
    /*! Sets new font by trying to get it from a rendered
        \param[in] name a name for a font
        \param[in] r a renderer
        \param[in] tree a tree, where font should be located
     */
    void setFont(const sad::String & name, sad::Renderer * r = NULL, const sad::String & tree = "");
    /*!  Returns a string for a label
            \return a string
     */
    inline const sad::String & string() const
    {
        return m_string;
    }
    /*! Sets a string for a label
        \param[in] string a rendered string in a label
     */
    void setString(const sad::String & string);
    /*!  Returns an upper-left point, where label is placed
            \return a string
     */
    inline const sad::Point2D & point() const
    {
        return m_point;
    }
    /*! Set fonts name for a label
        \param[in] name a name of font for a label
     */
    void setFontName(const sad::String & name);
    /*! Returns name for a font, used in label
        \return name for a used font
     */
    inline const sad::String & fontName() const
    {
        return m_font.path();
    }
    /*! Sets upper-left point for a label
        \param[in] point a point
     */
    void setPoint(const sad::Point2D & point);  
    /*! Sets upper-left point for a label
        \param[in] x x coordinate
        \param[in] y y coordinate
     */ 
    inline void setPoint(double x, double y)
    {
        setPoint(sad::Point2D(x, y));
    }
    /*! Returns true
        \return if node can be rotated
     */
    virtual bool canBeRotated() const;
    /*! Rotates a sprite around his middle point counter-clockwise
        \param[in]  angle an angle, which defines how sprite should be rotated
     */
    virtual void rotate(double angle);
    /*! Returns counter-clockwise rotation angle for a label
        \return angle
     */
    inline double angle() const
    {
        return m_angle;
    }
    /*! Sets angle for rotation of label
        \param[in] angle a rotation angle
     */
    void setAngle(double angle);
    /*! Returns a size for a font in pixels
        \return size of label
     */
    inline unsigned int size() const
    {
        return m_size;
    }
    /*! Returns a size for a label font in points
        \return a size for a label in points
     */
    inline unsigned int pointSize() const
    {
        return static_cast<unsigned int>(m_size / 1.333333);
    }
    /*! Sets a size for a font in pixels
        \param[in] size a size of label in pixels
     */
    void setSize(unsigned int size);
    /*!  Sets size for a font, used in label in pixels
            \param[in] size size of lalbe font in pixels
     */
    inline void setPointSize(unsigned int size)
    {
        setSize(static_cast<unsigned int>(size * 1.3333333));
    }
    /*! Returns a built-in line spacing for a font
        \return built-in line spacing
     */
    float builtinLineSpacing() const;
    /*! Returns a line spacing in pixels
        \return line spacings
     */
    inline float lineSpacing() const
    {
        return m_linespacing_ratio * builtinLineSpacing();
    }
    /*! Sets new line spacing for a label
        \param[in] spacing a line spacing
     */
    void setLineSpacing(float spacing);
    /*! Returns a line spacing ratio
        \return a line spacing ratio
     */
    inline float lineSpacingRatio() const
    {
        return m_linespacing_ratio;
    }
    /*! Sets a line spacing ratio
        \param[in] ratio a line spacing ratio
     */
    void setLineSpacingRatio(float ratio);
    /*! Returns a color for a label
        \return color
     */
    inline const sad::AColor & color() const
    {
        return m_color;
    }
    /*! Sets a color for a label
        \param[in] color a color for a label
     */
    inline void setColor(const sad::AColor & color)
    {
        m_color = color;
    }
    /*! Sets a color for a label
        \param[in] r red component
        \param[in] g green component
        \param[in] b blue component
        \param[in] a alpha component
     */
    inline void setColor(sad::uchar r, sad::uchar g, sad::uchar b, sad::uchar a)
    {
        setColor(sad::AColor(r,g,b,a));
    }
    /*! Destructs it
     */
    virtual ~Label();
    /*! When set scene and font name is defined label tries to reload
        itself from scene's renderer
        \param[in] scene a scene, which will render a node
     */
    virtual void setScene(sad::Scene * scene);
    /*! Sets treename for a sprite
        \param[in] treename a name for a tree
     */
    void setTreeName(const sad::String & treename);
    /*! Sets maximal line width for rendering a label in characters.
        If a current line is wider than limit, than it'll be splitted in multiple lines.
        0 must be passed, for label to have no constrain for rendering a label.
        \param[in] width a width for label
     */
    void setMaximalLineWidth(unsigned int width);
    /*! Returns maximal line width for label in characters.
        See details in sad::Label::setMaximalLineWidth .
        \return maximal line width for label
     */
    unsigned int maximalLineWidth() const;
    /*! Sets the strategy for text overflow, which should be applied,
        when text overflows a line with one word in case that BreakText parameter set to LBT_BREAK_WORD or
        maximal line width is non-zero and BreakText parameter is set to LBT_NORMAL.
        \param[in] s a strategy value
     */
    void setOverflowStrategy(sad::Label::OverflowStrategy s);
    /*! Sets the strategy for text overflow. Clamps a value to enumeration bounds if needed.
        \param[in] s strategy
     */
    void setOverflowStrategyFromIndex(unsigned int s);
    /*! Returns overflow strategy for label
        \return overflow strategy
     */
    sad::Label::OverflowStrategy overflowStrategy() const;
    /*! Returns overflow strategy for label as index 
        \return oveflow strategy
     */
    unsigned int overflowStrategyAsIndex() const;
    /*! Sets, whether we should break lines in case, that maximal line width is set
        \param[in] value value
     */
    void setBreakText(sad::Label::BreakText value);
    /*! Sets, whether we should break lines in case, that maximal line width is set. Clamps a value to enumeration bounds if needed.
        \param[in] value 
     */
    void setBreakTextFromIndex(unsigned int value);
    /*! Returns whether we should break text in lines
        \return break text value
     */
    sad::Label::BreakText breakText() const;
    /*! Returns whether we should break text as index of enum
        \return whether we should break text
     */
    unsigned int breakTextAsIndex() const;
    /*! Sets text ellipsis position in text
        \param[in] value value
     */
    void setTextEllipsisPosition(sad::Label::TextEllipsisPosition value);
    /*! Sets text ellipsisis position in text. Clamps a value to enumeration bounds if needed.
        \param[in] value 
     */
    void setTextEllipsisPositionAsIndex(unsigned int value);
    /*! Returns text ellipsis position
        \return text ellipsis position
     */
    sad::Label::TextEllipsisPosition textEllipsis() const;
    /*! Returns text ellipsis position as index
        \return text ellipsis position as index
     */
    unsigned int textEllipsisAsIndex() const;
    /*! Sets maximal lines count, that should be contained in label
        \param[in] line_count maximal lines count
     */
    void setMaximalLinesCount(unsigned int line_count);
    /*! Returns maximal lines count, that should be contained in label (0 - to not constrain)
        \return maximal lines
     */
    unsigned int maximalLinesCount() const;
    /*! Sets the strategy for text overflow, which should be applied,
        when text overflows amount of lines. 
        \param[in] s a strategy value
     */
    void setOverflowStrategyForLines(sad::Label::OverflowStrategy s);
    /*! Sets the strategy for text overflow for lines. Clamps a value to enumeration bounds if needed.
        \param[in] s strategy
     */
    void setOverflowStrategyForLinesFromIndex(unsigned int s);
    /*! Returns overflow strategy for label for lines in label
        \return overflow strategy
     */
    sad::Label::OverflowStrategy overflowStrategyForLines() const;
    /*! Returns overflow strategy for label as index  ffor lines in label
        \return oveflow strategy
     */
    unsigned int overflowStrategyForLinesAsIndex() const;
    /*! Sets text ellipsis position in text for lines
        \param[in] value value
     */
    void setTextEllipsisPositionForLines(sad::Label::TextEllipsisPosition value);
    /*! Sets text ellipsisis position in text for lines. Clamps a value to enumeration bounds if needed.
        \param[in] value 
     */
    void setTextEllipsisPositionForLinesAsIndex(unsigned int value);
    /*! Returns text ellipsis position for lines
        \return text ellipsis position
     */
    sad::Label::TextEllipsisPosition textEllipsisForLines() const;
    /*! Returns text ellipsis position as index
        \return text ellipsis position as index
     */
    unsigned int textEllipsisForLinesAsIndex() const;
    /*! Returns whether text label can have formatting
        \return whether text label can have formatting
     */
    bool hasFormatting() const;
    /*! Turns label into formatted (or not)
        \param[in] value a new value for label
     */
    void setHasFormatting(bool value);
    /*! Makes label formatted
     */
    void makeFormatted();
    /*! Makes label non-formatted
     */
    void disableFormatting();
    /*! Places label in the center between two points, with angle rotated to p1
        \param[in] p1 first point
        \param[in] p2 second point
     */
    void makeSpanBetweenPoints(
        const sad::Point2D & p1, 
        const sad::Point2D & p2
    );
    /*! Creates string, that could be rendered with label, 
        using specified settings. Used to create rendering string
        for label
        \param[in] string a string, that must be rendered
        \param[in] maximal_line_width a maximal width of line in characters
        \param[in] s a strategy, which should be applied (\@see sad::Label::OverflowStrategy)
        \param[in] bt whether we should break text into lines (\@see sad::Label::BreakText)
        \param[in] tep a suspension point position
        \param[in] maximum_lines a maximal lines count, that should be shown
        \param[in] overflow_for_lines how should function behave in case that amount of lines exceeds maximum_lines
        \param[in] text_ellipsis_for_lines where should ellipsis be placed in case that amount of lines exceeds maximum_lines and overflow_for_lines is set to sad::Label::LOS_ELLIPSIS
        \return string, how it would be rendered by label with specified settings
     */
    static sad::String makeRenderingString(
        const sad::String& string,   
        unsigned int maximal_line_width,
        sad::Label::OverflowStrategy s,
        sad::Label::BreakText bt,
        sad::Label::TextEllipsisPosition tep,
        unsigned int maximum_lines = 0,
        sad::Label::OverflowStrategy overflow_for_lines = sad::Label::LOS_ELLIPSIS,
        sad::Label::TextEllipsisPosition text_ellipsis_for_lines = sad::Label::LTEP_BEGIN
    );

    /*! Creates string, that could be rendered with label,
        using specified settings. Used to create rendering string
        for label
        \param[in] string a string, that must be rendered
        \param[in] maximal_line_width a maximal width of line in characters
        \param[in] s a strategy, which should be applied (\@see sad::Label::OverflowStrategy)
        \param[in] bt whether we should break text into lines (\@see sad::Label::BreakText)
        \param[in] tep a suspension point position
        \param[in] maximum_lines a maximal lines count, that should be shown
        \param[in] overflow_for_lines how should function behave in case that amount of lines exceeds maximum_lines
        \param[in] text_ellipsis_for_lines where should ellipsis be placed in case that amount of lines exceeds maximum_lines and overflow_for_lines is set to sad::Label::LOS_ELLIPSIS
        \return string, how it would be rendered by label with specified settings
    */
    static sad::util::Markup::Document makeRenderingString(
        const sad::util::Markup::Document& string,
        unsigned int maximal_line_width,
        sad::Label::OverflowStrategy s,
        sad::Label::BreakText bt,
        sad::Label::TextEllipsisPosition tep,
        unsigned int maximum_lines = 0,
        sad::Label::OverflowStrategy overflow_for_lines = sad::Label::LOS_ELLIPSIS,
        sad::Label::TextEllipsisPosition text_ellipsis_for_lines = sad::Label::LTEP_BEGIN
    );
    /*! Computes length for command
        \param[in] c a command for markup
        \return total length for string
     */
    static size_t length(const sad::Vector<sad::util::Markup::Command>& c);
    /*! Breaks a command into words. Each word has a space at the end if it need to
        \param[in] v a vector
        \return list of commands
     */
    static sad::Vector<sad::util::Markup::Command> breakIntoWords(const sad::Vector<sad::util::Markup::Command>& v);
    /*! Formats text line, constraining it by maximal line width, using specified overflow strategy
        and text ellipsis position
        \param[in] string a string  value
        \param[in] maximal_line_width maximal line width
        \param[in] s overflow strategy
        \param[in] tep text ellipsis position
     */
    static sad::String formatTextLine(
        const sad::String& string,
        unsigned int maximal_line_width,
        sad::Label::OverflowStrategy s,
        sad::Label::TextEllipsisPosition tep
    );
    /*! Formats text line, constraining it by maximal line width, using specified overflow strategy
        and text ellipsis position
        \param[in] string a string  value
        \param[in] maximal_line_width maximal line width
        \param[in] s overflow strategy
        \param[in] tep text ellipsis position
     */
    static sad::Vector<sad::util::Markup::Command> formatTextLine(
        const sad::Vector<sad::util::Markup::Command>& string,
        unsigned int maximal_line_width,
        sad::Label::OverflowStrategy s,
        sad::Label::TextEllipsisPosition tep
    );    
    /*! Returns first command for document
        \param[in] doc document
        \return first command for document or empty command if not found
     */
    static sad::util::Markup::Command firstCommand(const sad::util::Markup::Document& doc);
    /*! Returns last command for document
        \param[in] doc document
        \return last command for document or empty command if not found
     */
    static sad::util::Markup::Command lastCommand(const sad::util::Markup::Document& doc);
    /*! Tests if document row consists of whitespace character
        \param[in] row a row
        \return whether it consists from whitespace characters
     */
    static bool consistsOfWhitespaceCharacters(const sad::Vector<sad::util::Markup::Command>& row);
    /*! Trims a row of commands
        \param[in, out] row a row to edit
        \param[in] left whether we should trim left part
        \param[in] right whether we should trim right part
     */
    static void trim(sad::Vector<sad::util::Markup::Command>& row, bool left = true, bool right = true);
    /*! Returns substring of a row
        \param[in] row a row
        \param[in] begin a begin index
        \param[in] length a length for index
     */
    static sad::Vector<sad::util::Markup::Command> subString(const sad::Vector<sad::util::Markup::Command>& row, int begin, int length);
    /*! Moves object by specified vector
        \param[in] p point
     */
    virtual void moveBy(const sad::Point2D& p);
    /*! Returns rendered string length
        \return length of rendered string
     */
    unsigned int renderedStringLength() const;
    /*! Sets limit on rendering string, without actually changing it's size or other parameters.
        Change string if you want them to be changed
        \param[in] limit a limit on rendering string
     */
    void setRenderingStringLimit(unsigned int limit);
    /*! Clears limit on rendering string
     */
    void clearRenderingStringLimit();
    /*! Sets a rendering string limit as double. Limit should be in [0..1]
        \param[in] limit  a limit value
     */
    void setRenderingStringLimitAsRatioToLength(double limit);
    /*! Called, when label is removed from scene
     */
    virtual void onRemovedFromScene();
    /*! Sets shader function
     *  \param[in] fun a function
     */
    virtual void setShaderFunction(sad::ShaderFunction* fun);
    /*! Sets line shader function
     *  \param[in] fun line shader function
     */
    virtual void setLineShaderFunction(sad::FontShaderFunction* fun);
    /*! Returns line shader function
     *  \return line shader function
     */
    sad::FontShaderFunction* lineShaderFunction() const;
private:
    /*! Reloads font for a label from scene
     */
    void reloadFont();
    /*! Recomputes rotation coefficients, 
        so rotation will be placed just in place 
        \param[in] lock whether we should lock when recomputing
     */
    void recomputeRenderingPoint(bool lock = true);
    /*! Returns size of label  without formatting
        \param[in] font a basic font
        \return size
     */
    sad::Size2D getSizeWithoutFormatting(sad::Font* font);
    /*! Returns size of label  with formatting
        \param[in] font a basic font
    */
    sad::Size2D getSizeWithFormatting(sad::Font* font);
    /*! Recomputes rendered string, so it will be preserved on every change
        \param[in] lock whether we should lock, when recomputing
     */
    void recomputeRenderedString(bool lock = true);
    /*! Recomputes rendering string without respect to formatting
     */
    void recomputeRenderingStringWithoutFormatting();
    /*! Recomputes rendering string
     */
    void recomputeRenderingStringWithFormatting();
    /*! Clears font cache for label
    */
    void clearFontsCache();
    /*! Applies font command to font
        \param[in] font a font
        \param[in] c command
        \param[in] font with applied command data
        \return font with flags
     */
    sad::Pair<sad::Font*, sad::Font::RenderFlags> applyFontCommand(sad::Font* font, const sad::util::Markup::Command& c);
    /*! Tries to get font for document. If not found, returns default font
        \param[in] s string
     */
    sad::Font* getFontForDocument(const sad::String& s);
    /*! Renders text with formatting
        \param[in] font a font
    */
    void renderWithFormatting(sad::Font* font);
    /*! Renders text without formatting
        \param[in] font a font
     */
    void renderWithoutFormatting(sad::Font* font);
    /*! A link to font, that label is being renderd with
     */
    sad::resource::Link<sad::Font> m_font;
    /*! A stored string, used in label
     */
    sad::String      m_string; 
    /*!  A string, which will be rendered, when label is placed in scene
     */
    sad::String      m_rendered_string;
    /*! A maximal line width for rendering a label in characters.
        If a current line is wider than limit, than it'll be splitted in multiple lines.
        0 must be passed, for label to have no constrain for rendering a label.
     */
    unsigned int  m_maximal_line_width;
    /*! An upper left point, where text is rendered
     */
    sad::Point2D     m_point;  
    /*! A counter-clockwise rotation angle for a label
     */
    double            m_angle;
    /*! A size of font in label in pixels
     */
    unsigned int     m_size;
    /*! A line spacing ratio, that is being set on font
     */
    float            m_linespacing_ratio;
    /*! A color for label
     */
    sad::AColor      m_color;
    /*! A non-rotated center of label
     */
    sad::Point2D     m_center;
    /*! A half size with negative parts, needed to render a font
     */
    sad::Point2D     m_halfpadding;
    /*! A cached region for label
     */
    sad::Rect2D      m_cached_region;
    /*! A text overflow strategy for text
     */ 
    sad::Label::OverflowStrategy m_overflow_strategy;
    /*!  Whether we should break text if it exceeds line width
     */
    sad::Label::BreakText m_break_text;
    /*! A suspension point position
     */
    sad::Label::TextEllipsisPosition m_text_ellipsis_position;
    /*! A maximum lines in label, that should be displayed 
     */
    unsigned int m_maximum_lines;
    /*! A rendered characters length
     */
    unsigned int m_rendered_chars;
    /*! A strategy, that should be used, when amount of lines is larger than needed
     */
    sad::Label::OverflowStrategy m_overflow_strategy_for_lines;
    /*! Where should be ellipsis placed in case, when amount of lines are larger than needed
        and m_overflow_strategy_for_lines is set to LOS_ELLIPSIS .
     */
    sad::Label::TextEllipsisPosition m_text_ellipsis_position_for_lines;
    /*! Whether label is formatted
     */
    bool m_formatted;
    /*! Whether we computed rendering string
     */
    bool m_computed_rendering_string;
    /*! Whether we computed rendering point
     */
    bool m_computed_rendering_point;
    /*! A document to be set
     */
    sad::util::Markup::Document m_document;
    /*! A row metrics for a document
     */
    sad::Vector<sad::Label::FormattedRowMetrics> m_document_metrics;
    /*! A font cache for getting fonts
     */
    sad::util::FontCache m_font_cache;
    /*! A mutex for recomputing string label
     */
    sad::Mutex m_recompute_string_lock;
    /*! A mutex for recomputing point label
     */
    sad::Mutex m_recompute_point_lock;
    /*! A limit for rendering character
     */
    sad::Maybe<unsigned int> m_rendering_character_limit;
    /*! A geometries data
     */
    sad::os::GLFontGeometries* m_geometries;
    /*! Whether geometries are dirty
     */
    bool m_geometries_dirty;
    /*! A shader function for lines
     */
    sad::FontShaderFunction* m_line_shader_function;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::Label)
