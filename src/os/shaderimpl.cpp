#include "os/shaderimpl.h"

// ======================================== PUBLIC METHODS  ========================================

sad::os::ShaderImpl::ShaderImpl() : m_is_on_gpu(false), m_program(0)
{
    
}

// ======================================== PRIVATE METHODS  ========================================

void sad::os::ShaderImpl::tryDestroy()
{
    if (m_is_on_gpu)
    {
        //glDeleteProgram(m_program);
        m_is_on_gpu = false;
    }
}