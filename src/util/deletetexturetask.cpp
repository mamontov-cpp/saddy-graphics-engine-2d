#include "util/deletetexturetask.h"
#include "os/glheaders.h"

sad::util::DeleteTextureTask::DeleteTextureTask(unsigned int id)
{
    m_texture_ids << id;
}

sad::util::DeleteTextureTask::DeleteTextureTask(const sad::Vector<unsigned int> ids) : m_texture_ids(ids)
{

}

sad::util::DeleteTextureTask::~DeleteTextureTask()
{

}

void sad::util::DeleteTextureTask::_process()
{
    glDeleteTextures(m_texture_ids.size(), &(m_texture_ids[0]));
}
