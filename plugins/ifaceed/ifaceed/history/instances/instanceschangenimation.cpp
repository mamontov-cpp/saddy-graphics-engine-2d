#include "instanceschangeanimation.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

Q_DECLARE_METATYPE(sad::animations::Instance*) //-V566
Q_DECLARE_METATYPE(sad::animations::Animation*) //-V566

history::instances::ChangeAnimation::ChangeAnimation(
	sad::animations::Instance* d,
	QRadioButton* button,
	QRadioButton* oldbutton,
	const sad::String& oldname,
	unsigned long long oldid,
	const sad::String& newname,
	unsigned long long newid
) : m_node(d), m_button(button), m_oldbutton(oldbutton)
{
	m_node->addRef();

	m_maybe_old_name.setValue(oldname);
	m_maybe_old_id.setValue(oldid);

	m_maybe_new_name.setValue(newname);
	m_maybe_new_id.setValue(newid);
}

history::instances::ChangeAnimation::ChangeAnimation(
	sad::animations::Instance* d,
	const sad::String& oldname,
	const sad::String& newname
) : m_node(d), m_button(NULL), m_oldbutton(NULL)
{
	m_node->addRef();

	m_maybe_old_name.setValue(oldname);

	m_maybe_new_name.setValue(newname);
}

history::instances::ChangeAnimation::ChangeAnimation(
	sad::animations::Instance* d,
	unsigned long long oldid,
	unsigned long long newid
): m_node(d), m_button(NULL), m_oldbutton(NULL)
{
	m_node->addRef();

	m_maybe_old_id.setValue(oldid);

	m_maybe_new_id.setValue(newid);
}

history::instances::ChangeAnimation::~ChangeAnimation()
{
	m_node->delRef();
}

void history::instances::ChangeAnimation::commit(core::Editor * ob)
{
	if (m_maybe_new_name.exists())
	{
		m_node->setProperty("animation", m_maybe_new_name.value());
	}

	if (m_maybe_new_id.exists())
	{
		m_node->setProperty("animationmajorid", m_maybe_new_id.value());
	}

	if (ob)
	{
		if (m_node == ob->shared()->selectedInstance())
		{
			if (m_button)
			{
				ob->emitClosure(blocked_bind(
					m_button,
					&QRadioButton::setChecked,
					true
				));
			}

			if (m_maybe_new_name.exists())
			{
				ob->emitClosure(bind(
					this,
					&history::instances::ChangeAnimation::updateTreeComboValue,
					ob,
					m_maybe_new_name.value()
				));

				if (m_button == NULL && m_maybe_new_name.value().length() > 0)
				{
					ob->emitClosure(blocked_bind(
						ob->panel()->UI()->rbAnimationInstanceFromTree,
						&QRadioButton::setChecked,
						true
					));
				}
			}

			if (m_maybe_new_id.exists())
			{
				ob->emitClosure(bind(
					this,
					&history::instances::ChangeAnimation::updateDatabaseComboValue,
					ob,
					m_maybe_new_id.value()
				));

				if (m_button == NULL && m_maybe_new_id.value() > 0)
				{
					ob->emitClosure(blocked_bind(
						ob->panel()->UI()->rbAnimationInstanceFromDatabase,
						&QRadioButton::setChecked,
						true
					));
				}
			}
		}
	}
}

void history::instances::ChangeAnimation::rollback(core::Editor * ob)
{
	if (m_maybe_old_name.exists())
	{
		m_node->setProperty("animation", m_maybe_old_name.value());
	}

	if (m_maybe_old_id.exists())
	{
		m_node->setProperty("animationmajorid", m_maybe_old_id.value());
	}

	if (ob)
	{
		if (m_node == ob->shared()->selectedInstance())
		{
			if (m_oldbutton)
			{
				ob->emitClosure(blocked_bind(
					m_oldbutton,
					&QRadioButton::setChecked,
					true
				));
			}

			if (m_maybe_old_name.exists())
			{
				ob->emitClosure(bind(
					this,
					&history::instances::ChangeAnimation::updateTreeComboValue,
					ob,
					m_maybe_old_name.value()
				));

				if (m_button == NULL && m_maybe_old_name.value().length() > 0)
				{
					ob->emitClosure(blocked_bind(
						ob->panel()->UI()->rbAnimationInstanceFromTree,
						&QRadioButton::setChecked,
						true
					));
				}
			}

			if (m_maybe_old_id.exists())
			{
				ob->emitClosure(bind(
					this,
					&history::instances::ChangeAnimation::updateDatabaseComboValue,
					ob,
					m_maybe_old_id.value()
				));

				if (m_button == NULL && m_maybe_old_id.value() > 0)
				{
					ob->emitClosure(blocked_bind(
						ob->panel()->UI()->rbAnimationInstanceFromDatabase,
						&QRadioButton::setChecked,
						true
					));
				}
			}
		}
	}
}


void history::instances::ChangeAnimation::updateTreeComboValue(core::Editor* e, const sad::String& value)
{
	if (value.size() == 0)
	{
		invoke_blocked(
			e->panel()->UI()->cmbAnimationInstanceAnimationFromTree,
			&QComboBox::setCurrentIndex,
			0
		);
	}
	else
	{
		QComboBox* box = e->panel()->UI()->cmbAnimationInstanceAnimationFromTree;
		int pos = -1;
		QString stringttosearch = STD2QSTRING(value);
		for(size_t i = 1; (i < box->count()) && pos == -1; i++)
		{
			if (box->itemText(i) == stringttosearch)
			{
				pos = i;
			}
		}
		if (pos < 0)
		{
			pos = 0;
		}

		invoke_blocked(
			e->panel()->UI()->cmbAnimationInstanceAnimationFromTree,
			&QComboBox::setCurrentIndex,
			pos
		);
	}
}

void history::instances::ChangeAnimation::updateDatabaseComboValue(core::Editor* e, unsigned long long value)
{
	if (value == 0)
	{
		invoke_blocked(
			e->panel()->UI()->cmbAnimationInstanceAnimationFromDatabase,
			&QComboBox::setCurrentIndex,
			0
		);
	}
	else
	{
		int pos = e->panel()->findInComboBoxByMajorId<sad::animations::Animation*>(
			e->panel()->UI()->cmbAnimationInstanceAnimationFromDatabase, 
			value
		);
		if (pos < 0)
		{
			pos = 0;
		}
		invoke_blocked(
			e->panel()->UI()->cmbAnimationInstanceAnimationFromDatabase,
			&QComboBox::setCurrentIndex,
			pos
		);
	}
}
