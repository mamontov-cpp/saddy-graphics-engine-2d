#include "dukpp-03/context.h"

#include <sadpoint.h>
#include <sadsize.h>
#include <sadrect.h>
#include <p2d/vector.h>
#include <slurp.h>
#include <spit.h>

#include <renderer.h>
#include <util/fs.h>

#include <cstdio>

#include <settings.h>

void sad::dukpp03::exposeAPI(sad::dukpp03::Context* ctx)
{
    // Exposing API
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::Settings, unsigned int, unsigned int, bool>("SadSettings");
    c->addConstructor<sad::Settings, unsigned int, unsigned int, bool, sad::uchar, sad::uchar, float, float, float, bool, float>("SadSettings");
    c->addAccessor("width", sad::dukpp03::bind_method::from(&sad::Settings::width), sad::dukpp03::bind_method::from(&sad::Settings::setScreenWidth));
    c->addAccessor("height", sad::dukpp03::bind_method::from(&sad::Settings::height), sad::dukpp03::bind_method::from(&sad::Settings::setScreenHeight));
    c->addAccessor("isfullscreen", sad::dukpp03::bind_method::from(&sad::Settings::isFullscreen), sad::dukpp03::bind_method::from(&sad::Settings::setIsFullscreen));
    c->addAccessor("bpp", sad::dukpp03::bind_method::from(&sad::Settings::bpp), sad::dukpp03::bind_method::from(&sad::Settings::setBPP));
    c->addAccessor("depth", sad::dukpp03::bind_method::from(&sad::Settings::depth), sad::dukpp03::bind_method::from(&sad::Settings::setDepth));
    c->addAccessor("fov", sad::dukpp03::bind_method::from(&sad::Settings::fov), sad::dukpp03::bind_method::from(&sad::Settings::setFoV));
    c->addAccessor("znear", sad::dukpp03::bind_method::from(&sad::Settings::znear), sad::dukpp03::bind_method::from(&sad::Settings::setZNear));
    c->addAccessor("zfar", sad::dukpp03::bind_method::from(&sad::Settings::zfar), sad::dukpp03::bind_method::from(&sad::Settings::setZFar));
    c->addAccessor("ztest", sad::dukpp03::bind_method::from(&sad::Settings::ztest), sad::dukpp03::bind_method::from(&sad::Settings::setZTest));
    c->addAccessor("ztestvalue", sad::dukpp03::bind_method::from(&sad::Settings::ztestvalue), sad::dukpp03::bind_method::from(&sad::Settings::setZTestValue));
    c->addCloneValueObjectMethodFor<sad::Settings>();
    c->setPrototypeFunction("SadSettings");

    ctx->addClassBinding("sad::Settings", c);
}
