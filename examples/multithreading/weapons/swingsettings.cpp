#include "swingsettings.h"

#include <dukpp-03/context.h>

DECLARE_COMMON_TYPE(weapons::SwingSettings)


void weapons::exposeSwingSettings(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    sad::dukpp03::ClassBinding* binding = new sad::dukpp03::ClassBinding();
    binding->addConstructor("SwingSettings", new ::dukpp03::Constructor0<sad::dukpp03::BasicContext, weapons::SwingSettings>());
    binding->addAccessor(
        "IconName",
         sad::dukpp03::getter::from(&weapons::SwingSettings::IconName),
         sad::dukpp03::setter::from(&weapons::SwingSettings::IconName)
    );
    binding->addAccessor(
        "ScaleFactor",
         sad::dukpp03::getter::from(&weapons::SwingSettings::ScaleFactor),
         sad::dukpp03::setter::from(&weapons::SwingSettings::ScaleFactor)
    );
    binding->addAccessor(
        "DecayTime",
         sad::dukpp03::getter::from(&weapons::SwingSettings::DecayTime),
         sad::dukpp03::setter::from(&weapons::SwingSettings::DecayTime)
    );
    ctx->addClassBinding("weapons::SwingSettings", binding);
}
