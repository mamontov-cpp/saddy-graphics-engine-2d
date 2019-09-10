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
    r->init(sad::Settings(640, 480, false));

    r->log()->addTarget(new sad::log::ConsoleTarget());
    sad::Scene* scene = new sad::Scene();
    scene->setCamera(new sad::OrthographicCamera(640, 480));
    sad::Sprite2D* sprite2d = new sad::Sprite2D(NULL, sad::Rect2D(0, 0, 512, 512), sad::Rect2D(-1, -1, 480, 480));
    sprite2d->setColor(sad::AColor(255, 0, 0));
    scene->addNode(sprite2d);
    r->addScene(scene);
    r->run();
    // If we are here, this means, that window is closed and we can exit the main loop
    return 0;
}

