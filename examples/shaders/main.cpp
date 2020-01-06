/*! \mainpage shaders
    
    Describes a simple shooting game example,
    demonstrating engine basics.
 */
/*! \file main.cpp
    

    Describes a main file for game
 */

#include <renderer.h>
#include <texturemappedfont.h>
#include <mousecursor.h>
#include <sprite2d.h>
#include <sprite3d.h>
#include <objectdependentfpsinterpolation.h>
#include <orthographiccamera.h>
#include <log/consoletarget.h>

#include <math.h>
#include <time.h>

/*! A main function 
    \param[in] argc count of arguments
    \param[in] argv arguments
 */
int main(int argc, char** argv)
{   
    sad::Renderer* r = sad::Renderer::ref();
    sad::Vector<sad::resource::Error*> errors = r->loadResources("examples/stress-physics.json");
    sad::String errortext;
    if (!errors.empty())
    {
        SL_LOCAL_FATAL(sad::resource::format(errors), *r);
        return 1;
    }
    sad::util::free(errors);

    r->init(sad::Settings(640, 480, false));

    r->log()->addTarget(new sad::log::ConsoleTarget());
    sad::Scene* scene = new sad::Scene();
    scene->setCamera(new sad::OrthographicCamera(640, 480));
    {
        sad::Sprite3D* sprite3d = new sad::Sprite3D(r->texture("objects"), sad::Rect2D(1, 1, 86, 86), sad::Rect<sad::Point3D>(sad::Point3D(160, 160, 0.0), sad::Point3D(320, 480, 0.0)));
        sprite3d->rotate(0.75, 0);
        sprite3d->setColor(sad::AColor(255, 128, 255, 128.0));
        scene->addNode(sprite3d);
    }
    {
        sad::Sprite2D* sprite2d = new sad::Sprite2D(NULL, sad::Rect2D(0, 0, 512, 512), sad::Rect2D(160, 160, 320, 480));
        sprite2d->setAngle(-0.75);
        sprite2d->setColor(sad::AColor(0, 0, 255, 128.0));
        scene->addNode(sprite2d);
    }
    {
        sad::Sprite2D* sprite2d = new sad::Sprite2D(r->texture("objects"), sad::Rect2D(sad::Point2D(1, 1), sad::Point2D(86, 86)), sad::Rect2D(120, 120, 205, 205));
        sprite2d->setAngle(-0.75);
        sprite2d->setColor(sad::AColor(0, 255, 0));
        scene->addNode(sprite2d);
    }
    {
        sad::Sprite3D* sprite3d = new sad::Sprite3D(NULL, sad::Rect2D(1, 1, 86, 86), sad::Rect<sad::Point3D>(sad::Point3D(360, 240, 0.0), sad::Point3D(440, 480, 0.0)));
        sprite3d->rotate(0.75, 0);
        sprite3d->setColor(sad::AColor(255, 128, 255, 128.0));
        scene->addNode(sprite3d);
    }
    r->addScene(scene);
    r->run();
    // If we are here, this means, that window is closed and we can exit the main loop
    return 0;
}

