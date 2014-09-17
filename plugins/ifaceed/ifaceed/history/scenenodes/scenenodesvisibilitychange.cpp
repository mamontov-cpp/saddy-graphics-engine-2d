#include "scenenodesvisibilitychange.h"

#include "../../mainpanel.h"

history::Command* history::scenenodes::visibilityChange(sad::SceneNode* d, bool oldvalue, bool newvalue)
{
	return new history::scenenodes::CheckboxReferencedProperyChange(
		&MainPanel::visibilityCheckbox,
		d,
        "visible",
		oldvalue,
		newvalue
	);	
}
