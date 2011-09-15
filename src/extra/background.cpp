#include "background.h"

#ifndef LINUX

#include <windows.h>
#include <gl/gl.h>														
#include <gl/glu.h>	
#else

#include <GL/gl.h>														
#include <GL/glu.h>	

#endif

SAD_DECLARE(Background,Sprite)

Background::Background(const hst::string &name): 
Sprite(
	   sad::TextureManager::instance()->get(name),
	   hst::rect<::s3d::point>(::s3d::point(0,480,0),::s3d::point(640,0,0)),
	   hRectF(hPointF(0.0f,0.0f),hPointF(512.0f,512.0f))
	  )
{
}

Background::~Background()
{

}
