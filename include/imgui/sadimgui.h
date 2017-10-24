#include "../../3rdparty/imgui/imgui.h"
#include <functional>
#include <vector>
#include <string>

namespace sad
{

namespace imgui
{

class ImGui
{
public:
    /*! A task object
     */
    struct Process
    {
        std::string Name;               //!< A name for taks
        std::function<void()> Function; //!< A pipeline function
        /*! Constructs new process from a function
         */
        inline Process(std::function<void()> fn) : Function(fn) {}
        /*! Constructs new proces from function and name
            \param[in] name a name
            \param[in] fn function
         */
        inline Process(const std::string& name, std::function<void()> fn) : Name(name), Function(fn) {}
    };
    /*! Adds new function to pipeline
        \param[in] fn function
        \return whether adding was successfull
     */
    static bool add(std::function<void()> fn);
    /*! Adds new function with name to pipeline. If such function exists, does
        nothing
        \param[in] name a name for a function
        \param[in] fn function
        \return whether adding was successfull
     */
    static bool add(const std::string& name, std::function<void()> fn);
    /*! Removes a function from pipeline by name
        \param[in] name a name for function
     */
    static void remove(const std::string& name);
    /*! Returns true if pipeline has process with specified name
        \param[in] name a name for process
        \return true if process exists
     */
    static bool hasProcess(const std::string& name);
    /*! Finds a process by it's name. Returns -1 if not found
        \param[in] name a name for process
        \return -1 if not found
     */
    static int findProcess(const std::string& name);
    /*! Inserts one process before another. Returns true if such process exists, by looking to it's name
        \param[in] name a name for process
        \param[in] p process
     */
    static bool insertBefore(const std::string& name, const Process& p);
    /*! Inserts one process after another. Returns true if such process exists, by looking to it's name
        \param[in] name a name for process
        \param[in] p process
     */
    static bool insertAfter(const std::string& name, const Process& p);
    /*! Inserts a process at specified position
        \param[in] pos position
        \param[in] p process
     */
    static bool insert(size_t pos, const Process& p);
    /*! Returns size of pipeline
        \return size of pipeline
     */
    static size_t pipelineSize();
    /*! Inits ImGui bindings for main renderer (sad::Renderer::ref())
     */
    static void init();
    /*! Enables ImGui output and processing
     */
    static void enable();
    /*! Disables ImGui output and processing
     */
    static void disable();
    /*! Sets whether pipeline is enabled (default false). If it's disabled, ImGui windows won't be rendered
        \param[in] flag a flag value (if true, pipeline is enabled, otherwise - false)
     */
    static void togglePipelineEnabled(bool flag);
    /*!  Whether inner ImGui pipeline is enabled. If it's disabled all ImGui windows won't be rendered
     */
    static bool isPipelineEnabled();
    /*! Sets whether event processing by ImGui is enabled
        \param[in] flag a flag value (if true, event processing is enabled, otherwise - false)
     */
    static void toggleEventProcessingEnabled(bool flag);
    /*! Returns true if event processing by ImGui is enabled
     */
    static bool isEventProcessingEnabled();
    /*! Runs a pipeline for ImGui
     */
    static void runPipeline();
protected:
    /*! A flag, indicating, whether event processing is enabled
     */
    static bool m_event_processing_enabled;
    /*! Returns true if inner ImGui pipeline is enabled
     */
    static bool m_pipeline_enabled;
    /*! A pipeline list object
     */
    static std::vector<Process> m_pipeline;
};

}

}