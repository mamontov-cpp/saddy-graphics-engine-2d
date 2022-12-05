#include <QDoubleSpinBox>
#include <QLineEdit>

#include "animationinstanceactions.h"

#include <db/dbdatabase.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../blockedclosuremethodcall.h"
#include "../../qstdstring.h"


#include "../core/editor.h"

#include "../../history/instances/instancesnew.h"
#include "../../history/instances/instancesremove.h"
#include "../../history/instances/instanceschangename.h"
#include "../../history/instances/instanceschangeanimation.h"
#include "../../history/instances/instanceschangeobject.h"
#include "../../history/instances/instanceschangestarttime.h"
#include "../../history/instances/instanceschangeway.h"

#include "../uiblocks/uiblocks.h"
#include "../uiblocks/uianimationinstanceblock.h"
#include "../uiblocks/uianimationsgroupblock.h"

Q_DECLARE_METATYPE(sad::animations::Animation*) //-V566
Q_DECLARE_METATYPE(sad::animations::Instance*) //-V566
Q_DECLARE_METATYPE(sad::p2d::app::Way*) //-V566
Q_DECLARE_METATYPE(sad::db::Object*) //-V566

// ===============================  PUBLIC METHODS ===============================

gui::actions::AnimationInstanceActions::AnimationInstanceActions(QObject* parent) : QObject(parent), m_blk(nullptr)
{
    m_animation = new gui::AnimationInstanceProcess();
}

gui::actions::AnimationInstanceActions::~AnimationInstanceActions()
{
    delete m_animation;	
}


void gui::actions::AnimationInstanceActions::setEditor(core::Editor* e)
{
    this->gui::actions::AbstractActions::setEditor(e);
    m_animation->setEditor(e);
    m_blk = m_editor->uiBlocks()->uiAnimationInstanceBlock();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationInstanceActions::updateGroupInstanceList()
{
    sad::Vector<unsigned long long> ids;
    if (m_editor->shared()->selectedGroup())
    {
        ids = m_editor->shared()->selectedGroup()->instances();
    }
    gui::uiblocks::UIAnimationsGroupBlock* gblk = m_editor->uiBlocks()->uiAnimationsGroupBlock(); 

    QListWidget* list = m_blk->lstAnimationInstances;
    QListWidget* destlist = gblk->lstAnimationsGroupAllAnimations;
    int allrow = destlist->currentRow();
    destlist->clear();
    for(int i = 0; i < list->count(); i++)
    {
        QString name = list->item(i)->text();
        sad::animations::Instance* instance = list->item(i)->data(Qt::UserRole).value<sad::animations::Instance*>();

        if (std::find(ids.begin(), ids.end(), instance->MajorId) == ids.end())
        {
            QListWidgetItem* item = new QListWidgetItem(name);

            QVariant v;
            v.setValue(instance);

            item->setData(Qt::UserRole, v);
            destlist->addItem(item);
        }
    }
    destlist = gblk->lstAnimationsGroupInGroup;
    destlist->clear();
    int grouprow = destlist->currentRow();
    for(size_t  i = 0; i < ids.size(); i++)
    {
        sad::db::Object* o = sad::Renderer::ref()->database("")->queryByMajorId(ids[i]);
        if (o)
        {
            if (o->isInstanceOf("sad::animations::Instance") || o->isInstanceOf("sad::animations::WayInstance"))
            {
                sad::animations::Instance* instance = static_cast<sad::animations::Instance*>(o);

                QString name = this->nameForInstance(instance);

                QVariant v;
                v.setValue(instance);

                QListWidgetItem* item = new QListWidgetItem(name);

                item->setData(Qt::UserRole, v);
                destlist->addItem(item);
            }
        }
    }

    void (QListWidget::*f)(int) = &QListWidget::setCurrentRow;
    destlist = gblk->lstAnimationsGroupAllAnimations;
    if (allrow >= 0 && destlist->count() > 0)
    {
        if (allrow >= destlist->count())
        {
            allrow = destlist->count() - 1;
        }
        invoke_blocked(destlist, f, allrow);
    }

    destlist = gblk->lstAnimationsGroupInGroup;
    if (grouprow >= 0 && destlist->count() > 0)
    {
        if (grouprow >= destlist->count())
        {
            grouprow = destlist->count() - 1;
        }
        invoke_blocked(destlist, f, grouprow);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationInstanceActions::updateCurrentInstanceAnimation(sad::animations::Instance* a)
{
    core::Editor* e =  m_editor;
    // Try set animation from tree
    sad::String animationname = a->getProperty<sad::String>("animation").value();
    if (animationname.length())
    {
        e->emitClosure( blocked_bind(m_blk->rbAnimationInstanceFromTree, &QRadioButton::setChecked, true));

        QString value = animationname.c_str();
        int pos = -1;
        for (int i = 1; i < m_blk->cmbAnimationInstanceAnimationFromTree->count() && pos == -1; i++)
        {
            if (value == m_blk->cmbAnimationInstanceAnimationFromTree->itemText(i))
            {
                pos = i;
            }
        }
        if (pos != -1)
        {
            e->emitClosure( blocked_bind(m_blk->cmbAnimationInstanceAnimationFromTree, &QComboBox::setCurrentIndex, pos) );
        }
        else
        {
            e->emitClosure( blocked_bind(m_blk->cmbAnimationInstanceAnimationFromTree, &QComboBox::setCurrentIndex, 0) );
        }
    }
    else
    {
        e->emitClosure( blocked_bind(m_blk->cmbAnimationInstanceAnimationFromTree, &QComboBox::setCurrentIndex, 0) );
    }

    // Try set animation from database
    unsigned long long animationid =  a->getProperty<unsigned long long>("animationmajorid").value();
    if (animationid > 0)
    {
        e->emitClosure( blocked_bind(m_blk->rbAnimationInstanceFromDatabase, &QRadioButton::setChecked, true));
        int pos = this->findInComboBoxByMajorId<sad::animations::Animation*>(m_blk->cmbAnimationInstanceAnimationFromDatabase, animationid);
        if (pos > 0)
        {
            e->emitClosure( blocked_bind(m_blk->cmbAnimationInstanceAnimationFromDatabase, &QComboBox::setCurrentIndex, pos) );
        }
        else
        {
            e->emitClosure( blocked_bind(m_blk->cmbAnimationInstanceAnimationFromDatabase, &QComboBox::setCurrentIndex, 0) );
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationInstanceActions::removeInstanceFromDatabase(
    sad::animations::Instance* a,
    bool from_editor,
    int row
)
{
    if (row == -1)
    {
        row = this->findInList<sad::animations::Instance*>( m_blk->lstAnimationInstances, a);
    }

    if (row != -1)
    {
        sad::Vector<sad::db::Object*> objects;
        sad::Vector<sad::Pair<sad::animations::Group*, int> > dependentgroups;

        sad::Renderer::ref()->database("")->table("animationgroups")->objects(objects);
        for(size_t i = 0; i < objects.size(); i++)
        {
            sad::db::Object* tmp = objects[i];
            if (tmp->isInstanceOf("sad::animations::Group"))
            {
                sad::animations::Group* group = static_cast<sad::animations::Group*>(tmp);
                int pos = group->findInstance(a->MajorId);
                if (pos != -1)
                {
                    dependentgroups << sad::Pair<sad::animations::Group*, int>(group, pos);
                }
            }
        }

        history::instances::Remove* c  = new history::instances::Remove(a, row, dependentgroups);
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

QString gui::actions::AnimationInstanceActions::nameForInstance(sad::animations::Instance* i) const
{
    QString result = "[I] ";
    if (i)
    {
        if (i->isInstanceOf("sad::animations::WayInstance"))
        {
            result = "[WI] ";
        }
        result += const_cast<gui::actions::AnimationInstanceActions*>(this)->viewableObjectName(i); 
    }
    return result;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationInstanceActions::addInstance()
{
    if (m_blk->rbAnimationInstanceFromDatabase->isChecked()
        || m_blk->rbAnimationInstanceFromTree->isChecked())
    {
        sad::animations::Instance* instance = nullptr;
        // An default instance
        if (m_blk->cmbAddedAnimationInstanceType->currentIndex() == 0)
        {
            instance = new sad::animations::Instance();
            instance->setTreeName(sad::Renderer::ref(), "");

            // Set animation from tree
            if (m_blk->rbAnimationInstanceFromTree->isChecked())
            {
                if (m_blk->cmbAnimationInstanceAnimationFromTree->currentIndex() > 0)
                { 
                    QString value = m_blk->cmbAnimationInstanceAnimationFromTree->currentText();
                    assert( instance->setProperty("animation", value) );
                }
                else
                {
                    instance->setProperty("animation", sad::String(""));
                }
            }


            // Set animation from database
            if (m_blk->rbAnimationInstanceFromDatabase->isChecked())
            {
                if (m_blk->cmbAnimationInstanceAnimationFromDatabase->currentIndex() > 0)
                {
                    QVariant v = m_blk->cmbAnimationInstanceAnimationFromDatabase->itemData( m_blk->cmbAnimationInstanceAnimationFromDatabase->currentIndex(), Qt::UserRole);
                    sad::animations::Animation* a = v.value<sad::animations::Animation*>();
                    instance->setProperty("animationmajorid", a->MajorId);
                }
                else
                {
                    instance->setProperty("animationmajorid", 0ull);
                }
            }

        }

        // A way instance
        if (m_blk->cmbAddedAnimationInstanceType->currentIndex() == 1)
        {
            instance = new sad::animations::WayInstance();

            // Set way from database
            if (m_blk->cmbWayAnimationInstanceWay->currentIndex() > 0)
            {
                QVariant v = m_blk->cmbWayAnimationInstanceWay->itemData(m_blk->cmbWayAnimationInstanceWay->currentIndex(), Qt::UserRole);
                sad::p2d::app::Way* a = v.value<sad::p2d::app::Way*>();
                instance->setProperty("way", a->MajorId);
            }
            else
            {
                instance->setProperty("way", 0ull);
            }
        }

        // An object
        if (m_blk->cmbAnimationInstanceObject->currentIndex() > 0)
        {
            QVariant v = m_blk->cmbAnimationInstanceObject->itemData(m_blk->cmbAnimationInstanceObject->currentIndex(), Qt::UserRole);
            sad::db::Object* a = v.value<sad::db::Object*>();
            instance->setProperty("object", a->MajorId);
        }
        else
        {
            instance->setProperty("object", 0ull);
        }

        // A start time
        instance->setProperty("starttime", m_blk->dsbAnimationInstanceStartTime->value());
        instance->setObjectName(Q2STDSTRING(m_blk->txtAnimationInstanceName->text()));

        sad::Renderer::ref()->database("")->table("animationinstances")->add(instance);

        history::instances::New* c  = new history::instances::New(instance);
        c->commit(m_editor);

        m_blk->lstAnimationInstances->setCurrentRow(m_blk->lstAnimationInstances->count() - 1);
        m_editor->history()->add(c);
    }
}

void gui::actions::AnimationInstanceActions::removeInstance()
{
    int row = m_blk->lstAnimationInstances->currentRow();
    if (row > -1)
    {
        QVariant v = m_blk->lstAnimationInstances->item(row)->data(Qt::UserRole);
        sad::animations::Instance* a = v.value<sad::animations::Instance*>();
        removeInstanceFromDatabase(a, true, row);
    }
}

void gui::actions::AnimationInstanceActions::currentInstanceChanged(int row)
{
    core::Editor* e = m_editor;
    if (row > -1)
    {
        QVariant v = m_blk->lstAnimationInstances->item(row)->data(Qt::UserRole);
        sad::animations::Instance* a = v.value<sad::animations::Instance*>();
        e->shared()->setSelectedInstance(a);

        if (a->isInstanceOf("sad::animations::Instance"))
        {
            this->updateCurrentInstanceAnimation(a);
        }

        if (a->isInstanceOf("sad::animations::WayInstance"))
        {
            unsigned long long wayid = a->getProperty<unsigned long long>("way").value();
            int pos = this->findInComboBoxByMajorId<sad::p2d::app::Way*>(m_blk->cmbWayAnimationInstanceWay, wayid);
            if (pos > -1)
            {
                e->emitClosure( blocked_bind(m_blk->cmbWayAnimationInstanceWay, &QComboBox::setCurrentIndex, pos) );
            }
            else
            {
                e->emitClosure( blocked_bind(m_blk->cmbWayAnimationInstanceWay, &QComboBox::setCurrentIndex, 0) );
            }
        }

        // Object id
        unsigned long long objectid = a->getProperty<unsigned long long>("object").value();
        int pos = this->findInComboBoxByMajorId<sad::db::Object*>(m_blk->cmbAnimationInstanceObject, objectid);
        if (pos > -1)
        {
            e->emitClosure( blocked_bind(m_blk->cmbAnimationInstanceObject, &QComboBox::setCurrentIndex, pos) );
        }
        else
        {
            e->emitClosure( blocked_bind(m_blk->cmbAnimationInstanceObject, &QComboBox::setCurrentIndex, 0) );
        }

        double starttime =  a->getProperty<double>("starttime").value();
        e->emitClosure( blocked_bind(m_blk->dsbAnimationInstanceStartTime, &QDoubleSpinBox::setValue, starttime) );

        sad::String name = a->objectName();
        e->emitClosure( blocked_bind(m_blk->txtAnimationInstanceName, &QLineEdit::setText, STD2QSTRING(name.c_str())) );
    }
    else
    {
        if (e)
        {
            e->shared()->setSelectedInstance(nullptr);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationInstanceActions::nameChanged(const QString& name)
{
    int row = m_blk->lstAnimationInstances->currentRow();
    if (row > -1)
    {
        QVariant v = m_blk->lstAnimationInstances->item(row)->data(Qt::UserRole);
        sad::animations::Instance* a = v.value<sad::animations::Instance*>();

        sad::String old_value = a->objectName();
        sad::String new_value = Q2STDSTRING(name);
        
        if (old_value != new_value)
        {
            history::instances::ChangeName* c = new history::instances::ChangeName(a, row, old_value, new_value);
            c->commitWithoutUpdatingUI(m_editor);
            m_editor->history()->add(c);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationInstanceActions::treeLinkStateChanged(bool state)
{
    if (state)
    {
        core::Editor* e = m_editor;
        int row = m_blk->lstAnimationInstances->currentRow();
        if (row > -1)
        {
            QVariant v = m_blk->lstAnimationInstances->item(row)->data(Qt::UserRole);
            sad::animations::Instance* a = v.value<sad::animations::Instance*>();

            if (a->isInstanceOf("sad::animations::Instance"))
            {
                unsigned long long old_value = a->animationMajorId();
                sad::String oldname  = a->animationName();

                unsigned long long new_value = 0;
                sad::String newname = m_blk->cmbAnimationInstanceAnimationFromTree->currentText().toStdString();
                if (m_blk->cmbAnimationInstanceAnimationFromTree->currentIndex() <= 0)
                {
                    newname = "";
                }

                QRadioButton* oldbutton = m_blk->rbAnimationInstanceFromDatabase;
                QRadioButton* button = m_blk->rbAnimationInstanceFromTree;

                history::Command* c = new history::instances::ChangeAnimation(
                    a,
                    button,
                    oldbutton,
                    oldname,
                    old_value,
                    newname,
                    new_value
                );
                c->commitWithoutUpdatingUI(e);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationInstanceActions::databaseLinkStateChanged(bool state)
{
    if (state)
    {
        core::Editor* e = m_editor;
        int row = m_blk->lstAnimationInstances->currentRow();
        if (row > -1)
        {
            QVariant v = m_blk->lstAnimationInstances->item(row)->data(Qt::UserRole);
            sad::animations::Instance* a = v.value<sad::animations::Instance*>();

            if (a->isInstanceOf("sad::animations::Instance"))
            {
                unsigned long long old_value = a->animationMajorId();
                sad::String oldname  = a->animationName();

                unsigned long long new_value = 0;
                QComboBox* combo = m_blk->cmbAnimationInstanceAnimationFromDatabase;
                if (combo->currentIndex() > 0)
                {
                    int krow = combo->currentIndex();
                    sad::animations::Animation* kv = combo->itemData(krow, Qt::UserRole).value<sad::animations::Animation*>();
                    new_value = kv->MajorId;
                }				
                sad::String newname = "";

                QRadioButton* oldbutton = m_blk->rbAnimationInstanceFromTree;
                QRadioButton* button = m_blk->rbAnimationInstanceFromDatabase;
                
                history::Command* c = new history::instances::ChangeAnimation(
                    a,
                    button,
                    oldbutton,
                    oldname,
                    old_value,
                    newname,
                    new_value
                );
                c->commitWithoutUpdatingUI(e);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void  gui::actions::AnimationInstanceActions::treeElementChanged(int new_row)
{
    if (m_blk->rbAnimationInstanceFromTree->isChecked())
    {
        core::Editor* e = m_editor;
        int row = m_blk->lstAnimationInstances->currentRow();
        if (row > -1)
        {
            QVariant v = m_blk->lstAnimationInstances->item(row)->data(Qt::UserRole);
            sad::animations::Instance* a = v.value<sad::animations::Instance*>();

            if (a->isInstanceOf("sad::animations::Instance"))
            {
                sad::String oldname  = a->animationName();
                sad::String newname = "";
                if (new_row > 0)
                {
                    newname = Q2STDSTRING(m_blk->cmbAnimationInstanceAnimationFromTree->itemText(new_row));
                }

                history::Command* c = new history::instances::ChangeAnimation(
                    a,
                    oldname,
                    newname
                );
                c->commitWithoutUpdatingUI(e);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationInstanceActions::databaseElementChanged(int new_row)
{
    if (m_blk->rbAnimationInstanceFromDatabase->isChecked())
    {
        core::Editor* e = m_editor;
        int row = m_blk->lstAnimationInstances->currentRow();
        if (row > -1)
        {
            QVariant v = m_blk->lstAnimationInstances->item(row)->data(Qt::UserRole);
            sad::animations::Instance* a = v.value<sad::animations::Instance*>();

            if (a->isInstanceOf("sad::animations::Instance"))
            {
                unsigned long long oldid  = a->animationMajorId();
                unsigned long long newid = 0;
                if (new_row > 0)
                {
                    newid = m_blk->cmbAnimationInstanceAnimationFromDatabase->itemData(new_row, Qt::UserRole).value<sad::animations::Animation*>()->MajorId;
                }

                history::Command* c = new history::instances::ChangeAnimation(
                    a,
                    oldid,
                    newid
                );
                c->commitWithoutUpdatingUI(e);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void  gui::actions::AnimationInstanceActions::objectChanged(int new_row)
{
    core::Editor* e = m_editor;
    int row = m_blk->lstAnimationInstances->currentRow();
    if (row > -1)
    {
        QVariant v = m_blk->lstAnimationInstances->item(row)->data(Qt::UserRole);
        sad::animations::Instance* a = v.value<sad::animations::Instance*>();

        sad::db::Object* obj = m_blk->cmbAnimationInstanceObject->itemData(new_row, Qt::UserRole).value<sad::db::Object*>();
        unsigned long long new_value = 0;
        if (obj)
        {
            new_value = obj->MajorId;
        }

        unsigned long long old_value = a->objectId();
        if (old_value != new_value)
        {
            history::Command* c = new history::instances::ChangeObject(
                    a,
                    old_value,
                    new_value
            );
            c->commitWithoutUpdatingUI(e);
            m_editor->history()->add(c);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void  gui::actions::AnimationInstanceActions::startTimeChanged(double new_value)
{
    core::Editor* e = m_editor;
    int row = m_blk->lstAnimationInstances->currentRow();
    if (row > -1)
    {
        QVariant v = m_blk->lstAnimationInstances->item(row)->data(Qt::UserRole);
        sad::animations::Instance* a = v.value<sad::animations::Instance*>();

        double old_value = a->startTime();
        if (sad::is_fuzzy_equal(old_value, new_value) == false)
        {
            history::Command* c = new history::instances::ChangeStartTime(
                    a,
                    old_value,
                    new_value
            );
            c->commitWithoutUpdatingUI(e);
            m_editor->history()->add(c);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void  gui::actions::AnimationInstanceActions::wayChanged(int new_row)
{
    core::Editor* e = m_editor;
    int row = m_blk->lstAnimationInstances->currentRow();
    if (row > -1)
    {
        QVariant v = m_blk->lstAnimationInstances->item(row)->data(Qt::UserRole);
        sad::animations::Instance* a = v.value<sad::animations::Instance*>();

        if (a->isInstanceOf("sad::animations::WayInstance"))
        {
            sad::p2d::app::Way* obj = m_blk->cmbWayAnimationInstanceWay->itemData(new_row, Qt::UserRole).value<sad::p2d::app::Way*>();
            unsigned long long new_value = 0;
            if (obj)
            {
                new_value = obj->MajorId;
            }

            unsigned long long old_value = static_cast<sad::animations::WayInstance*>(a)->wayMajorId();
            if (old_value != new_value)
            {
                history::Command* c = new history::instances::ChangeWay(
                        a,
                        old_value,
                        new_value
                );
                c->commitWithoutUpdatingUI(e);
                m_editor->history()->add(c);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationInstanceActions::start()
{
    core::Shared* s = m_editor->shared();
    if (s->isAnyKindOfAnimationIsRunning() == false 
        && s->selectedInstance() != nullptr)
    {
        s->selectedInstance()->restart(sad::Renderer::ref()->animations());
        m_animation->setEditor(m_editor);
        m_animation->start(s->selectedInstance() );
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::AnimationInstanceActions::stop()
{
    core::Shared* s = m_editor->shared();
    if (s->isAnyKindOfAnimationIsRunning() == true)
    {
        m_animation->setEditor(m_editor);
        m_animation->stop();
    }
}
