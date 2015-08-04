#include "animationschangewaymovingway.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../closuremethodcall.h"
#include "../../blockedclosuremethodcall.h"

Q_DECLARE_METATYPE(sad::p2d::app::Way*) //-V566

history::animations::ChangeWayMovingWay::ChangeWayMovingWay(
    sad::animations::Animation* d,
    unsigned long long oldvalue,
    unsigned long long newvalue

)
: history::animations::ChangeProperty<unsigned long long>(
      d,
      "way",
      oldvalue,
      newvalue
)
{
}

history::animations::ChangeWayMovingWay::~ChangeWayMovingWay()
{

}

void history::animations::ChangeWayMovingWay::performUpdateUI(core::Editor* e, unsigned long long value)
{
	MainPanel* panel = e->panel();
	QComboBox* list = panel->UI()->cmbWayAnimationWay;
	invoke_blocked(list, &QComboBox::setCurrentIndex,  panel->findInComboBoxByMajorId<sad::p2d::app::Way*>(list,value));
}

void history::animations::ChangeWayMovingWay::updateUI(core::Editor* e, const unsigned long long& value)
{
    e->emitClosure( bind(
            this,
            &history::animations::ChangeWayMovingWay::performUpdateUI,
            e,
			value
        )
    );
}
