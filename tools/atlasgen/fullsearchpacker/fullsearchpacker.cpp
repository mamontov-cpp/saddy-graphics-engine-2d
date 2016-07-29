#include "fullsearchpacker.h"

#include "imagegluingorder.h"
#include "imagearranger.h"

fullsearchpacker::FullSearchPacker::FullSearchPacker()
{
    
}

void fullsearchpacker::FullSearchPacker::pack(Atlas& atlas, QImage*& image)
{
    fullsearchpacker::ImageGluingOrder gluingorder;
    fullsearchpacker::ImageGluingOrder::Result gluingorderresult = gluingorder.find(atlas.textures(), *m_options);
                    
    fullsearchpacker::ImageArranger arranger;
    double widthheight = arranger.arrange(
        *m_options,
        atlas.textures(),
        gluingorderresult.Order,
        gluingorderresult.Size
    );

    image = new QImage(widthheight, widthheight, QImage::Format_ARGB32);
    image->fill(QColor(255, 255, 255, 0));

    this->copyImages(atlas, image);
}

fullsearchpacker::FullSearchPacker::~FullSearchPacker()
{
    
}
