#include "groupactions.h"

#include "instanceactions.h"

#include "../blockedclosuremethodcall.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../history/groups/groupsnew.h"
#include "../history/groups/groupsremove.h"
#include "../history/groups/groupschangename.h"
#include "../history/groups/groupschangelooped.h"
#include "../history/groups/groupsaddinstance.h"
#include "../history/groups/groupsremoveinstance.h"

Q_DECLARE_METATYPE(sad::animations::Instance*)
Q_DECLARE_METATYPE(sad::animations::Group*)

// ===============================  PUBLIC METHODS ===============================

gui::GroupActions::GroupActions(QObject* parent) : QObject(parent), m_panel(NULL)
{
	m_animation = new gui::AnimationGroupProcess();
}

gui::GroupActions::~GroupActions()
{
	delete m_animation;	
}

void gui::GroupActions::setPanel(MainPanel* e)
{
	m_panel = e;
	m_animation->setEditor(e->editor());
}

MainPanel* gui::GroupActions::panel() const
{
	return m_panel;
}

// ===============================  PUBLIC SLOTS METHODS ===============================

void gui::GroupActions::addGroup()
{
	sad::animations::Group* g = new sad::animations::Group();
	g->setObjectName(m_panel->UI()->txtAnimationsGroupName->text().toStdString());
	g->setLooped(m_panel->UI()->cbAnimationsGroupLooped->checkState() == Qt::Checked);
	g->setTable(sad::Renderer::ref()->database("")->table("animationgroups"));

	QListWidget* w = m_panel->UI()->lstAnimationsGroupInGroup;
	for(size_t  i = 0; i < w->count(); i++)
	{
		QVariant v = w->item(i)->data(Qt::UserRole);

		sad::animations::Instance* instance = v.value<sad::animations::Instance*>();
		g->addAsLink(instance);
	}

	sad::Renderer::ref()->database("")->table("animationgroups")->add(g);

	history::groups::New* c  = new history::groups::New(g);
	c->commit(m_panel->editor());

	m_panel->UI()->lstAnimationsGroup->setCurrentRow(m_panel->UI()->lstAnimationsGroup->count() - 1);
	m_panel->editor()->history()->add(c);
}

void gui::GroupActions::currentGroupChanged(int newrow)
{
	if (newrow > -1)
	{
		sad::animations::Group* g = m_panel->UI()->lstAnimationsGroup->item(newrow)->data(Qt::UserRole).value<sad::animations::Group*>();
		m_panel->editor()->shared()->setSelectedGroup(g);

		invoke_blocked(
			m_panel->UI()->txtAnimationsGroupName,
			&QLineEdit::setText,
			QString(g->objectName().c_str())
		);

		invoke_blocked(
			m_panel->UI()->cbAnimationsGroupLooped,
			&QCheckBox::setCheckState,
			(g->looped() ? Qt::Checked : Qt::Unchecked)
		);
	}
	else
	{
		m_panel->editor()->shared()->setSelectedGroup(NULL);
	}

	m_panel->instanceActions()->updateGroupInstanceList();
}

void gui::GroupActions::removeGroup()
{
	int row = m_panel->UI()->lstAnimationsGroup->currentRow();
	if (row > -1)
	{
		sad::animations::Group* g = m_panel->UI()->lstAnimationsGroup->item(row)->data(Qt::UserRole).value<sad::animations::Group*>();
	
		history::groups::Remove* c  = new history::groups::Remove(g, row);
		c->commit(m_panel->editor());

		m_panel->editor()->history()->add(c);
	}
}

void gui::GroupActions::nameChanged(const QString& name)
{
	int row = m_panel->UI()->lstAnimationsGroup->currentRow();
	if (row > -1)
	{
		sad::animations::Group* g = m_panel->UI()->lstAnimationsGroup->item(row)->data(Qt::UserRole).value<sad::animations::Group*>();
	
		sad::String oldvalue = g->objectName();
		sad::String newvalue = name.toStdString();
		
		if (oldvalue != newvalue)
		{
			history::groups::ChangeName* c = new history::groups::ChangeName(g, row, oldvalue, newvalue);
			c->commit(m_panel->editor());
			m_panel->editor()->history()->add(c);
		}
	}
}


void gui::GroupActions::loopedChanged(bool newvalue)
{
	int row = m_panel->UI()->lstAnimationsGroup->currentRow();
	if (row > -1)
	{
		sad::animations::Group* g = m_panel->UI()->lstAnimationsGroup->item(row)->data(Qt::UserRole).value<sad::animations::Group*>();
		bool oldvalue =g->looped();
		if (oldvalue != newvalue)
		{
			history::groups::ChangeLooped* c = new history::groups::ChangeLooped(g, oldvalue, newvalue);
			c->commit(this->m_panel->editor());

			this->m_panel->editor()->history()->add(c);
		}
	}
}


void gui::GroupActions::addInstance()
{
	int row = m_panel->UI()->lstAnimationsGroup->currentRow();
	int irow = m_panel->UI()->lstAnimationsGroupAllAnimations->currentRow();
	if (row > -1 && irow > -1)
	{
		sad::animations::Group* g = m_panel->UI()->lstAnimationsGroup->item(row)->data(Qt::UserRole).value<sad::animations::Group*>();

		sad::animations::Instance* i = m_panel->UI()->lstAnimationsGroupAllAnimations->item(row)->data(Qt::UserRole).value<sad::animations::Instance*>();

		history::groups::AddInstance* c = new history::groups::AddInstance(g, i, irow);
		c->commit(this->m_panel->editor());

		this->m_panel->editor()->history()->add(c);
	}
}


void gui::GroupActions::removeInstance()
{
	int row = m_panel->UI()->lstAnimationsGroup->currentRow();
	int irow = m_panel->UI()->lstAnimationsGroupInGroup->currentRow();
	if (row > -1 && irow > -1)
	{
		sad::animations::Group* g = m_panel->UI()->lstAnimationsGroup->item(row)->data(Qt::UserRole).value<sad::animations::Group*>();

		sad::animations::Instance* i = m_panel->UI()->lstAnimationsGroupInGroup->item(row)->data(Qt::UserRole).value<sad::animations::Instance*>();

		history::groups::RemoveInstance* c = new history::groups::RemoveInstance(g, i, irow);
		c->commit(this->m_panel->editor());

		this->m_panel->editor()->history()->add(c);
	}
}


void gui::GroupActions::start()
{
	core::Shared* s = m_panel->editor()->shared();
	if (s->isAnyKindOfAnimationIsRunning() == false 
		&& s->selectedGroup() != NULL)
	{
		s->selectedGroup()->restart(sad::Renderer::ref()->animations());
		m_animation->setEditor(m_panel->editor());
		m_animation->start(s->selectedGroup() );
	}
}

void gui::GroupActions::stop()
{
	core::Shared* s = m_panel->editor()->shared();
	if (s->isAnyKindOfAnimationIsRunning() == true)
	{
		m_animation->setEditor(m_panel->editor());
		m_animation->stop();
	}
}
