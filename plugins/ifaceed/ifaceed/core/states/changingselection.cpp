#include "../editor.h"

#include "../../objects/abstractscreenobject.h"
#include "../../objects/screentemplate.h"

#include "../../core/editorbehaviour.h"
#include "../../core/shared.h"

#include "selectedstate.h"

#include <algorithm>
#include <vector>

// Contains methods for changing selection in editor
void core::Editor::showObjectStats(AbstractScreenObject * o)
{
	if (o != NULL)
	{
		this->panel()->updateObjectStats(o);
	}
}



void core::Editor::trySelectObject(sad::Point2D p, bool enterSelected) 
{
	const std::vector<AbstractScreenObject*> & tbl = this->result()->fetchObjectsWithin(p);
	sad::log::Log * log = sad::log::Log::ref();
	if (tbl.empty() == false) {
		this->behaviourSharedData()->setSelectedObject(tbl[0]);
		this->showObjectStats(tbl[0]);
		std::vector<sad::String> chain;
        for(unsigned int i = 0; i < tbl.size(); i++)
		{
			chain.push_back(tbl[i]->prop<sad::String>("uid",log));
		}
		if (enterSelected) 
		{
			this->currentBehaviour()->enterState("selected");
		}
		if (chain.size() > 1) 
		{
			SelectedState * s = this->cbStateAs<SelectedState>("selected");
			s->enterNavigation(chain);
		}
	}
	this->result()->clearPickedCache();
}

