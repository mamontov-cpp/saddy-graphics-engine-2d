#include "instanceschangeway.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include <p2d/app/way.h>

Q_DECLARE_METATYPE(sad::p2d::app::Way*)

history::instances::ChangeWay::ChangeWay(
    sad::animations::Instance* d,
    unsigned long long oldvalue,
    unsigned long long newvalue

)
: history::instances::ChangeProperty<unsigned long long>(
      d,
      "way",
      oldvalue,
      newvalue
)
{
}

history::instances::ChangeWay::~ChangeWay()
{

}

void history::instances::ChangeWay::updateItem(core::Editor* e, unsigned long long value)
{
	int pos = 0;
	if (value > 0)
	{
		pos = e->panel()->findInComboBoxByMajorId<sad::p2d::app::Way*>(
			e->panel()->UI()->cmbWayAnimationInstanceWay,
			value
		);
		if (pos < 0)
		{
			pos = 0;
		}
	}

	invoke_blocked(
		e->panel()->UI()->cmbWayAnimationInstanceWay,
		&QComboBox::setCurrentIndex,
		pos
	);
}

void history::instances::ChangeWay::updateUI(core::Editor* e, const unsigned long long& value)
{
    e->emitClosure( bind(
            this,
            &history::instances::ChangeWay::updateItem,
            e,
			value
        )
    );
}
