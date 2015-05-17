#include "glueorder.h"


GlueOrder::GlueOrder() : Mode(HORIZONTAL)
{

}


GlueOrder::GlueOrder(size_t i1, size_t i2, GlueMode mode)
: Mode(mode)
{
    Images.push_back(i1);
    Images.push_back(i2);
}

GlueOrder::~GlueOrder()
{

}
