/*! \file sprite2d.h
    

    Contains a definition of sprite in 2-dimensional space.
 */
#include "resource/resource.h"
#include "resource/link.h"
#include "scene.h"
#include "maybe.h"
#include "sadrect.h"
#include "sadstring.h"
#include "sadsize.h"
#include "texture.h"
#pragma once

namespace sad
{

/*! \class Sprite2D

    A simple colored sprite in 2-dimensional space, that can be rotated, moved and
    resized. A sprite is represented as a quad with both texture blending and mixing
    with static color (no color by default).
 */
class Sprite2D: public sad::SceneNode
{
SAD_OBJECT
public:
    /*! A simple options to make 2d sprite from template. Could be stored in some places
     */
    class Options: public sad::resource::Resource
    {
      SAD_OBJECT
      public:
          
          /*! A texture name, that defines a texture in sprite
           */
          sad::String Texture;       
          /*! Stored texture coordinates in pixelds
           */
          sad::Rect2D TextureRectangle;  
          /*! A rectangle, which defines current quad to be rendered
           */
          sad::Rect2D Rectangle;
          /*! Whether some color is transparent in sprite. Applied only in case
              of loading resources
           */
          bool Transparent;
          /*! A transparent color data
           */
          sad::Color TransparentColor;

          /*! Creates default invalid options
           */
          inline Options() : Transparent(false)
          {
              
          }

          /*! Loads an options from specified file, using specified renderer for building mip maps.
              \param[in] file a file, via which a resource should be loaded
              \param[in] r  a renderer, which resource should be linked to (NULL if global renderer)
              \param[in] options  an options for loading a resource
              \return whether loading was successfull
           */
           virtual bool load(
                const sad::resource::ResourceFile & file,
                sad::Renderer * r,
                const picojson::value& options
           );
           /*! Load an options from value
               \param[in] v an options to be loaded
               \return whether loading was successfull
            */
           bool load(const picojson::value& v);
    };
    /*! Creates default invalid sprite, which must be initialized via setter methods
     */
    Sprite2D();
    /*! Creates a new sprite from supplied parameters
        \param[in] texture a texture, supplied for sprite (NULL to make sprite not viewable)
        \param[in] texturecoordinates a texture coordinates from top left to bottom right
                                      in pixels
        \param[in] area     a rectangle, where sprite should be rendered
        \param[in] fast     whether we should not init angle rotations for area and just treat it as base	 
     */
    Sprite2D(
        sad::Texture* texture,
        const sad::Rect2D& texturecoordinates,
        const sad::Rect2D& area,
        bool fast = true
    );
    /*! Creates a new sprite from supplied parameters
        \param[in] texture a full texture name (must not be empty)
        \param[in] texturecoordinates a texture coordinates from top left to bottom right
                                      in pixels
        \param[in] area     a rectangle, where sprite should be rendered
        \param[in] fast     whether we should not init angle rotations for area and just treat it as base
        \param[in] treename a name of tree, where we should take our texture resource
     */
    Sprite2D(
        const sad::String& texture,
        const sad::Rect2D& texturecoordinates,
        const sad::Rect2D& area,
        bool fast = true,
        const sad::String & treename = ""
    );
    /*! You can inherit the sprite, using various implementation
        defined behaviour
     */
    virtual ~Sprite2D();
    /*! Fills vector of regions with data, that could be used for identifying bounds of item.
        As default, no regions are produced.
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
    /*! Renders a sprite as a simple quad 
     */
    virtual void render();
    /*! Called, when renderer for scene is changed
     */
    virtual void rendererChanged();
    /*! Sets a texture coordinates for sprites
        \param[in] texturecoordinates a texture coordinates for a sprite in notation, defined in 
                                      constructor
     */
    void setTextureCoordinates(const sad::Rect2D & texturecoordinates);
    /*! Sets a texture coordinate in pixels for specified point
        \param[in] index an index to be set
        \param[in] point a point to be set
     */
    void setTextureCoordinate(int index, const sad::Point2D & point);
    /*! Sets a new texture coordinates on a point
        \param[in] index index
        \param[in] x   x coord
        \param[in] y   y coord.
     */
    void setTextureCoordinate(int index, double x ,double y);
    /*! Returns a texture coordinates
        \return a texture coordinates
     */
    const sad::Rect2D & textureCoordinates() const;
    /*! Sets area for a rectangle to following rectangle
        \param[in] rect a non-rotated rectangle
     */
    void setRenderableArea(const sad::Rect2D & rect);
    /*! Returns a non-rotated area for a sprite
        \return area for sprite
     */
    sad::Rect2D area() const;
    /*! Sets non-rotated area for a sprite
        \param[in] a area
     */
    void setArea(const sad::Rect2D & a);
    /*! Returns rotated renderable area for a sprite
        \return area for sprite
     */
    const sad::Rect2D & renderableArea() const;
    /*! Returns a reference to a sprite point
        \param[in] n index of point in rectangle
        \return reference to point
     */
    const sad::Point2D & point(int n) const;
    /*! Calculates a middle point of a sprite
        \return middle point of sprite
     */
    const sad::Point2D &  middle() const;
    /*! Sets middle point of a sprite
        \param[in] p a middle point to be set
     */
    void setMiddle(const sad::Point2D & p);
    /*! Returns size of a sprite
        \return size of sprite
     */
    const sad::Size2D & size() const;
    /*! Sets size of a sprite
        \param[in] size size of sprite
        \param[in] reg  whether we should register size change in flag, which will be returned
                        in sad::Sprite2D::sizeChanged().
     */
    void setSize(const sad::Size2D & size, bool reg = true);
    /*! Moves a sprite by following vector
        \param[in] dist a distance to be moved
     */
    virtual void moveBy(const sad::Point2D & dist);
    /*! Moves a sprite center to a point
        \param[in] p a new center for a sprite
     */
    void moveTo(const sad::Point2D & p);
    /*! Returns true
        \return if node can be rotated
     */
    virtual bool canBeRotated() const;
    /*! Rotates a sprite around his middle point counter-clockwise
        \param[in]  angle an angle, which defines how sprite should be rotated
     */
    virtual void rotate(double angle);
    /*! Sets sprite counter-clockwise rotation angle
        \param[in] alpha alpha rotation angle
     */
    void setAngle(double alpha);
    /*! Returns counter-clockwise rotation angle
        \return counter-clockwise rotation angle
     */ 
    double angle() const;
    /*! Sets a color for sprite
        \param[in] clr blending sprite color
     */
    void setColor(const sad::AColor & clr);
    /*! Returns a color for sprite
        \return blending color of sprite 
     */
    const sad::AColor & color() const;
    /*! Sets horizontal flip rotation flag
        \param[in] flipx new rotation flag value
     */
    void setFlipX(bool flipx);
    /*! Sets vertical flip rotation flag
        \param[in] flipy new rotation flag value
     */	
    void setFlipY(bool flipy);
    /*! Returns horizontal flipped y flag
        \return flipped x flag
     */
    bool flipX() const;
    /*! Returns vertical flipped y flag
        \return flipped y flag
     */
    bool flipY() const;
    /*! Sets a texture for sprite
        \param[in] texture a new texture
     */
    void setTexture(sad::Texture * texture);
    /*! Returns a texture for a sprite
        \return texture for a sprite
     */
    sad::Texture * texture() const;
    /*! Sets a name of texture, which should be taken from renderer's database
        \param[in] name name of texture
     */
    void setTexureName(const sad::String & name);
    /*! Returns a texture name
        \return texture name
     */
    const sad::String& textureName();
    /*! Initializes sprite parameters from options, passed to sprite
        \param[in] o options, which defines texture, texture coordinates and rendered rectangle
     */
    void set(const sad::Sprite2D::Options & o);
    /*! Initializes sprite parameters from options, passed to sprite
        \param[in] optionsname a name of attached options element
     */
    void set(const sad::String & optionsname);
    /*! Returns options for sprite
        \return options
     */
    sad::Sprite2D::Options* getOptions() const;
    /*! Returns option name
        \return options name
     */
    const sad::String& optionsName() const;
    /*! Sets tree name for options and textures links
        \param[in] treename a name of renderer's tree
     */
    void setTreeName(const sad::String & treename);
    /*! A name of renderer's tree for texture resource
        \return name of tree
     */
    const sad::String & treeName() const;
    /*! Makes sprite rectangle span between two points, which defines centers of opposite sides
        of sprite
        \param[in] p1 first point
        \param[in] p2 second point
     */
    void makeSpanBetweenPoints(
        const sad::Point2D & p1, 
        const sad::Point2D & p2
    );
    /*! Makes sprite rectangle span between two points, which defines centers of opposite sides
        of sprite
        \param[in] r source rectangle for adapter
        \param[in] p1 first point
        \param[in] p2 second point
     */
    void makeSpanBetweenPoints(
        const sad::Rect2D & r, 
        const sad::Point2D & p1, 
        const sad::Point2D & p2
    );
    /*! When set scene and texture name is defined 2D sprite tries to reload
        itself from scene's renderer
        \param[in] scene a scene, which will render a node
     */
    virtual void setScene(sad::Scene * scene);
    /*! Sets, whether or not sprite should change size, when options for a sprite are changed
        \param[in] flag whether or  not
     */
    void setChangeSizeWhenOptionsAreChanged(bool flag);
    /*! Returns whether or not sprite, should change size, when options for a sprite are changed
        \return flag
     */
    bool changeSizeWhenOptionsAreChanged() const;
    /*! Enables or disables loading mode for sprite. In loading mode, when sprite changes options,
        it's area will not be reset. Also, it works only once, so you can call it
        before set method to make sure, that area will be preserved
        \param[in] load whether loading mode is enabled
     */
    void toggleLoadingMode(bool load);
    /*! Loads sprite from picojson object
        \param[in] v a picojson object
        \return  whether it as successfull
     */
    virtual bool load(const picojson::value& v);
    /*! Sets a tree name for object with specified renderer
        \param[in] r renderer, which tree should be fetched from
        \param[in] tree_name a name for an item for object
     */
    virtual void setTreeName(sad::Renderer* r, const sad::String & tree_name);
protected:
    /*! Performed, when texture is changed
        \param[in] tex a new texture
     */
    void onTextureChange(sad::Texture * tex);
    /*! Performed, when sprite options are changed
        \param[in] opts a new options
     */
    void onOptionsChange(sad::Sprite2D::Options * opts);
    /*! Fast version of 2D sprite initialization from rectangle. Just sets it as
        current renderable rectangle, all angles to zero
        \param[in] rect a rectangle
     */
    void initFromRectangleFast(const sad::Rect2D & rect);
    /*! Initializes 2D sprite from rectangle
        \param[in] rect a rectangle
     */
    void initFromRectangle(const sad::Rect2D & rect);
    /*! Rebuilds renderable area for a sprite
     */
    void buildRenderableArea();
    /*! Reload sprites from a texture for a data
     */
    void reloadTexture();
    /*! Normalizes texture coordinates, filling a normalized a texture coordinates
     */
    void normalizeTextureCoordinates();
    /*! Normalizes texture coordinates, filling a normalized a texture coordinates
        \param[in] tex a texture, which coordinates must be normalized with
     */
    void normalizeTextureCoordinates(sad::Texture * tex);
    /*! A counter-clockwise rotation angle
     */
    double     m_angle;
    /*! Whether sprite is flipped on X axis
     */
    bool       m_flipx;
    /*! Whether sprite is flipped on Y axis
     */
    bool       m_flipy;
    /*! A normalized texture coordinates, that must be less than 1
        and represent a fraction of texture
     */
    sad::Rect2D m_normalized_texture_coordinates;
    /*! Denormalized texture coordinates, that is definted in pixels
     */
    sad::Rect2D m_texture_coordinates;
    /*! A middle point for rectangle and pivot point
     */
    sad::Point2D  m_middle;
    /*! A sizes for rectangle
     */
    sad::Size2D   m_size;
    /*! A renderable area is an area, that could be rendered, when a 
        rendering is performed.
     */
    sad::Rect2D  m_renderable_area;
    /*! A texture to be used in sprite
     */
    sad::resource::Link<sad::Texture> m_texture;
    /*! A link to an options
     */
    sad::resource::Link<sad::Sprite2D::Options> m_options;
    /*! A used color of sprite
     */
    sad::AColor  m_color;
    /*! A current color buffer, used when getting current color of scene
     */ 
    int  m_current_color_buffer[4];
    /*! Determines, whether we should change own size, if options size is changed
     */
    bool m_changesizeifoptionssizechanged;
    /*! Whether we should explicitly set a sprite value
     */
    bool m_explicit_set;
    /*! Toggles loading time on sprite.
        If sprite is loading, it would not synchronize it's area with 
        options
     */
    bool m_loading;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::Sprite2D)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::Sprite2D::Options)
