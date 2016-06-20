#include <camera.h>
#include <scene.h>
#include <renderer.h>

#ifdef WIN32
// ReSharper disable once CppUnusedIncludeDirective
#include <windows.h>
#endif

#include <GL/gl.h>                                                      
// ReSharper disable once CppUnusedIncludeDirective
#include <GL/glu.h>

sad::Camera::Camera() 
: TranslationOffset(0,0,0), Angle(0), TemporaryRotationOffset(0, 0, 0), RotationVectorDirection(0, 0, 0), Scene(NULL)
{

}

void sad::Camera::apply()
{
    sad::Vector3D vector3 = TranslationOffset;  
    sad::Scene* scene = Scene;
    if (scene)
    {
        sad::Renderer* renderer  = Scene->renderer();
        if (renderer)
        {
            vector3 += renderer->globalTranslationOffset();
        }
    }
    glPushMatrix();
    glTranslatef(
        static_cast<GLfloat>(vector3.x()),
        static_cast<GLfloat>(vector3.y()),
        static_cast<GLfloat>(vector3.z())
    );
    glTranslatef(
        static_cast<GLfloat>(TemporaryRotationOffset.x()),
        static_cast<GLfloat>(TemporaryRotationOffset.y()),
        static_cast<GLfloat>(TemporaryRotationOffset.z())
    );
    glRotatef(
        static_cast<GLfloat>(Angle),
        static_cast<GLfloat>(RotationVectorDirection.x()),
        static_cast<GLfloat>(RotationVectorDirection.y()),
        static_cast<GLfloat>(RotationVectorDirection.z())
    );
    glTranslatef(
        static_cast<GLfloat>(-(TemporaryRotationOffset.x())),
        static_cast<GLfloat>(-(TemporaryRotationOffset.y())),
        static_cast<GLfloat>(-(TemporaryRotationOffset.z()))
    );
}

void sad::Camera::restore()
{
    glPopMatrix();	
}

sad::Camera::~Camera()
{

}
