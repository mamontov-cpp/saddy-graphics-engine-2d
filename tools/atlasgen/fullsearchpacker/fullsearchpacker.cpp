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

    QPainter painter(image);
    for(size_t i = 0; i < atlas.textures().size(); i++)
    {
        painter.drawImage(
            atlas.textures()[i]->TextureRectangle.topLeft(),
            atlas.textures()[i]->Image
        );
    }
    painter.end();
}

fullsearchpacker::FullSearchPacker::~FullSearchPacker()
{
    
}
