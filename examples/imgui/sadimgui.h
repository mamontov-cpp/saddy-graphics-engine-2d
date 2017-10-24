#include <imgui.h>
#include <functional>

namespace sad
{

namespace imgui
{

class ImGui
{
public:
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
protected:
    /*! A flag, indicating, whether event processing is enabled
     */
    static bool m_event_processing_enabled;
    /*! Returns true if inner ImGui pipeline is enabled
     */
    static bool m_pipeline_enabled;
};

}

}