/*! \file   sprite3d.h
    

    Contains a definition of sprite in 3-dimensional space.
*/
#pragma once
#include "scene.h"
#include "texture.h"
#include "sadrect.h"
#include "sadstring.h"
#include "sadsize.h"
#include "resource/link.h"

namespace sad
{
 
/*! \class Sprite3D

    A simple colored sprite in 3-dimensional space, that can be rotated, moved and
    resized. A sprite is represented as a quad with both texture blending and mixing
    with static color (no color by default).
 */
class Sprite3D: public sad::SceneNode
{
SAD_OBJECT
public:
    /*! Creates default invalid sprite, which must be initialized via setter methods
     */
    Sprite3D();
    /*! Creates a new sprite from supplied parameters
        \param[in] texture a texture, supplied for sprite (NULL to make sprite not viewable)
        \param[in] texturecoordinates a texture coordinates from top left to bottom right
                                      in pixels
        \param[in] area     a rectangle, where sprite should be rendered
        \param[in] fast     whether we should not init angle rotations for area and just treat it as base	 
     */
    Sprite3D(
        sad::Texture* texture,
        const sad::Rect2D& texturecoordinates,
        const sad::Rect<sad::Point3D>& area,
        bool fast = true
    );
    /*! Creates a new sprite from supplied parameters
        \param[in] texture a full texture name (must not be empty)
        \param[in] texturecoordinates a texture coordinates from top left to bottom right
                                      in pixels
        \param[in] area     a rectangle, where sprite should be rendered
        \param[in] tree     a name of tree, from which texture must be taken
        \param[in] fast     whether we should not init angle rotations for area and just treat it as base
     */
    Sprite3D(
        const sad::String& texture,
        const sad::Rect2D& texturecoordinates,
        const sad::Rect<sad::Point3D>& area,
        const sad::String& tree = "",
        bool fast = true
    );
    /*! You can inherit the sprite, using various implementation
        defined behaviour
     */
    virtual ~Sprite3D();
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
    void setRenderableArea(const sad::Rect<sad::Point3D> & rect);
    /*! Returns a non-rotated area for a sprite
        \return area for sprite
     */
    sad::Rect<sad::Point3D> area() const;
    /*! Returns rotated renderable area for a sprite
        \return area for sprite
     */
    const sad::Rect<sad::Point3D> & renderableArea() const;
    /*! Returns a reference to a sprite point
        \param[in] n index of point in rectangle
        \return reference to point
     */
    const sad::Point3D & point(int n) const;
    /*! Calculates a middle point of a sprite
        \return middle point of sprite
     */
    const sad::Point3D &  middle() const;
    /*! Sets middle point of a sprite
        \param[in] p middle point to be set
     */
    void setMiddle(const sad::Point3D & p);
    /*! Returns size of a sprite
        \return size of sprite
     */
    const sad::Size2D & size() const;
    /*! Sets size of a sprite
        \param[in] size size of sprite
     */
    void setSize(const sad::Size2D & size);
    /*! Moves a sprite by following vector
        \param[in] dist a distance to be moved
     */
    void moveBy(const sad::Point3D & dist);
    /*! Moves a sprite by following vector
        \param[in] p a distance to be moved
     */    
    virtual void moveBy(const sad::Point2D& p);
    /*! Moves a sprite center to a point
        \param[in] p a new center for a sprite
     */
    void moveTo(const sad::Point3D & p);
    /*! Returns true
        \return if node can be rotated
     */
    virtual bool canBeRotated() const;
    /*! Equivalent to rotate(angle, 0)
        \param[in]  angle an angle, which defines how sprite should be rotated
     */
    virtual void rotate(double angle);
    /*! Rotates a sprite around his middle point in three dimensions
        \param[in]  alpha angle of rotation on XY axis
        \param[in]  theta angle of rotation on YZ axis
     */
    void rotate(double alpha, double theta);
    /*! Sets XY axis rotation angle
        \param[in] alpha alpha rotation angle
     */
    void setAlpha(double alpha);
    /*!	Sets YZ axis rotation angle
        \param[in] theta theta rotation angle
     */
    void setTheta(double theta);
    /*! Returns XY axis rotation angle
        \return XY axis rotation angle
     */ 
    double alpha() const;
    /*! Returns YZ axis rotation angle
        \return YZ axis rotation angle
     */ 
    double theta() const;
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
        \param[in] texture a new texture for a sprite
     */
    void setTexture(sad::Texture * texture);
    /*! Returns a texture for a sprite
        \return texture for a sprite
     */
    sad::Texture * texture() const;
    /*! Sets a name of texture, which should be taken from renderer's database
        \param[in] name name of texture
     */
    void setTextureName(const sad::String & name);
    /*! Returns a texture name
        \return texture name
     */
    const sad::String& textureName();
    /*! Sets treename for a sprite
        \param[in] treename a name for a tree
     */
    void setTreeName(const sad::String & treename);
    /*! When set scene and texture name is defined 3D sprite tries to reload
        itself from scene's renderer
        \param[in] scene a scene, which will render a node
     */
    virtual void setScene(sad::Scene * scene);
    /*! Sets a tree name for object with specified renderer
        \param[in] r renderer, which tree should be fetched from
        \param[in] tree_name a name for an item for object
     */
    virtual void setTreeName(sad::Renderer* r, const sad::String & tree_name);
protected:
    /*! Fast version of 3D sprite initialization from rectangle. Just sets it as
        current renderable rectangle, all angles to zero
        \param[in] rect a rectangle
     */
    void initFromRectangleFast(const sad::Rect<sad::Point3D> & rect);
    /*! Initializes 3D sprite from rectangle
        \param[in] rect a rectangle
     */
    void initFromRectangle(const sad::Rect<sad::Point3D> & rect);
    /*! Rebuilds renderable area for a sprite
     */
    void buildRenderableArea();
    /*! Reload sprites from a texture for a data
     */
    void reloadTexture();
    /*! Normalizes texture coordinates, filling a normalized a texture coordinates
     */
    void normalizeTextureCoordinates();
    /*! A rotation angle in  XY plane
     */
    double     m_alpha;
    /*! A rotation angle in  YZ plane
     */
    double     m_theta;
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
    sad::Point3D  m_middle;
    /*! A sizes for rectangle
     */
    sad::Size2D   m_size;
    /*! A renderable area is an area, that could be rendered, when a 
        rendering is performed.
     */
    sad::Rect< sad::Point3D >  m_renderable_area;
    /*! A linked texture for a sprite
     */
    sad::resource::Link<sad::Texture> m_texture;
    /*! A used color of sprite
     */
    sad::AColor  m_color;
    /*! A current color buffer, used when getting current color of scene
     */ 
    int  m_current_color_buffer[4];
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::Sprite3D)