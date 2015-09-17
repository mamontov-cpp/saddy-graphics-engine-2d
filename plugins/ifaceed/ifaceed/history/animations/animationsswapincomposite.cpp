#include "animationsswapincomposite.h"


#include "../../gui/animationactions.h"

#include "../../mainpanel.h"
#include "../core/editor.h"

#include "../closuremethodcall.h"

history::animations::SwapInComposite::SwapInComposite(
    sad::animations::Composite* a, 
    int pos1, 
    int pos2
)
: m_animation(a), m_position1(pos1), m_position2(pos2)
{
    m_animation->addRef();
}

history::animations::SwapInComposite::~SwapInComposite()
{
    m_animation->delRef();
}

void history::animations::SwapInComposite::commit(core::Editor * ob)
{
    m_animation->swap(m_position1, m_position2);
    if (ob)
    {
        if (ob->shared()->selectedAnimation() == m_animation)
        {
            QListWidget* w = ob->panel()->UI()->lstCompositeList;
            this->swapItemsInList(w);
        }
    }
}

void history::animations::SwapInComposite::rollback(core::Editor * ob) //-V524
{
    m_animation->swap(m_position1, m_position2);
    if (ob)
    {
        if (ob->shared()->selectedAnimation() == m_animation)
        {
            QListWidget* w = ob->panel()->UI()->lstCompositeList;
            this->swapItemsInList(w);
        }
    }
}

void history::animations::SwapInComposite::swapItemsInList(QListWidget* w)
{
    QString name1 = w->item(m_position1)->text();
    QVariant v1 = w->item(m_position1)->data(Qt::UserRole);

    QString name2 = w->item(m_position2)->text();
    QVariant v2 = w->item(m_position2)->data(Qt::UserRole);

    w->item(m_position1)->setText(name2);
    w->item(m_position1)->setData(Qt::UserRole, v2);

    w->item(m_position2)->setText(name1);
    w->item(m_position2)->setData(Qt::UserRole, v1);
}
