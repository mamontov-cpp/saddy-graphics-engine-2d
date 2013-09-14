#include "gridnode.h"

DECLARE_SOBJ_INHERITANCE(GridNode, p2d::app::Object)

GridNode::GridNode()
{
	this->initFromConstants<GridNode>();
}
