/*! \file   settings.h
	\author FreakyBlast

	\brief  Declaration of SadSettings.

	This file contains a declaration of SadSettings - settings upon OpenGL. 
*/
#pragma once
#define DEFAULT_DEPTH_VALUE 0.8f //!< Value, which gives us a z=0.5 in mapping coordinates

namespace sad
{

/*! \class Settings 
	Describes a settings, passed to sad::Renderer
*/
class Settings
{
 private:
  
  unsigned int m_screenwidth;  //!< Width of screen
  unsigned int m_screenheight; //!< Height of screen
  bool m_isfullscreen;         //!< Whether we are going fullscreen
  sad::uchar m_bpp;                 //!< Bits per pixel
  sad::uchar m_depth;               //!< Bits for z-buffer
  float m_fov;                 //!< Angle 
  float m_znear;               //!< Near-clipping plane
  float m_zfar;                //!< Far-clipping plane
  bool  m_ztest; //!< Whether we are going to do depth-test, or simply use zvalue to determine z
  float m_zvalue; //!< Zvalue for mapping
 public:
  Settings(
		   unsigned int screenwidth,
		   unsigned int screenheight,
		   bool isfullscreen,
		   sad::uchar bpp=24,
		   sad::uchar depth=8,
		   float fov=45.0,
		   float znear=0.1f,
		   float zfar=100.0f,
		   bool  ztest=false,
		   float zvalue=DEFAULT_DEPTH_VALUE
		  ); //!< Default constructor
  Settings(const Settings& other);
  Settings();
 ~Settings(void);

 inline unsigned int width(void)  const     {return m_screenwidth;     }
 inline unsigned int height(void) const     {return m_screenheight;    }
 inline bool isFullscreen(void)const        {return m_isfullscreen;    }
 inline sad::uchar bpp(void)const                {return m_bpp;             }
 inline sad::uchar depth(void)const              {return m_depth;           }
 inline float fov(void)const                {return m_fov;             }
 inline float znear(void)const              {return m_znear;           }
 inline float zfar(void)const               {return m_zfar;            }
 inline bool  ztest(void)const              {return m_ztest;           }
 inline float ztestvalue(void)const         {return m_zvalue;          }
 
 inline void setIsFullscreen(bool value)         {m_isfullscreen=value;}
 inline void setWidthScreen(unsigned int value)  {m_screenwidth=value; }
 inline void setHeightScreen(unsigned int value) {m_screenheight=value;}
 inline void setBPP(sad::uchar value)                 {m_bpp=value;         }
 inline void setFoV(float value)                 {m_fov=value;         }
 inline void setZNear(float value)               {m_znear=value;       }
 inline void setZFar(float value)                {m_zfar=value;        }
 inline void setDepth(sad::uchar value)               {m_depth=value;       }
 inline void setZTest(bool value)                {m_ztest=value;       }
 inline void setZTestValue(float value)          {m_zvalue=value;      }
};

}
