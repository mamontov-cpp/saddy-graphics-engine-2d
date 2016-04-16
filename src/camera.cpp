#include <camera.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>														
#include <GL/glu.h>

sad::Camera::Camera() 
: TranslationOffset(0,0,0), Angle(0), RotationVectorDirection(0, 0, 0), TemporaryRotationOffset(0, 0, 0), Scene(NULL)
{

}

void sad::Camera::apply()
{
    glTranslatef(
        (GLfloat)(TranslationOffset.x()),
        (GLfloat)(TranslationOffset.y()),
        (GLfloat)(TranslationOffset.z())
    );
    glTranslatef(
        (GLfloat)(TemporaryRotationOffset.x()),
        (GLfloat)(TemporaryRotationOffset.y()),
        (GLfloat)(TemporaryRotationOffset.z())
    );
    glRotatef(
        (GLfloat)(Angle),
        (GLfloat)(RotationVectorDirection.x()),
        (GLfloat)(RotationVectorDirection.y()),
        (GLfloat)(RotationVectorDirection.z())
    );
    glTranslatef(
        (GLfloat)(-(TemporaryRotationOffset.x())),
        (GLfloat)(-(TemporaryRotationOffset.y())),
        (GLfloat)(-(TemporaryRotationOffset.z()))
    );
}

sad::Camera::~Camera()
{

}
