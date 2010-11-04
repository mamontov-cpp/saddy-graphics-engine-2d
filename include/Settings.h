/*! \file   setting.h
    \author FreakyBlast

	\brief  Declaration of SadSettings and helper typ.

	This file contains a declaration of SadTimer. 
*/

#ifndef Settings_h__
#define Settings_h__


#include "types.h"


/*!\class Setting class
*/
class SadSettings
{
public:
	SadSettings(void);
	~SadSettings(void);

	
	hString TITLE;   //!< Name of window (titlebar)

	UINT SCREEN_WIDTH;  //!< Width of screen
	
	UINT SCREEN_HEIGHT;  //!< Height of screen
	
	BOOL FULLSCREEN;  //!< Whether fullscreen
	
	BYTE BPP;  //!< Depth color (bit per pixel)

	BYTE DEPTH;  //!< bit for z-buffer

	float FOV; //!< Angle 

	float ZNEAR;  //!< Ѕлижн€€ плоскость отсечени€
	
	float ZFAR;  //!< ƒальн€€ плоскость отсечени€
};


extern SadSettings MainSettings;  //!< Global variable of settings


#endif // Settings_h__