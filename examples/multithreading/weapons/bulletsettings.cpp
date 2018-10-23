#include "bulletsettings.h"

#include <dukpp-03/context.h>

DECLARE_COMMON_TYPE(weapons::BulletSettings)


void weapons::exposeBulletSettings(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    sad::dukpp03::ClassBinding* binding = new sad::dukpp03::ClassBinding();
    binding->addConstructor("BulletSettings", new ::dukpp03::Constructor0<sad::dukpp03::BasicContext, weapons::BulletSettings>());
    binding->addAccessor(
        "IconName",
         sad::dukpp03::getter::from(&weapons::BulletSettings::IconName),
         sad::dukpp03::setter::from(&weapons::BulletSettings::IconName)
    );
    binding->addAccessor(
        "Speed",
         sad::dukpp03::getter::from(&weapons::BulletSettings::Speed),
         sad::dukpp03::setter::from(&weapons::BulletSettings::Speed)
    );
    binding->addAccessor(
        "ApplyGravity",
         sad::dukpp03::getter::from(&weapons::BulletSettings::ApplyGravity),
         sad::dukpp03::setter::from(&weapons::BulletSettings::ApplyGravity)
    );
    binding->addAccessor(
        "GravityValue",
         sad::dukpp03::getter::from(&weapons::BulletSettings::GravityValue),
         sad::dukpp03::setter::from(&weapons::BulletSettings::GravityValue)
    );
    binding->addAccessor(
        "AngularSpeed",
         sad::dukpp03::getter::from(&weapons::BulletSettings::AngularSpeed),
         sad::dukpp03::setter::from(&weapons::BulletSettings::AngularSpeed)
    );
    binding->addAccessor(
        "IsGhost",
         sad::dukpp03::getter::from(&weapons::BulletSettings::IsGhost),
         sad::dukpp03::setter::from(&weapons::BulletSettings::IsGhost)
    );
    binding->addAccessor(
        "MaxBounceCount",
         sad::dukpp03::getter::from(&weapons::BulletSettings::MaxBounceCount),
         sad::dukpp03::setter::from(&weapons::BulletSettings::MaxBounceCount)
    );
    binding->addAccessor(
        "BounceResilienceCoefficient",
         sad::dukpp03::getter::from(&weapons::BulletSettings::BounceResilienceCoefficient),
         sad::dukpp03::setter::from(&weapons::BulletSettings::BounceResilienceCoefficient)
    );
    binding->addAccessor(
        "IsPiercing",
         sad::dukpp03::getter::from(&weapons::BulletSettings::IsPiercing),
         sad::dukpp03::setter::from(&weapons::BulletSettings::IsPiercing)
    );
    binding->addAccessor(
        "IsNavigatingGravityToDirection",
        sad::dukpp03::getter::from(&weapons::BulletSettings::IsNavigatingGravityToDirection),
        sad::dukpp03::setter::from(&weapons::BulletSettings::IsNavigatingGravityToDirection)
    );
    ctx->addClassBinding("weapons::BulletSettings", binding);
}
