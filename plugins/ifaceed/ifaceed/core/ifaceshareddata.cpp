#include "ifaceshareddata.h"
#include "ifaceeditor.h"

sad::Sprite2DConfig * IFaceSharedData::icons()
{
	return &(this->m_editor->icons());
}


sad::log::Log * IFaceSharedData::log()
{
	return sad::log::Log::ref();
}
