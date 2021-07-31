/*! \file scenetransitionoptions.h

    Describes an options for scene transitions
 */
#pragma once
#include <functional>
/*! An options, that should be used for scene transitions between threads
 */
class SceneTransitionOptions
{
public:
/*! An options, specific for thread
 */
struct ThreadOptions
{
    /*! A thread options
     */
    ThreadOptions();
    /*! Sets loader function ,that should be used in thread
        \param[in] loader a function
        \return self-reference
     */
    SceneTransitionOptions::ThreadOptions& load(std::function<void()> loader);
    /*! Sets function ,that should be executed, when execution reaches
        middle of execution and load function is finished
        \param[in] load a function
        \return self-reference
     */
    SceneTransitionOptions::ThreadOptions& onLoaded(std::function<void()> load);
    /*! Sets function ,that should be executed, when transition finished
        \param[in] finished a function, that should be called, when loading is finished
        \return self-reference
     */
    SceneTransitionOptions::ThreadOptions& onFinishedFunction(std::function<void()> finished);

    /*! A function, that should be used to load data in separate thread
     */
    std::function<void()> LoadFunction;
    /*! A function ,that should be performed, when data is loaded
     */
    std::function<void()> OnLoadedFunction;
    /*! A function, that should be used, when transition is finished
     */
    std::function<void()> OnFinishedFunction;
};
    /*! Initializes options with default empty callback
     */
    SceneTransitionOptions();
    /*! Returns options for main thread
        \return main thread
     */
    SceneTransitionOptions::ThreadOptions& mainThread();

    /*! Returns options for inventory thread
        \return inventory thread
     */
    SceneTransitionOptions::ThreadOptions& inventoryThread();
private:
    /*! An options for main thread
     */
    SceneTransitionOptions::ThreadOptions m_main_thread_options;
    /*! An options for inventory thread
     */
    SceneTransitionOptions::ThreadOptions m_inventory_thread_options;
};
