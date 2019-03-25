#include "lasersettings.h"

#include <dukpp-03/context.h>

DECLARE_COMMON_TYPE(weapons::LaserSettings)


void weapons::exposeLaserSettings(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    sad::dukpp03::ClassBinding* binding = new sad::dukpp03::ClassBinding();
    binding->addConstructor("LaserSettings", new ::dukpp03::Constructor0<sad::dukpp03::BasicContext, weapons::LaserSettings>());
    binding->addAccessor(
        "IconName",
         sad::dukpp03::getter::from(&weapons::LaserSettings::IconName),
         sad::dukpp03::setter::from(&weapons::LaserSettings::IconName)
    );
    binding->addAccessor(
        "SpriteWidth",
         sad::dukpp03::getter::from(&weapons::LaserSettings::SpriteWidth),
         sad::dukpp03::setter::from(&weapons::LaserSettings::SpriteWidth)
    );
    binding->addAccessor(
        "PhysicalWidth",
        sad::dukpp03::getter::from(&weapons::LaserSettings::PhysicalWidth),
        sad::dukpp03::setter::from(&weapons::LaserSettings::PhysicalWidth)
    );
    binding->addAccessor(
        "Height",
         sad::dukpp03::getter::from(&weapons::LaserSettings::Height),
         sad::dukpp03::setter::from(&weapons::LaserSettings::Height)
    );
    binding->addAccessor(
        "Time",
         sad::dukpp03::getter::from(&weapons::LaserSettings::Time),
         sad::dukpp03::setter::from(&weapons::LaserSettings::Time)
    );
    binding->addAccessor(
        "SoundName",
        sad::dukpp03::getter::from(&weapons::LaserSettings::SoundName),
        sad::dukpp03::setter::from(&weapons::LaserSettings::SoundName)
    );
    ctx->addClassBinding("weapons::LaserSettings", binding);
}
