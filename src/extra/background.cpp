#include "background.h"
#include "../renderer.h"

using namespace sad;

SAD_DECLARE(Background,Sprite)

#define SCREEN_HEIGHT  (sad::Renderer::ref()->settings().height())
#define SCREEN_WIDTH   (sad::Renderer::ref()->settings().width())

Background::Background(const hst::string &name,const hRectF & r): 
Sprite(
	   sad::TextureManager::ref()->get(name),
	   hst::rect< ::s3d::point>(::s3d::point(0.0f,(float)SCREEN_HEIGHT,0.0f),::s3d::point((float)SCREEN_WIDTH,0.0f,0.0f)),
	   r)
{
}
#define TEX_PTR (sad::TextureManager::ref()->get(name))
Background::Background(const hst::string &name): 
Sprite(
	   TEX_PTR,
	   hst::rect< ::s3d::point>(::s3d::point(0.0f,(float)SCREEN_HEIGHT,0.0f),::s3d::point((float)SCREEN_WIDTH,0.0f,0.0f)),
	   hRectF(hPointF(0,0),hPointF(TEX_PTR->width(),TEX_PTR->height())))
{
}


Background::~Background()
{

}
