#include "instanceschangeobject.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

Q_DECLARE_METATYPE(sad::db::Object*) //-V566

history::instances::ChangeObject::ChangeObject(
    sad::animations::Instance* d,
    unsigned long long oldvalue,
    unsigned long long newvalue

)
: history::instances::ChangeProperty<unsigned long long>(
      d,
      "object",
      oldvalue,
      newvalue
)
{
}

history::instances::ChangeObject::~ChangeObject()
{

}

void history::instances::ChangeObject::updateItem(core::Editor* e, unsigned long long value)
{
	int pos = 0;
	if (value > 0)
	{
		pos = e->panel()->findInComboBoxByMajorId<sad::db::Object*>(
			e->panel()->UI()->cmbAnimationInstanceObject,
			value
		);
		if (pos < 0)
		{
			pos = 0;
		}
	}

	invoke_blocked(
		e->panel()->UI()->cmbAnimationInstanceObject,
		&QComboBox::setCurrentIndex,
		pos
	);
}

void history::instances::ChangeObject::updateUI(core::Editor* e, const unsigned long long& value)
{
    e->emitClosure( bind(
            this,
            &history::instances::ChangeObject::updateItem,
            e,
			value
        )
    );
}
