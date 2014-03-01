#include "util/deletetexturetask.h"
#include "os/glheaders.h"

sad::util::DeleteTextureTask::DeleteTextureTask(unsigned int id)
: m_texture_id(id)
{

}

sad::util::DeleteTextureTask::~DeleteTextureTask()
{

}

void sad::util::DeleteTextureTask::_process()
{
	glDeleteTextures(1, &m_texture_id);
}
