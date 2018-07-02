#include "scenetransitionprocess.h"
#include "game.h"

// ======================================= PUBLIC METHODS =======================================

SceneTransitionProcess::SceneTransitionProcess(Game* game)
{
    m_main_thread_data.Renderer = game->rendererForMainThread();
    m_inventory_thread_data.Renderer = game->rendererForInventoryThread();

    m_main_thread_data.Texture = new sad::Texture();
    m_inventory_thread_data.Texture = new sad::Texture();
}


void SceneTransitionProcess::unloadTexturesForMainThread() const
{
    unloadTextureIfOnGPU(m_main_thread_data.Texture);
}

void SceneTransitionProcess::unloadTexturesForInventoryThread() const
{
    unloadTextureIfOnGPU(m_main_thread_data.Texture);
}


// ======================================= PRIVATE METHODS =======================================


void SceneTransitionProcess::unloadTextureIfOnGPU(sad::Texture* t)
{
    if (t)
    {
        if (t->OnGPU)
        {
            t->unload();
        }
    }
}