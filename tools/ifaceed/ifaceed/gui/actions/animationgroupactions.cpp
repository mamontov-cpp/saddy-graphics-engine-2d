#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>

#include "actions.h"
#include "animationinstanceactions.h"
#include "animationgroupactions.h"


#include <db/dbdatabase.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../blockedclosuremethodcall.h"
#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../history/groups/groupsnew.h"
#include "../../history/groups/groupsremove.h"
#include "../../history/groups/groupschangename.h"
#include "../../history/groups/groupschangelooped.h"
#include "../../history/groups/groupschangesequential.h"
#include "../../history/groups/groupsaddinstance.h"
#include "../../history/groups/groupsremoveinstance.h"

#include "../uiblocks/uiblocks.h"
#include "../uiblocks/uianimationsgroupblock.h"


Q_DECLARE_METATYPE(sad::animations::Instance*) //-V566
Q_DECLARE_METATYPE(sad::animations::Group*) //-V566

// ===============================  PUBLIC METHODS ===============================

gui::actions::AnimationGroupActions::AnimationGroupActions(QObject* parent) : QObject(parent)
{
    m_animation = new gui::AnimationGroupProcess();
}

gui::actions::AnimationGroupActions::~AnimationGroupActions()
{
    delete m_animation;	
}

void gui::actions::AnimationGroupActions::setEditor(core::Editor* e)
{
    this->gui::actions::AbstractActions::setEditor(e);
    m_animation->setEditor(e);
}


// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::removeFromDatabase(
    sad::animations::Group* g,
    bool from_editor,
    int row
)
{
    gui::uiblocks::UIAnimationsGroupBlock* blk = m_editor->uiBlocks()->uiAnimationsGroupBlock();
    if (row == -1)
    {
        row = this->findInList<sad::animations::Group*>(blk->lstAnimationsGroup, g);
    }
    if (row > - 1)
    {
        history::groups::Remove* c  = new history::groups::Remove(g, row);
        c->commit(m_editor);

        if (from_editor)
        {
            m_editor->history()->add(c);
        }
        else
        {
            m_editor->currentBatchCommand()->add(c);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::addGroupToList(sad::animations::Group* g)
{
    gui::uiblocks::UIAnimationsGroupBlock* blk = m_editor->uiBlocks()->uiAnimationsGroupBlock();
    QListWidgetItem* item = new QListWidgetItem(this->nameForGroup(g));
    
    QVariant v;
    v.setValue(g);
    item->setData(Qt::UserRole, v);
    
    blk->lstAnimationsGroup->addItem(item);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::removeLastGroupFromList()
{
    gui::uiblocks::UIAnimationsGroupBlock* blk = m_editor->uiBlocks()->uiAnimationsGroupBlock();
    if (blk->lstAnimationsGroup->count())
    {
        delete blk->lstAnimationsGroup->takeItem(blk->lstAnimationsGroup->count() - 1);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::insertGroupToList(int pos, sad::animations::Group* g)
{
    gui::uiblocks::UIAnimationsGroupBlock* blk = m_editor->uiBlocks()->uiAnimationsGroupBlock();
    QListWidgetItem* item = new QListWidgetItem(this->nameForGroup(g));
    
    QVariant v;
    v.setValue(g);
    item->setData(Qt::UserRole, v);
    
    blk->lstAnimationsGroup->insertItem(pos, item);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::removeGroupFromList(int pos)
{
    gui::uiblocks::UIAnimationsGroupBlock* blk = m_editor->uiBlocks()->uiAnimationsGroupBlock();
    delete blk->lstAnimationsGroup->takeItem(pos);
}

QString gui::actions::AnimationGroupActions::nameForGroup(sad::animations::Group* g) const
{
    return const_cast<gui::actions::AnimationGroupActions*>(this)->viewableObjectName(g);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::changedIsSequential(bool sequential)
{
    gui::uiblocks::UIAnimationsGroupBlock* blk = m_editor->uiBlocks()->uiAnimationsGroupBlock();
    
    int row = blk->lstAnimationsGroup->currentRow();
    if (row > -1)
    {
        sad::animations::Group* g = blk->lstAnimationsGroup->item(row)->data(Qt::UserRole).value<sad::animations::Group*>();
    
        bool old_value = g->isSequential();
        bool new_value = sequential;
        
        if (old_value != new_value)
        {
            history::groups::ChangeSequential* c = new history::groups::ChangeSequential(g, old_value, new_value);
            c->commitWithoutUpdatingUI(m_editor);
            m_editor->history()->add(c);
        }
    }
}

// ===============================  PUBLIC SLOTS METHODS ===============================

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::addGroup()
{
    gui::uiblocks::UIAnimationsGroupBlock* blk = m_editor->uiBlocks()->uiAnimationsGroupBlock();

    sad::animations::Group* g = new sad::animations::Group();
    g->setObjectName(Q2STDSTRING(blk->txtAnimationsGroupName->text()));
    g->setLooped(blk->cbAnimationsGroupLooped->checkState() == Qt::Checked);
    g->toggleIsSequential(blk->rbAnimationsGroupSequential->isChecked());
    g->setTable(sad::Renderer::ref()->database("")->table("animationgroups"));

    QListWidget* w = blk->lstAnimationsGroupInGroup;
    for(size_t  i = 0; i < w->count(); i++)
    {
        QVariant v = w->item(i)->data(Qt::UserRole);

        sad::animations::Instance* instance = v.value<sad::animations::Instance*>();
        g->addAsLink(instance);
    }

    sad::Renderer::ref()->database("")->table("animationgroups")->add(g);

    history::groups::New* c  = new history::groups::New(g);
    c->commit(m_editor);

    blk->lstAnimationsGroup->setCurrentRow(blk->lstAnimationsGroup->count() - 1);
    m_editor->history()->add(c);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::currentGroupChanged(int new_row)
{
    if (new_row > -1)
    {
        gui::uiblocks::UIAnimationsGroupBlock* blk = m_editor->uiBlocks()->uiAnimationsGroupBlock();

        sad::animations::Group* g = blk->lstAnimationsGroup->item(new_row)->data(Qt::UserRole).value<sad::animations::Group*>();
        m_editor->shared()->setSelectedGroup(g);

        invoke_blocked(
            blk->rbAnimationsGroupSequential,
            &QRadioButton::setChecked,
            g->isSequential()
        );

        invoke_blocked(
            blk->rbAnimationsGroupParallel,
            &QRadioButton::setChecked,
            g->isSequential() == false
        );

        invoke_blocked(
            blk->txtAnimationsGroupName,
            &QLineEdit::setText,
            STD2QSTRING(g->objectName().c_str())
        );

        invoke_blocked(
            blk->cbAnimationsGroupLooped,
            &QCheckBox::setCheckState,
            (g->looped() ? Qt::Checked : Qt::Unchecked)
        );
    }
    else
    {
        m_editor->shared()->setSelectedGroup(nullptr);
    }

    m_editor->actions()->instanceActions()->updateGroupInstanceList();
}

void gui::actions::AnimationGroupActions::removeGroup()
{
    gui::uiblocks::UIAnimationsGroupBlock* blk = m_editor->uiBlocks()->uiAnimationsGroupBlock();
    int row = blk->lstAnimationsGroup->currentRow();
    if (row > -1)
    {
        sad::animations::Group* g = blk->lstAnimationsGroup->item(row)->data(Qt::UserRole).value<sad::animations::Group*>();
    
        removeFromDatabase(g, true, row);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::nameChanged(const QString& name)
{
    gui::uiblocks::UIAnimationsGroupBlock* blk = m_editor->uiBlocks()->uiAnimationsGroupBlock();
    
    int row = blk->lstAnimationsGroup->currentRow();
    if (row > -1)
    {
        sad::animations::Group* g = blk->lstAnimationsGroup->item(row)->data(Qt::UserRole).value<sad::animations::Group*>();
    
        sad::String old_value = g->objectName();
        sad::String new_value = Q2STDSTRING(name);
        
        if (old_value != new_value)
        {
            history::groups::ChangeName* c = new history::groups::ChangeName(g, row, old_value, new_value);
            c->commitWithoutUpdatingUI(m_editor);
            m_editor->history()->add(c);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::loopedChanged(bool new_value)
{
    gui::uiblocks::UIAnimationsGroupBlock* blk = m_editor->uiBlocks()->uiAnimationsGroupBlock();
    int row = blk->lstAnimationsGroup->currentRow();
    if (row > -1)
    {
        sad::animations::Group* g = blk->lstAnimationsGroup->item(row)->data(Qt::UserRole).value<sad::animations::Group*>();
        bool old_value =g->looped();
        if (old_value != new_value)
        {
            history::groups::ChangeLooped* c = new history::groups::ChangeLooped(g, old_value, new_value);
            c->commitWithoutUpdatingUI(this->m_editor);

            this->m_editor->history()->add(c);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::addInstance()
{
    gui::uiblocks::UIAnimationsGroupBlock* blk = m_editor->uiBlocks()->uiAnimationsGroupBlock();
    int row = blk->lstAnimationsGroup->currentRow();
    int irow = blk->lstAnimationsGroupAllAnimations->currentRow();
    if (row > -1 && irow > -1)
    {
        sad::animations::Group* g = blk->lstAnimationsGroup->item(row)->data(Qt::UserRole).value<sad::animations::Group*>();

        sad::animations::Instance* i = blk->lstAnimationsGroupAllAnimations->item(irow)->data(Qt::UserRole).value<sad::animations::Instance*>();

        history::groups::AddInstance* c = new history::groups::AddInstance(g, i, irow);
        c->commit(this->m_editor);

        this->m_editor->history()->add(c);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::removeInstance()
{
    gui::uiblocks::UIAnimationsGroupBlock* blk = m_editor->uiBlocks()->uiAnimationsGroupBlock();
    int row = blk->lstAnimationsGroup->currentRow();
    int irow = blk->lstAnimationsGroupInGroup->currentRow();
    if (row > -1 && irow > -1)
    {
        sad::animations::Group* g = blk->lstAnimationsGroup->item(row)->data(Qt::UserRole).value<sad::animations::Group*>();

        sad::animations::Instance* i = blk->lstAnimationsGroupInGroup->item(irow)->data(Qt::UserRole).value<sad::animations::Instance*>();

        history::groups::RemoveInstance* c = new history::groups::RemoveInstance(g, i, irow);
        c->commit(this->m_editor);

        this->m_editor->history()->add(c);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::start()
{
    core::Shared* s = m_editor->shared();
    if (s->isAnyKindOfAnimationIsRunning() == false 
        && s->selectedGroup() != nullptr)
    {
        s->selectedGroup()->restart(sad::Renderer::ref()->animations());
        m_animation->setEditor(m_editor);
        m_animation->start(s->selectedGroup() );
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationGroupActions::stop()
{
    core::Shared* s = m_editor->shared();
    if (s->isAnyKindOfAnimationIsRunning() == true)
    {
        m_animation->setEditor(m_editor);
        m_animation->stop();
    }
}

void gui::actions::AnimationGroupActions::toggledSequential(bool toggled)
{
    if (toggled)
    {
        changedIsSequential(true);
    }
}

void gui::actions::AnimationGroupActions::toggledParallel(bool toggled)
{
    if (toggled)
    {
        changedIsSequential(false);
    }
}