#include "scenetransitionprocess.h"
#include "game.h"

#include <stdexcept>
#include <pipeline/pipeline.h>

/*! A transition time, used to make transition between screens
 */
#define TRANSITION_TIME (1000)

// ======================================= PUBLIC METHODS =======================================

SceneTransitionProcess::SceneTransitionProcess(Game* game)
{
    fillThreadData(&m_main_thread_data,  game->rendererForMainThread());
    fillThreadData(&m_inventory_thread_data, game->rendererForInventoryThread());
}

SceneTransitionProcess::~SceneTransitionProcess()
{
    destroyThreadData(&m_main_thread_data);
    destroyThreadData(&m_inventory_thread_data);
}

void SceneTransitionProcess::start(const SceneTransitionOptions& options)
{
    m_options = options;
    SceneTransitionOptions* opts = &(m_options);
    SceneTransitionProcess::ThreadData* main  = &(m_main_thread_data);
    SceneTransitionProcess::ThreadData* inventory  = &(m_inventory_thread_data);

    main->Sprite->setColor(sad::AColor(0, 0, 0, 255));
    inventory->Sprite->setColor(sad::AColor(0, 0, 0, 255));

    main->ExecutedOnLoaded = false;
    main->FinishedDarkening = false;

    inventory->ExecutedOnLoaded = false;
    inventory->FinishedDarkening = false;

    main->LighteningAnimationInstance->clearCallbacks();
    main->LighteningAnimationInstance->clearFinished();

    main->DarkeningAnimationInstance->clearCallbacks();
    main->DarkeningAnimationInstance->clearFinished();

    inventory->LighteningAnimationInstance->clearCallbacks();
    inventory->LighteningAnimationInstance->clearFinished();

    inventory->DarkeningAnimationInstance->clearCallbacks();
    inventory->DarkeningAnimationInstance->clearFinished();

    main->Thread = new sad::Thread(opts->mainThread().LoadFunction);
    inventory->Thread = new sad::Thread(opts->inventoryThread().LoadFunction);

    std::function<void()> task_for_main_renderer = [main, inventory, opts]() {
        main->Thread->run();
        sad::Renderer* r = inventory->Renderer;
        r->scenes()[r->scenes().size() - 1]->addNode(inventory->Sprite);

        std::function<void()> on_main_darkening_end = [r, main, inventory, opts]() {
            // Wait for other thread to complete
            main->Thread->wait();
            inventory->Thread->wait();
            delete main->Thread;
            main->Thread = NULL;

            (opts->mainThread().OnLoadedFunction)();
            r->scenes()[r->scenes().size() - 1]->removeNode(main->Sprite);
            r->scenes()[r->scenes().size() - 1]->addNode(main->Sprite);
            main->Sprite->setColor(sad::AColor(0, 0, 0, 0));
            main->ExecutedOnLoaded = true;
            while (!inventory->ExecutedOnLoaded)
            {
                sad::sleep(100);
            }

            main->LighteningAnimationInstance->end([r, main, inventory, opts]() {
                main->FinishedDarkening = true;
                while(!inventory->FinishedDarkening)
                {
                    sad::sleep(100);
                }
                (opts->mainThread().OnFinishedFunction)();
                r->scenes()[r->scenes().size() - 1]->removeNode(main->Sprite);
            });
    		r->animations()->add(main->LighteningAnimationInstance);
    	};
        main->DarkeningAnimationInstance->end(on_main_darkening_end);

        r->animations()->add(main->DarkeningAnimationInstance);
    };

    std::function<void()> task_for_inventory_renderer = [main, inventory, opts]() {
        inventory->Thread->run();
        sad::Renderer* r = inventory->Renderer;
        r->scenes()[r->scenes().size() - 1]->addNode(inventory->Sprite);

        std::function<void()> on_inventory_darkening_end = [r, main, inventory, opts]() {
            // Wait for other thread to complete
            inventory->Thread->wait();
            main->Thread->wait();
            delete inventory->Thread;
            inventory->Thread = NULL;

            (opts->inventoryThread().OnLoadedFunction)();
            r->scenes()[r->scenes().size() - 1]->removeNode(inventory->Sprite);
            r->scenes()[r->scenes().size() - 1]->addNode(inventory->Sprite);
            inventory->Sprite->setColor(sad::AColor(0, 0, 0, 0));
            inventory->ExecutedOnLoaded = true;
            while (!main->ExecutedOnLoaded)
            {
                sad::sleep(100);
            }

            inventory->LighteningAnimationInstance->end([r, main, inventory, opts]() {
                inventory->FinishedDarkening = true;
                while(!main->FinishedDarkening)
                {
                    sad::sleep(100);
                }
                (opts->inventoryThread().OnFinishedFunction)();
                r->scenes()[r->scenes().size() - 1]->removeNode(inventory->Sprite);
            });
            r->animations()->add(inventory->LighteningAnimationInstance);
        };

        inventory->DarkeningAnimationInstance->end(on_inventory_darkening_end);
        r->animations()->add(inventory->DarkeningAnimationInstance);
    };

    m_main_thread_data.Renderer->pipeline()->appendTask(task_for_main_renderer);
    m_inventory_thread_data.Renderer->pipeline()->appendTask(task_for_inventory_renderer);
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

void SceneTransitionProcess::fillThreadData(SceneTransitionProcess::ThreadData* data, sad::Renderer* r)
{
    data->Renderer = r;
    data->Thread = NULL;

    data->Texture = makeTextureForRenderer(r);
    data->Texture->addRef();

    data->Sprite = makeCoveringSpriteForTexture(data->Texture);
    data->Sprite->addRef();

    data->LighteningAnimation = makeAnimation(LIGHTENING);
    data->LighteningAnimation->addRef();

    data->LighteningAnimationInstance = makeInstance(data->LighteningAnimation, data->Sprite);
    data->LighteningAnimationInstance->addRef();

    data->DarkeningAnimation = makeAnimation(DARKENING);
    data->DarkeningAnimation->addRef();

    data->DarkeningAnimationInstance = makeInstance(data->DarkeningAnimation, data->Sprite);
    data->DarkeningAnimationInstance->addRef();

    data->ExecutedOnLoaded = false;
    data->FinishedDarkening = false;
}

void SceneTransitionProcess::destroyThreadData(SceneTransitionProcess::ThreadData* data)
{
    data->DarkeningAnimationInstance->delRef();
    data->DarkeningAnimation->delRef();

    data->LighteningAnimationInstance->delRef();
    data->LighteningAnimation->delRef();

    data->Sprite->delRef();
    data->Texture->delRef();
}

sad::Sprite2D* SceneTransitionProcess::makeCoveringSpriteForTexture(sad::Texture* t)
{
    sad::Sprite2D* sprite = new sad::Sprite2D();
    sprite->setTexture(t);
    sprite->setTextureCoordinates(sad::Rect2D(0, 0, 2, 2));
    sprite->setArea(sad::Rect2D(0, 0, 800, 600));
    sprite->setColor(sad::AColor(0, 0, 0, 255));

    return sprite;
}

sad::animations::Color* SceneTransitionProcess::makeAnimation(bool darkening)
{
    sad::animations::Color* color = new sad::animations::Color();
    unsigned int start = 0, end = 255;
    if (darkening)
    {
        start = 255;
        end = 0;
    }
    color->setMinColor(sad::AColor(0, 0, 0, start));
    color->setMaxColor(sad::AColor(0, 0, 0, end));
    color->setTime(TRANSITION_TIME);
    color->setLooped(false);
    return color;
}

sad::animations::Instance* SceneTransitionProcess::makeInstance(sad::animations::Color* c, sad::Sprite2D* s)
{
    sad::animations::Instance* animation = new sad::animations::Instance();
    animation->setAnimation(c);
    animation->setObject(s);
    animation->clearFinished();
    animation->disableStateRestoringOnFinish();
    return animation;
}

SceneTransitionProcess::SceneTransitionProcess(const SceneTransitionProcess&)
{
    throw std::logic_error("This object is non-copyable");
}

SceneTransitionProcess& SceneTransitionProcess::operator=(const SceneTransitionProcess& p)
{
    throw std::logic_error("This object is non-copyable");
    return *this;
}
