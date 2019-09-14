/*! \file customobject.h
    

    Describes custom object with custom properties. Could be Sprite2D or Labels, depending on stuff
 */
#pragma once
#include "../../scenenode.h"
#include "../../resource/link.h"
#include "../../resource/tree.h"
#include "customschema.h"
#include "label.h"


namespace sad
{
class Sprite2D;

namespace db
{
    
namespace custom
{

/*! Defines an extensible object as scene node, that could 
    be represented as either Sprite2D, or Label 
 */
class Object: public sad::SceneNode
{
SAD_OBJECT
public:
    /*! Creates new empty stub object
     */
    Object();
    /*! Deletes own schema and data
     */
    virtual ~Object();
    /*! Sets scene 
        \param[in] scene a scene, which will render a node
     */
    virtual void setScene(sad::Scene * scene);
    /*! Whether inner object is one of specified types
        \param[in] type a name of type
        \return whether it is of specified type
     */
    bool innerTypeIs(const sad::String& type);
    /*! Sets a tree name for object with specified renderer
        \param[in] r renderer, which tree should be fetched from
        \param[in] tree_name a name for an item for object
     */
    virtual void setTreeName(sad::Renderer* r, const sad::String & tree_name);
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
    /*! Fills vector of regions with data, that could be used for identifying bounds of item.
        As default, no regions are produced.
        \param[out] r a vector of regions
     */
    virtual void regions(sad::Vector<sad::Rect2D> & r);
    /*! Returns fully rotated region for object
        \return region
     */
    virtual sad::Rect2D renderableArea() const;
    /*! Renders a current rendered object
     */ 
    virtual void render();
    /*! Sets new schema name for an object
        \param[in] s a string name
     */
    void setSchemaName(const sad::String & s);
    /*! Returns schema path as path to resource 
        \return schema path to resource
     */
    const sad::String& schemaName() const;
    /*! Returns size of font, used in label
        \return font size
     */
    unsigned int fontSize() const;
    /*! Sets size of font, used in label
        \param[in] size a size of custom object
     */
    void setFontSize(unsigned int);
    /*! Sets font name for custom object
        \return font name
     */
    const sad::String& fontName() const;
    /*! Sets font name for object
        \param[in] name name for font
     */
    void setFontName(const sad::String& name);
    /*! Returns line spacing for, used in label
        \return line spacing for label
     */ 
    float lineSpacingRatio() const;
    /*! Sets line spacing, used in label
        \param[in] s a line spacing
     */
    void setLineSpacingRatio(float s);
    /*! Sets maximal line width, used in label
        \param[in] width a width of object
     */
    void setMaximalLineWidth(unsigned int width);
    /*! Returns true
        \return if node can be rotated
     */
    virtual bool canBeRotated() const;
    /*! Rotates a sprite around his middle point counter-clockwise
        \param[in]  angle an angle, which defines how sprite should be rotated
     */
    virtual void rotate(double angle);
    /*! Returns angle, used in object
        \return angle in object
     */
    double angle() const;
    /*! Sets angle
        \param[in] a object rotation angle
     */
    void setAngle(double a);
    /*! Returns non-rotated object area
        \return non-rotated object area
     */
    sad::Rect2D area() const;
    /*! Sets non-rotated object area
        \param[in] object area
     */
    void setArea(const sad::Rect2D & area);
    /*! Returns a string for label
        \return string data
     */
    const sad::String& string() const;
    /*! Sets a string for a label
        \param[in] s a string data
     */
    void setString(const sad::String & s);
    /*! Returns color for sprite and label
        \return color for sprite and label
     */
    const sad::AColor& color() const;
    /*! Sets color for sprite and label
        \return color
     */
    void setColor(const sad::AColor & c);
    /*! Whether sprite is flipped horizontally
        \return whether sprite is flipped horizontally
     */
    bool flipX() const;
    /*! Whether sprite is flipped horizontally 
        \param[in] whether sprite is flipped horizontally
     */
    void setFlipX(bool f);
    /*! Whether sprite is flipped vertically
        \return whether sprite is flipped horizontally
     */
    bool flipY() const;
    /*! Whether sprite is flipped vertically 
        \param[in] f flag
     */
    void setFlipY(bool f);
    /*! Returns maximal line width, used in label
        \return maximal line width
     */
    unsigned int maximalLineWidth() const;
    /*! Whether custom object could be rendered
        \return whether object could be rendered
     */
    bool canBeRendered() const;
    /*! Returns property list from schema
        \return property list from schema
     */
    const sad::Hash<sad::String, sad::db::Property*>& schemaProperties() const;
    /*! Sets sprite options for custom object
        \param[in] o options
     */
    void setOptions(const sad::String& o);
    /*! Returns sprite options for custom object
        \return sprite options
     */
    const sad::String& options() const;
    /*! Loads object from picojson object
        \param[in] v a picojson object
        \return  whether it as successfull
     */
    virtual bool load(const picojson::value& v);
    /*! Sets a texture coordinates for sprites
        \param[in] texturecoordinates a texture coordinates for a sprite in notation, defined in 
                                      constructor
     */
    void setTextureCoordinates(const sad::Rect2D & texturecoordinates);
    /*! Returns a texture coordinates
        \return a texture coordinates
     */
    const sad::Rect2D & textureCoordinates() const;
    /*! Sets the strategy for text overflow, which should be applied,
        when text overflows a line with one word in case that BreakText parameter set to LBT_BREAK_WORD or
        maximal line width is non-zero and BreakText parameter is set to LBT_NORMAL.
        \param[in] ovstrategy a strategy value
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
        \param[in] ovstrategy a strategy value
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
    /*! Copies state of custom property values from other object. Please, note that this will overwrite
        in  case that this object has properties, which o doesn't, if they have same schema and fail otherwise
        \param[in] o other object
        \return whether it was successfull. In case of non-successfull old values are preserved
     */
    bool copyCustomPropertyValuesFrom(sad::db::custom::Object* o);
    /*! Places label in the center between two points, with angle rotated to p1
        \param[in] p1 first point
        \param[in] p2 second point
     */
    void makeSpanBetweenPoints(
        const sad::Point2D & p1, 
        const sad::Point2D & p2
    );
    /*! Moves custom object by specified number
        \param[in] dist a distance
     */
    virtual void moveBy(const sad::Point2D & dist);
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
    /*! Sets shader function
     *  \param[in] fun a function
     */
    virtual void setShaderFunction(sad::ShaderFunction* fun);
protected:
    /*! Fills custom object with defailt item properties
     */
    void initDefaultSchema();
    /*! Updates configuration of object, basing on schema
        \param[in] s a new schema 
     */
    void updateConfiguration(sad::db::custom::Schema * s);
    /*! A current rendered object for custom object
     */
    sad::SceneNode * m_current_rendered_object;
    /*! A sprite as defined 
     */
    sad::Sprite2D * m_sprite2d;
    /*! A label as defined
     */
    sad::Label * m_label;
    /*! A stored properties schema for object
     */
    sad::db::schema::Schema * m_custom_schema;
    /*! An object's schema
     */
    sad::db::schema::Schema * m_my_schema;
    /*! A schema for an object
        \param[in] schema for object
     */
    sad::resource::Link<sad::db::custom::Schema> m_schema;
};


}

}

}
