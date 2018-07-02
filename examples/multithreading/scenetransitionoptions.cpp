#include "scenetransitionoptions.h"

// ============================  SceneTransitionOptions::ThreadOptions ============================

SceneTransitionOptions::ThreadOptions::ThreadOptions()
{
    std::function<void()> nop = [](){};
    LoadFunction = nop;
    OnLoadedFunction = nop;
    OnFinishedFunction = nop;
}

SceneTransitionOptions::ThreadOptions& SceneTransitionOptions::ThreadOptions::load(std::function<void()> loader)
{
    LoadFunction = loader;
    return *this;
}

SceneTransitionOptions::ThreadOptions& SceneTransitionOptions::ThreadOptions::onLoaded(std::function<void()> load)
{
    OnLoadedFunction = load;
    return *this;
}

SceneTransitionOptions::ThreadOptions& SceneTransitionOptions::ThreadOptions::onFinishedFunction(std::function<void()> finished)
{
    OnFinishedFunction = finished;
    return *this;
}


// ============================  SceneTransitionOptions ============================

SceneTransitionOptions::SceneTransitionOptions()
{

}

SceneTransitionOptions::ThreadOptions& SceneTransitionOptions::mainThread()
{
    return m_main_thread_options;
}


SceneTransitionOptions::ThreadOptions& SceneTransitionOptions::inventoryThread()
{
    return m_inventory_thread_options;
}
