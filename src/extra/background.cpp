#include "background.h"
#include "../renderer.h"

using namespace sad;


DECLARE_SOBJ_INHERITANCE(Background,Sprite)

#define SCREEN_HEIGHT  (sad::Renderer::ref()->settings().height())
#define SCREEN_WIDTH   (sad::Renderer::ref()->settings().width())

Background::Background(const sad::String &name,const sad::Rect2D & r): 
Sprite(
	   sad::TextureManager::ref()->get(name),
	   sad::Rect< ::sad::Point3D>(::sad::Point3D(0.0f,(float)SCREEN_HEIGHT,0.0f),::sad::Point3D((float)SCREEN_WIDTH,0.0f,0.0f)),
	   r)
{
}
#define TEX_PTR (sad::TextureManager::ref()->get(name))
Background::Background(const sad::String &name): 
Sprite(
	   TEX_PTR,
	   sad::Rect< ::sad::Point3D>(::sad::Point3D(0.0f,(float)SCREEN_HEIGHT,0.0f),::sad::Point3D((float)SCREEN_WIDTH,0.0f,0.0f)),
	   sad::Rect2D(sad::Point2D(0,0),sad::Point2D(TEX_PTR->width(),TEX_PTR->height())))
{
}


Background::~Background()
{

}
