#include "imagearranger.h"

#include <cstdio>

ImageArranger::Bucket::Bucket()
{

}

ImageArranger::Bucket::Bucket(Texture *t)
{
	this->Images << t;
	this->Size = t->size();
}

ImageArranger::Bucket::~Bucket()
{

}

void ImageArranger::Bucket::shift(const QPointF& p) const
{
	for(size_t i = 0; i < this->Images.size(); i++)
	{
		Texture* t = this->Images[i];
		t->TextureRectangle.moveTopLeft(
			t->TextureRectangle.topLeft() + p
		);
	}
}

ImageArranger::Bucket ImageArranger::Bucket::merge(
			const ImageArranger::Bucket& bucket1,
			const ImageArranger::Bucket& bucket2,
			const GlueOrder& order
)
{
	ImageArranger::Bucket result;
	result.Images = bucket1.Images;
	result.Images << bucket2.Images;

	if (order.Mode == HORIZONTAL)
	{
		// Handle horizontal merge
        result.Size = QSizeF(
			bucket1.Size.width() + bucket2.Size.width(), 
			std::max(bucket1.Size.height() , bucket2.Size.height())
		);
        bucket2.shift(QPointF(bucket1.Size.width(),0));
	} 
	else
	{
        // Handle vertical merge
		result.Size = QSizeF(
			std::max(bucket1.Size.width() , bucket2.Size.width()),
			bucket1.Size.height() + bucket2.Size.height()	
		);
		bucket2.shift(QPointF(0, bucket1.Size.height()));
	}
    return result;
}


ImageArranger::ImageArranger()
{

}

ImageArranger::~ImageArranger()
{

}

double ImageArranger::nextPOT(double value)
{
	unsigned int  size = 1;
	while(size < value)
	{
		size =  size << 1;
	}
	return static_cast<double>(size);
}


double ImageArranger::arrange(
	const TextureArray& images, 
	const QVector<GlueOrder>& order, 
	const QSizeF& totalSize
)
{
	if (images.size() > 0)
	{
		if (images.size() == 1)
		{
			images[0]->TextureRectangle = QRectF(QPointF(0, 0), images[0]->TextureRectangle.size());
		}
		else
		{	
			QVector<ImageArranger::Bucket> buckets;
			for(size_t i = 0; i < images.size(); i++)
			{
				buckets << ImageArranger::Bucket(images[i]);
			}
			for(size_t i = 0; i < order.size(); i++)
			{
				GlueOrder entry = order[i];

				QVector<size_t> indexes = entry.Images;
				if (indexes[0] < indexes[1])
				{
					std::swap(indexes[0], indexes[1]);
				}
				
				ImageArranger::Bucket new_bucket =  ImageArranger::Bucket::merge(buckets[entry.Images[0]], buckets[entry.Images[1]], entry);
				buckets.remove(indexes[0]);
				buckets.remove(indexes[1]);
				buckets << new_bucket;
			}
		}
	}
    double largesize = std::max(totalSize.width(), totalSize.height());
    //printf("Total size: %lf\n", largesize);
    return ImageArranger::nextPOT(largesize);
}


