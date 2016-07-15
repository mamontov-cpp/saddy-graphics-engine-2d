/*! \file cell.h
    

    Describes a cell parameters in table widget
 */
#pragma once

namespace gui
{

namespace resourcetreewidget
{
/*! Describes a basic parameter of cell
 */
struct Cell
{
    /*! A width of cell
     */
    static const int Width;
    /*! A height of cell
     */
    static const int Height;
    /*! A width of image part of cell
     */
    static const int ImageWidth;
    /*! A height of image part of cell
     */
    static const int ImageHeight;
    /*! A space between image and text
     */
    static const int SpaceBetweenImageAndText;
    /*! A font size for a label of cell
     */
    static const int FontSize;
};

}

}
