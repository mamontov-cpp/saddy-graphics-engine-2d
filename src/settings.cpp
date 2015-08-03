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


}
