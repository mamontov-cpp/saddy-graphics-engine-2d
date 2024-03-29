#include <QDoubleSpinBox>
#include "animationinstanceprocess.h"

#include <animations/animationsanimations.h>
#include <renderer.h>

#include "../blockedclosuremethodcall.h"
#include "../closuremethodcall.h"
#include "../core/editor.h"
#include "../core/shared.h"

#include "mainpanelproxy.h"

#include "uiblocks/uiblocks.h"
#include "uiblocks/uianimationblock.h"

// ============================ PUBLIC METHODS ============================

gui::AnimationInstanceProcess::AnimationInstanceProcess() : m_editor(nullptr), m_instance(nullptr)
{
    m_timer.setSingleShot(false);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerExpired()));
}

gui::AnimationInstanceProcess::~AnimationInstanceProcess()
{
    if (m_instance)
    {
        m_instance->delRef();
    }
}

void gui::AnimationInstanceProcess::start(sad::animations::Instance* i)
{
    if (m_editor)
    {
        if (m_editor->shared()->isAnyKindOfAnimationIsRunning() == false)
        {
            m_editor->shared()->setAnimationInstanceIsRunning(true);
            m_editor->panelProxy()->lockTypesTab(true);
            m_editor->panelProxy()->toggleAnimationPropertiesEditable(false);

            m_instance = i;
            m_instance->addRef();

            m_timer.setSingleShot(false);
            m_timer.setInterval(gui::AnimationInstanceProcess::TIMEOUT);

            sad::Renderer::ref()->animations()->add(m_instance);

            m_timer.start();
        }
    }
}

void gui::AnimationInstanceProcess::stop()
{
    m_mutex.lock();
    if (m_instance != nullptr)
    {
        if (m_editor->shared()->isAnyKindOfAnimationIsRunning())
        {
            m_editor->shared()->setAnimationInstanceIsRunning(false);
            m_editor->emitClosure( bind(m_editor->panelProxy(), &MainPanelProxy::toggleAnimationPropertiesEditable, true) );
            m_editor->emitClosure( bind(m_editor->panelProxy(), &MainPanelProxy::lockTypesTab, false) );

            if (m_instance->finished() == false)
            {
                sad::Renderer::ref()->lockRendering();
                sad::Renderer::ref()->animations()->remove(m_instance);
                m_instance->cancel(sad::Renderer::ref()->animations());
                sad::Renderer::ref()->unlockRendering();
            }

            m_instance = nullptr;

            m_editor->emitClosure( bind(&m_timer, &QTimer::stop) );
        }
    }
    m_mutex.unlock();
}

void  gui::AnimationInstanceProcess::setEditor(core::Editor* e)
{
    m_editor = e;
}

const int gui::AnimationInstanceProcess::TIMEOUT = 100;

// ========================== PUBLIC SLOTS METHODS ==========================

void gui::AnimationInstanceProcess::timerExpired()
{
    if (m_editor && m_instance)
    {
        if (m_editor->shared()->isAnyKindOfAnimationIsRunning())
        {
            if (m_instance)
            {
                // Update animation time if instance updates it (WayMoving animation type could do it)
                if (m_instance->animation())
                {
                    double animationtime = m_instance->animation()->time();
                    QDoubleSpinBox* timebox = m_editor->uiBlocks()->uiAnimationBlock()->dsbAnimationTime;
                    double animationbeforetime = timebox->value();
                    if (sad::is_fuzzy_equal(animationtime, animationbeforetime) == false)
                    {
                        invoke_blocked(timebox, &QDoubleSpinBox::setValue, animationtime);
                    }
                }
                
                if (m_instance->finished())
                {
                    this->stop();
                }
            }
        }
    }
}

