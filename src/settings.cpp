#include "settings.h"
#include "db/dbtypename.h"

sad::Settings::Settings() 
: m_screenwidth(800.0),
  m_screenheight(600.0),
  m_isfullscreen(false),
  m_bpp(24),
  m_depth(8),
  m_fov(45.0),
  m_znear(0.1f),
  m_zfar(100.0f),
  m_ztest(false),
  m_zvalue(0.8)
{

}

sad::Settings::Settings(
    unsigned int screenwidth, 
    unsigned int screenheight,
    bool isfullscreen, 
    sad::uchar bpp,
    sad::uchar depth, 
    float fov, 
    float znear, 
    float zfar, 
    bool ztest, 
    float zvalue
)
: m_screenwidth(screenwidth),
  m_screenheight(screenheight),
  m_isfullscreen(isfullscreen),
  m_bpp(bpp),
  m_depth(depth),
  m_fov(fov),
  m_znear(znear),
  m_zfar(zfar),
  m_ztest(ztest),
  m_zvalue(zvalue)
{

}

sad::Settings::~Settings(void)
{

}

DECLARE_COMMON_TYPE(sad::Settings);