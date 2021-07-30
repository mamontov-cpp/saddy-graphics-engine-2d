#include "glueorder.h"


fullsearchpacker::GlueOrder::GlueOrder() : Mode(fullsearchpacker::GlueMode::HORIZONTAL)
{

}


fullsearchpacker::GlueOrder::GlueOrder(size_t i1, size_t i2, fullsearchpacker::GlueMode mode)
: Mode(mode)
{
    Images.push_back(i1);
    Images.push_back(i2);
}

fullsearchpacker::GlueOrder::~GlueOrder()
{

}
