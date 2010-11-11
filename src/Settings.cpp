#include "..\include\Settings.h"



namespace Sad{


	Sad::Settings::Settings(unsigned int screenwidth, unsigned int screenheight,
		bool isfullscreen, Uint8 bpp,
		Uint8 depth, float fov, float znear, float zfar)
	{

		m_screenwidth = screenwidth;
		m_screenheight = screenheight;
		m_isfullscreen = isfullscreen;
		m_bpp = bpp;
		m_depth = depth;
		m_fov = fov;
		m_znear = znear;
		m_zfar = zfar;

	}
	Sad::Settings::~Settings(void)
	{
	}

}