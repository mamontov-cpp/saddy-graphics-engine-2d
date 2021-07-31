/*! \file quitreason.h
    

    Describes a reason, why editor must be quit. For example it will be quit by closing saddy's rendering
    window or closing main panel of editor. 
 */
#pragma once

namespace core
{
/*! Describes a reason, why editor must be quit. For example it will be quit by closing saddy's rendering
    window or closing main panel of editor. 
 */
enum class QuitReason: int
{
    /*! A reason is not specified
     */
    QR_NOTSET = 0,
    /*! A saddy renderer's window is closed
     */
    QR_SADDY = 1,
    /*! A qt window main panel is closed
     */
    QR_QTWINDOW = 2
};

}
