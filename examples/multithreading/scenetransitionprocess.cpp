#include "scenetransitionprocess.h"
#include "game.h"

// ======================================= PUBLIC METHODS =======================================

SceneTransitionProcess::SceneTransitionProcess(Game* game)
{
    m_main_thread_data.Renderer = game->rendererForMainThread();
    m_main_thread_data.Thread = NULL;
    m_main_thread_data.Texture = makeTextureForRenderer(m_main_thread_data.Renderer);

    m_inventory_thread_data.Renderer = game->rendererForInventoryThread();
    m_main_thread_data.Thread = NULL;
    m_inventory_thread_data.Texture = new sad::Texture(m_inventory_thread_data.Renderer);
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

sad::Texture* SceneTransitionProcess::makeTextureForRenderer(sad::Renderer* r)
{
    // Make a texture for sprite, that will shroud objects
    sad::Texture* t = new sad::Texture();
    // Create 2x2x4 white texture
    sad::Texture::DefaultBuffer* buf = static_cast<sad::Texture::DefaultBuffer*>(t->Buffer);
    buf->Data.resize(16, 255);
    t->BuildMipMaps = true;
    t->Bpp = 32;
    t->Format = sad::Texture::SFT_R8_G8_B8_A8;
    t->Width = 2;
    t->Height = 2;
    t->Id = 0;
    t->OnGPU = false;
    t->setRenderer(r);
    return t;
}
