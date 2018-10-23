/*! \file lasersettings.h

    A settings for in-game laser
 */
#pragma once
#include <sadstring.h>


namespace weapons
{

/*! A settings for laser
 */
class LaserSettings
{
public:
    /*! An icon name for sprite
     */
    sad::String IconName;
    /*! A width for sprite
     */
    double Width;
    /*! A height for sprite
     */
    double Height;
    /*! An existence time for sprite
     */
    double Time;

    /*! A settings for laser
     */
    inline LaserSettings() : IconName("bullets/green/x_huge"), Width(10), Height(600), Time(500)
    {

    }

};

/*! Exposes laser settings to context
    \param[in] c context
 */
void exposeLaserSettings(void* c);

}

