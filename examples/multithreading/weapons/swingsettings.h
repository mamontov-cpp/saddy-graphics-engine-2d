/*! \file swingsettings.h

    A swing settings for swinging weapons
 */
#pragma once
#include <sadstring.h>

namespace weapons
{

/*! A settings for swing
 */
class SwingSettings
{
public:
    /*! An icon name
     */
    sad::String IconName;
    /*! A scale factor for weapon icon
     */
    double ScaleFactor;
    /*! A decay time for swing
     */
    double DecayTime;

    /*! Constructs default settings
     */
    inline SwingSettings() : IconName("icons_list/S_Sword01ng"), ScaleFactor(2), DecayTime(5000)
    {

    }
};


/*! Exposes swing settings to context
    \param[in] c context
 */
void exposeSwingSettings(void* c);

}
