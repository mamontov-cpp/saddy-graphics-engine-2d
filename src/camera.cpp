#include <camera.h>
#include <scene.h>
#include <renderer.h>
#include <opengl.h>
#include <renderer.h>
#include <glcontext.h>
#include <fuzzyequal.h>
#include <os/ubo.h>

#ifdef WIN32
// ReSharper disable once CppUnusedIncludeDirective
#include <windows.h>
#endif

#include <GL/gl.h>                                                      
// ReSharper disable once CppUnusedIncludeDirective
#include <GL/glu.h>

#include <../3rdparty/glm/glm/glm.hpp>
#include <../3rdparty/glm/glm/ext.hpp>

DECLARE_SOBJ(sad::Camera)

sad::Camera::Camera() 
: m_transform_is_cached(false), m_are_matrices_uploaded(false), m_projection_matrix{0}, m_model_view_matrix{0}, m_translation_offset(0,0,0), m_angle(0), m_temporary_rotation_offset(0, 0, 0), m_rotation_vector_direction(0, 0, 0), m_scene(nullptr)
{
    m_projection_matrix[0] = 1.0f;
    m_projection_matrix[4] = 0;
    m_projection_matrix[8] = 0;
    m_projection_matrix[12] = 0;

    m_projection_matrix[1] = 0;
    m_projection_matrix[5] = 1.0f;
    m_projection_matrix[9] = 0;
    m_projection_matrix[13] = 0;

    m_projection_matrix[2] = 0;
    m_projection_matrix[6] = 0;
    m_projection_matrix[10] = 1.0f;
    m_projection_matrix[14] = 0;

    m_projection_matrix[3] = 0;
    m_projection_matrix[7] = 0;
    m_projection_matrix[11] = 0;
    m_projection_matrix[15] = 1.0f;
}

void sad::Camera::setScene(sad::Scene* s)
{
    m_scene = s;
    m_transform_is_cached = false;
    m_are_matrices_uploaded = false;
}

sad::Scene* sad::Camera::scene() const
{
    return m_scene;
}

void sad::Camera::setTranslationOffset(const sad::Vector3D& v)
{
    m_translation_offset = v;
    m_transform_is_cached = false;
    m_are_matrices_uploaded = false;
}

const sad::Vector3D& sad::Camera::translationOffset() const
{
    return m_translation_offset;
}

void sad::Camera::setRotationVectorDirection(const sad::Vector3D& v)
{
    m_rotation_vector_direction = v;
    m_transform_is_cached = false;
    m_are_matrices_uploaded = false;
}


const sad::Vector3D& sad::Camera::rotationVectorDirection() const
{
    return m_rotation_vector_direction;
}

void sad::Camera::setAngle(double angle)
{
    m_angle = angle;
    m_transform_is_cached = false;
    m_are_matrices_uploaded = false;
}

double sad::Camera::angle() const
{
    return m_angle;
}

void sad::Camera::setTemporaryRotationOffset(const sad::Vector3D& o)
{
    m_temporary_rotation_offset = o;
    m_transform_is_cached = false;
    m_are_matrices_uploaded = false;
}

const sad::Vector3D& sad::Camera::temporaryRotationOffset() const
{
    return m_temporary_rotation_offset;
}


void sad::Camera::apply()
{
    sad::Renderer* renderer = sad::Renderer::ref();
    if (m_scene)
    {
        sad::Renderer* local_renderer = m_scene->renderer();
        if (local_renderer)
        {
            renderer = local_renderer;
        }
    }
    if (renderer->context()->isOpenGL3compatible())
    {
        forceRecomputeMatrices();
        if (m_scene)
        {
            sad::os::UBO* ubo = m_scene->cameraBufferObject();
            if (!m_are_matrices_uploaded || (ubo->userData() != this))
            {
                ubo->setSubData(0, 16 * sizeof(float), this->modelViewMatrix());
                ubo->setSubData(16 * sizeof(float), 16 * sizeof(float), this->projectionMatrix());
                ubo->setUserData(this);
                m_are_matrices_uploaded = true;
            }
        }
    }
    else
    {
        sad::Vector3D vector3 = m_translation_offset;
        vector3 += renderer->globalTranslationOffset();
        glPushMatrix();
        glTranslatef(
            static_cast<GLfloat>(vector3.x()),
            static_cast<GLfloat>(vector3.y()),
            static_cast<GLfloat>(vector3.z())
        );
        glTranslatef(
            static_cast<GLfloat>(m_temporary_rotation_offset.x()),
            static_cast<GLfloat>(m_temporary_rotation_offset.y()),
            static_cast<GLfloat>(m_temporary_rotation_offset.z())
        );
        glRotatef(
            static_cast<GLfloat>(m_angle),
            static_cast<GLfloat>(m_rotation_vector_direction.x()),
            static_cast<GLfloat>(m_rotation_vector_direction.y()),
            static_cast<GLfloat>(m_rotation_vector_direction.z())
        );
        glTranslatef(
            static_cast<GLfloat>(-(m_temporary_rotation_offset.x())),
            static_cast<GLfloat>(-(m_temporary_rotation_offset.y())),
            static_cast<GLfloat>(-(m_temporary_rotation_offset.z()))
        );
    }
}

void sad::Camera::moveMatricesIntoCameraBuffer()
{
    sad::Renderer* renderer = sad::Renderer::ref();
    sad::Scene* scene = m_scene;
    if (scene)
    {
        sad::Renderer* local_renderer = m_scene->renderer();
        if (local_renderer)
        {
            renderer = local_renderer;
        }
    }
    if (renderer->context()->isOpenGL3compatible())
    {
        if (m_scene)
        {
            sad::os::UBO* ubo = m_scene->cameraBufferObject();
            if (ubo->userData() != this)
            {
                ubo->setSubData(0, 16 * sizeof(float), this->modelViewMatrix());
                ubo->setSubData(16 * sizeof(float), 16 * sizeof(float), this->projectionMatrix());
                ubo->setUserData(this);
            }
        }
    }
}

void sad::Camera::forceRecomputeMatrices()
{
    sad::Vector3D vector3 = m_translation_offset;
    sad::Scene* scene = m_scene;
    if (scene)
    {
        sad::Renderer* renderer = m_scene->renderer();
        if (renderer)
        {
            vector3 += renderer->globalTranslationOffset();
        }
    }

    float arr[16] = {
            1, 0, 0, static_cast<float>(vector3.x() + m_temporary_rotation_offset.x()),
            0, 1, 0, static_cast<float>(vector3.y() + m_temporary_rotation_offset.y()),
            0, 0, 1, static_cast<float>(vector3.z() + m_temporary_rotation_offset.z()),
            0, 0, 0, 1
    };
    glm::mat4x4 model = glm::make_mat4x4(arr);
    if (sad::non_fuzzy_zero(m_angle)) {
        glm::vec3  rotvector(
            static_cast<float>(m_rotation_vector_direction.x()),
            static_cast<float>(m_rotation_vector_direction.y()),
            static_cast<float>(m_rotation_vector_direction.z())
        );
        model = glm::rotate(model, static_cast<float>(m_angle), rotvector);
        glm::vec3  retvector(
            static_cast<float>(-m_temporary_rotation_offset.x()),
            static_cast<float>(-m_temporary_rotation_offset.y()),
            static_cast<float>(-m_temporary_rotation_offset.z())
        );
        model = glm::translate(model, rotvector);
    }
    model = glm::transpose(model);
    const float* src = static_cast<const float*>(glm::value_ptr(model));
    for (int i = 0; i < 16; ++i) {
        m_model_view_matrix[i] = src[i];
    }
    m_transform_is_cached = true;
}

void sad::Camera::clearTransformCache()
{
    m_transform_is_cached = false;
    m_are_matrices_uploaded = false;
}

void sad::Camera::restore()
{
    sad::Renderer* renderer = sad::Renderer::ref();
    if (m_scene)
    {
        sad::Renderer* local_renderer = m_scene->renderer();
        if (local_renderer)
        {
            renderer = local_renderer;
        }
    }
    if (!(renderer->context()->isOpenGL3compatible()))
    {
        glPopMatrix();
    }
}

float* sad::Camera::projectionMatrix()
{
    return &(this->m_projection_matrix[0]);
}

float* sad::Camera::modelViewMatrix()
{
    if (!this->m_transform_is_cached)
    {
         sad::Scene* localScene = scene();
         sad::Renderer* r = localScene->renderer();
         if (!r)
         {
             r = sad::Renderer::ref();
         }
         glGetFloatv(GL_MODELVIEW_MATRIX, &(this->m_model_view_matrix[0]));
         GLenum err_code = glGetError();
         if (err_code != GL_NO_ERROR)
         {
             sad::String error_string = reinterpret_cast<const char*>(gluErrorString(err_code));
             SL_LOCAL_WARNING(error_string, *r);
         }
         if (m_scene)
         {
             m_scene->cameraBufferObject()->setUserData(nullptr);
         }
         this->m_transform_is_cached = true;
    }
    return &(this->m_model_view_matrix[0]);
}

sad::Camera::~Camera()
{

}
