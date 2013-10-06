#include "ifaceshareddata.h"
#include "ifaceeditor.h"

sad::Sprite2DConfig * IFaceSharedData::icons()
{
	return &(this->m_editor->icons());
}


sad::Log * IFaceSharedData::log()
{
	return this->m_editor->log();
}
