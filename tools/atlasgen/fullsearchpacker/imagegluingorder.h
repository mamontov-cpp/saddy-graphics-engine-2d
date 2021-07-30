/*! \file imagegluingorder.h
    

    A class, which can find an order in which images should be glued.
    Merging images on this order, must lead to a texture with minimum width and height
 */
#pragma once
#include "../texturearray.h"
#include "glueorder.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QSizeF>

namespace fullsearchpacker
{

/*! A class, which can find an order in which images should be glued.
    Merging images on this order, must lead to a texture with minimum width and height
 */
class ImageGluingOrder
{
public:
    /*! A result of seeking order
     */
    struct Result
    {
        /*! Order, in which images should be merged
         */
        QVector<fullsearchpacker::GlueOrder> Order;
        /*! A size of image
         */
        QSizeF Size;

        /*! Constructs default result
         */
        Result();
        /*! Can be inherited
         */
        virtual ~Result();
    };
    /*! Constructs default order
     */
    ImageGluingOrder();
    /*! Can be inherited
     */
    virtual ~ImageGluingOrder();
    /*! Finds an order on specified array of images.
        Merging by this order, must lead to image with minimal width and height.
        \param[in] images a list of images to be merged
        \param[in] options a program options
        \return result resulting order
     */
    fullsearchpacker::ImageGluingOrder::Result find(
        const TextureArray& images,
        const QHash<QString, QVariant>& options
    );
};

}
