/*! \file camera.h
    

    Describes a camera, which defines a rotation and translation action of scene
 */
#pragma once
#include "sadpoint.h"

namespace sad
{
class Scene;
/*! A camera, which rotates and translates all space, using OpenGL functions, first
    translates using glTranslatef, than rotates using glRotatef
 */
class Camera
{
public:
    /*! Default camera set all transformations to zero, so view won't be changed 
     */
    Camera();
    /*! Adds a vector to current translation vector, changing point of view
        \param[in] v vector
     */
    inline void move(const sad::Vector3D & v)
    {
        TranslationOffset += v;
    }
    /*! Adds a vector to current translation vector, changing point of view
        \param[in] v vector
     */
    inline void move(const sad::Point2D & v)
    {
        TranslationOffset += v;
    }
    /*! Applies a camera transformation, 
        firstly using glTranslatef, than glRotatef
     */
    virtual void apply();
    /*! You can define your camera, which can be used if you want to bound rotation,
        move around point and other stuff
     */
    virtual ~Camera();
public:
    /*! An offset, that is substituted to glTranslatef
     */
    sad::Vector3D  TranslationOffset;
    /*! An angle, subsituted to camera
     */
    double Angle;
    /*! An offset for temporary rotation
     */
    sad::Vector3D TemporaryRotationOffset;
    /*! A pivot vector point for rotation, used to be substituted to glRotatef
     */
    sad::Vector3D  RotationVectorDirection;
    /*! Attached scene to a camera
     */
    sad::Scene *   Scene;
};

}
