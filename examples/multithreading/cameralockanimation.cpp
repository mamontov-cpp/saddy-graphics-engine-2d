#include "cameramovement.h"
#include "cameralockanimation.h"
#include "game.h"

#include <fuzzyequal.h>
#include <renderer.h>

#include <cassert>

CameraLockAnimation::CameraLockAnimation(Game* g, double finishing_offset, double total_time) : m_game(g), m_finishing_offset(finishing_offset), m_finished(false)
{
    assert(g);
    if (sad::is_fuzzy_zero(total_time))
    {
        total_time = 1000.0;
    }
    m_total_time = total_time;
    m_starting_offset  = g->rendererForMainThread()->globalTranslationOffset().x();
    m_pausable_timer.start();
}

CameraLockAnimation::~CameraLockAnimation()
{
    
}


bool CameraLockAnimation::shouldBeDestroyedAfterProcessing()
{
    if (!m_game->cameraMovement()->isLocked())
    {
        return true;
    }
    if (m_finished)
    {
        return true;
    }
    return false;
}

void CameraLockAnimation::_process()
{
    bool game_is_paused = m_game->isPaused();
    if (game_is_paused && !m_pausable_timer.paused())
    {
        m_pausable_timer.pause();
        return;
    }
    if (!game_is_paused && m_pausable_timer.paused())
    {
        m_pausable_timer.resume();
    }
    if (!m_game->cameraMovement()->isLocked())
    {
        return;
    }
    double current_offset = m_game->rendererForMainThread()->globalTranslationOffset().x();
    double precision = m_pausable_timer.elapsed() / m_total_time;
    if (precision > 1.0)
    {
        precision = 1.0;
    }
    double computed_offset = m_starting_offset + (m_finishing_offset - m_starting_offset) * precision;
    if (sad::is_fuzzy_equal(current_offset, computed_offset))
    {
        m_finished = true;
        return;
    }
    double dx = computed_offset - current_offset;
    if (dx < 0)
    {
        m_game->cameraMovement()->moveCameraLeft(dx, true);
    }
    else
    {
        m_game->cameraMovement()->moveCameraRight(dx, true);
    }
    m_game->setWallsAccordingToOffset();
}

