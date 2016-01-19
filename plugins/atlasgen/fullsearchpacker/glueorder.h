/*! \file glueorder.h
    \authir HiddenSeeker

    Defines a list of images and how they should be glued
 */
#pragma once
#include "gluemode.h"

#include <QtCore/QVector>

namespace fullsearchpacker
{

/*! Defines a list of images and how they should be glued
 */
class GlueOrder
{
public:
    /*! A list of images
     */
    QVector<size_t> Images;
    /*! How should images be glued
     */
    fullsearchpacker::GlueMode Mode;

    /*! Default constructor
     */
    GlueOrder();
    /*! Constructs new order with specified parameters
       \param i1 first image
       \param i2 second image
       \param mode mode
     */
    GlueOrder(size_t i1, size_t i2, fullsearchpacker::GlueMode mode);
    /*! Could be inherited
     */
    virtual ~GlueOrder();
};


}
