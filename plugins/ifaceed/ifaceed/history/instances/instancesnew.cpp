#include "instancesnew.h"

#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationinstanceactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"
#include "../../gui/uiblocks/uianimationsgroupblock.h"


Q_DECLARE_METATYPE(sad::animations::Instance*) //-V566


history::instances::New::New(sad::animations::Instance* d) : m_node(d)
{
    m_node->addRef();
}

history::instances::New::~New()
{
    m_node->delRef();
}


void history::instances::New::commit(core::Editor * ob)
{
    m_node->Active = true;

    QVariant v;
    v.setValue(m_node);

    if(ob)
    {
        QString name = ob->actions()->instanceActions()->nameForInstance(m_node);


        void (QListWidget::*f)(QListWidgetItem*) = &QListWidget::addItem;

        QListWidgetItem* item = new QListWidgetItem(name);
        item->setData(Qt::UserRole, v);

        ob->emitClosure( bind(
            ob->uiBlocks()->uiAnimationInstanceBlock()->lstAnimationInstances,
            f,
            item
        ));

        item = new QListWidgetItem(name);
        item->setData(Qt::UserRole, v);

        ob->emitClosure( bind(
            ob->uiBlocks()->uiAnimationsGroupBlock()->lstAnimationsGroupAllAnimations,
            f,
            item
        ));
    }
}

void history::instances::New::rollback(core::Editor * ob)
{
    m_node->Active = false;

    if (ob)
    {
        ob->emitClosure( bind(
            this,
            &history::instances::New::eraseFromList,
             ob->uiBlocks()->uiAnimationInstanceBlock()->lstAnimationInstances
        ));

        ob->emitClosure( bind(
            this,
            &history::instances::New::eraseFromList,
            ob->uiBlocks()->uiAnimationsGroupBlock()->lstAnimationsGroupAllAnimations
        ));
    }
}


void history::instances::New::eraseFromList(QListWidget* list)
{
    if (list->count())
    {
        delete list->takeItem(list->count() - 1);
    }
}
