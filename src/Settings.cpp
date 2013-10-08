#include "settings.h"



namespace sad{

	sad::Settings::Settings()
	{

	}

	sad::Settings::Settings(unsigned int screenwidth, unsigned int screenheight,
		bool isfullscreen, sad::uchar bpp,
		sad::uchar depth, float fov, float znear, float zfar, bool ztest, float zvalue)
	{

		m_screenwidth = screenwidth;
		m_screenheight = screenheight;
		m_isfullscreen = isfullscreen;
		m_bpp = bpp;
		m_depth = depth;
		m_fov = fov;
		m_znear = znear;
		m_zfar = zfar;
		m_ztest=ztest;
		m_zvalue=zvalue;
	}
	sad::Settings::~Settings(void)
	{
	}
	sad::Settings::Settings(const sad::Settings & o)
	{
		m_screenwidth =o.m_screenwidth;
		m_screenheight = o.m_screenheight;
		m_isfullscreen = o.m_isfullscreen;
		m_bpp = o.m_bpp;
		m_depth = o.m_depth;
		m_fov = o.m_fov;
		m_znear = o.m_znear;
		m_zfar = o.m_zfar;
		m_ztest=o.m_ztest;
		m_zvalue=o.m_zvalue;
	}


}
