#include "platform.h"

DECLARE_SOBJ_INHERITANCE(Platform, sad::p2d::app::Object)

Platform::Platform()
{
    this->initFromConstants<Platform>();
}
