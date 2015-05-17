#include "sceneslayerswap.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

history::scenes::LayerSwap::LayerSwap(sad::Scene* s1, sad::Scene* s2, int l1, int l2)
: m_s1(s1), m_s2(s2), m_list_position1(l1), m_list_position2(l2)
{

}

history::scenes::LayerSwap::~LayerSwap()
{

}

void history::scenes::LayerSwap::commit(core::Editor * ob)
{
	sad::Renderer::ref()->swapLayers(m_s1, m_s2);
	if (ob)
	{
		ob->panel()->setScenesInList(m_s1, m_s2, m_list_position2, m_list_position1);
	}
}

void history::scenes::LayerSwap::rollback(core::Editor * ob)
{
	sad::Renderer::ref()->swapLayers(m_s1, m_s2);
	if (ob)
	{
		ob->panel()->setScenesInList(m_s1, m_s2, m_list_position1, m_list_position2);
	}
}
