#include "background.h"
#include "../renderer.h"

SAD_DECLARE(Background,Sprite)

#define SCREEN_HEIGHT  (sad::Renderer::instance().settings().height())
#define SCREEN_WIDTH   (sad::Renderer::instance().settings().width())

Background::Background(const hst::string &name,const hRectF & r): 
Sprite(
	   sad::TextureManager::instance()->get(name),
	   hst::rect< ::s3d::point>(::s3d::point(0,SCREEN_HEIGHT,0),::s3d::point(SCREEN_WIDTH,0,0)),
	   r)
{
}
#define TEX_PTR (sad::TextureManager::instance()->get(name))
Background::Background(const hst::string &name): 
Sprite(
	   TEX_PTR,
	   hst::rect< ::s3d::point>(::s3d::point(0,SCREEN_HEIGHT,0),::s3d::point(SCREEN_WIDTH,0,0)),
	   hRectF(hPointF(0,TEX_PTR->height()),hPointF(TEX_PTR->width(),0)))
{
}


Background::~Background()
{

}
