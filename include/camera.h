/*! \file camera.h
    

    Describes a camera, which defines a rotation and translation action of scene
 */
#pragma once
#include "sadpoint.h"
#include "object.h"

namespace sad
{
class Scene;
/*! A camera, which rotates and translates all space, using OpenGL functions, first
    translates using glTranslatef, than rotates using glRotatef
 */
class Camera: public sad::Object
{
SAD_OBJECT
public:
    /*! Default camera set all transformations to zero, so view won't be changed 
     */
    Camera();
    /*! Adds a vector to current translation vector, changing point of view
        \param[in] v vector
     */
    inline void move(const sad::Vector3D & v)
    {
        m_translation_offset += v;
        m_transform_is_cached = false;
    }
    /*! Adds a vector to current translation vector, changing point of view
        \param[in] v vector
     */
    inline void move(const sad::Point2D & v)
    {
        m_translation_offset += v;
        m_transform_is_cached = false;
    }
    /*! Sets scene for camera
     *  \param[in] s scene
     */
    void setScene(sad::Scene* s);
    /*! Returns scene for camera
     *  \return scene
     */
    sad::Scene* scene() const;
    /*! Sets translation offset for camera
     *  \param[in] v vector
     */
    void setTranslationOffset(const sad::Vector3D& v);
    /*! Returns translation offset for camera
     *  \return translation offset
     */
    const sad::Vector3D& translationOffset() const;
    /*! Sets rotation vector direction
     *  \param[in] v rotation vector direction
     */
    void setRotationVectorDirection(const sad::Vector3D& v);
    /*! Returns rotation vector direction
     *  \return rotation vector direction
     */
    const sad::Vector3D& rotationVectorDirection() const;
    /*! Sets rotation angle for camera
     * \param[in] angle angle
     */
    void setAngle(double angle);
    /*! Returns angle for camera
     * \return angle
     */
    double angle() const;
    /*! Sets temporary rotation offset
     *  \param[in] o offset
     */
    void setTemporaryRotationOffset(const sad::Vector3D& o);
    /*! Returns temporary rotation offset
     * \return offset
     */
    const sad::Vector3D& temporaryRotationOffset() const;
    /*! Applies a camera transformation, 
        firstly using glTranslatef, than glRotatef
     */
    virtual void apply();
    /*! Restores a camera transformation
     */
    virtual void restore();
    /*! Returns pointer to 16 floats, that contains a projection matrix
     * \return projection matrix
     */
    float* projectionMatrix();
    /*! Returns pointer to 16 floats, that contains a modelview matrix
     * \return projection matrix
     */
    float* modelViewMatrix();
    /*! Clears transform cache for camera
     */
    void clearTransformCache();
    /*! Forces moving matrices into camera buffer
     */
    void moveMatricesIntoCameraBuffer();
    /*! Forces re-computing matrices 
     */
    void forceRecomputeMatrices();
    /*! You can define your camera, which can be used if you want to bound rotation,
        move around point and other stuff
     */
    virtual ~Camera();
protected:
    /*! Whether transformation is cached
     */
    bool m_transform_is_cached;
    /*! Projection matrix
     */
    float m_projection_matrix[16];
    /*! A model view matrix
     */
    float m_model_view_matrix[16];
    /*! An offset, that is substituted to glTranslatef
     */
    sad::Vector3D  m_translation_offset;
    /*! An angle, substituted to camera
     */
    double m_angle;
    /*! An offset for temporary rotation
     */
    sad::Vector3D m_temporary_rotation_offset;
    /*! A pivot vector point for rotation, used to be substituted to glRotatef
     */
    sad::Vector3D  m_rotation_vector_direction;
    /*! Attached scene to a camera
     */
    sad::Scene*   m_scene;
};

}
