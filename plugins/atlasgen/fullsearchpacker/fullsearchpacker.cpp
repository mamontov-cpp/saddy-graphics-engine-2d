#include "fullsearchpacker.h"

#include "imagegluingorder.h"
#include "imagearranger.h"

#include <QPainter>

fullsearchpacker::FullSearchPacker::FullSearchPacker()
{
	
}

void fullsearchpacker::FullSearchPacker::pack(Atlas& atlas, QImage*& image)
{
	fullsearchpacker::ImageGluingOrder gluingorder;
	fullsearchpacker::ImageGluingOrder::Result gluingorderresult = gluingorder.find(atlas.Textures);
                    
	fullsearchpacker::ImageArranger arranger;
	double widthheight = arranger.arrange(atlas.Textures,gluingorderresult.Order,gluingorderresult.Size);

	image = new QImage(widthheight, widthheight, QImage::Format_ARGB32);
	image->fill(QColor(255, 255, 255, 0));

	QPainter painter(image);
	for(size_t i = 0; i < atlas.Textures.size(); i++)
	{
		painter.drawImage(
			atlas.Textures[i]->TextureRectangle.topLeft(),
			atlas.Textures[i]->Image
		);
	}
	painter.end();
}

fullsearchpacker::FullSearchPacker::~FullSearchPacker()
{
	
}
