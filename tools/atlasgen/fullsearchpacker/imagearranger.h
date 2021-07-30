/*! \file imagearranger.h
    

    Defines an image arranger, used to arrange images of textures in specified order
 */
#pragma once
#include "imagegluingorder.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QVariant>

namespace fullsearchpacker
{

/*! A class, that performs a texture rectangle computation, setting texture rectangle properties, 
    according to gluing order and computes size of output texture
 */
class ImageArranger
{
public:
    /*! A class, that holds current textures, which is glued by GlueOrder together 
     */
    class Bucket
    {
    public:
        /*! Textures, glued together
         */
        QVector<Texture*> Images;	
        /*! Width and height of bucket
         */
        QSizeF Size;
    
        /*! Creates a default bucket
         */
        Bucket();
        /*! Creates a texture bucket
            \param[in] t texture
            \param[in] pad_x x width padding
            \param[in] pad_y y height padding
         */
        Bucket(Texture *t, double pad_x = 0, double pad_y = 0);
        /*! Destroys bucket
         */
        virtual ~Bucket();
        /*! Shifts all images by the point, without modifying size. 
            Used by merged to arrange one bucket against another
            \param[in] p pivot point
         */
        void shift(const QPointF& p) const;
        /*! Merges two buckets into one in specified order
            \param[in] bucket1 first bucket
            \param[in] bucket2 second bucket
            \param[in] order an order in which they should be merged
            \return bucket
         */
        static fullsearchpacker::ImageArranger::Bucket merge(
            const fullsearchpacker::ImageArranger::Bucket& bucket1,
            const fullsearchpacker::ImageArranger::Bucket& bucket2,
            const fullsearchpacker::GlueOrder& order
        );
    };

    /*! Constructs new arranger
     */
    ImageArranger();
    /*! Could be inherited
     */
    virtual ~ImageArranger();
    
    /*! Computes next pot value for texture size
        \param[in] value value
        \return value
     */
    static double nextPOT(double value);

    /*! Arranges an images in specified order.
        Also changes images, setting their textureRectangle parameters
        \param[in] options a program options
        \param[in] images images to be arranged by specified order
        \param[in] order order in which images should be arranged
        \param[in] totalSize size of texture
        \return width and height of total texture
     */
    double arrange(
        const QHash<QString, QVariant>& options,
        const TextureArray& images, 
        const QVector<fullsearchpacker::GlueOrder>& order, 
        const QSizeF& totalSize
    );
};

}
