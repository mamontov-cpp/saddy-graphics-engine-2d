#include <imgui/sadimgui.h>
#ifdef WIN32
    #ifndef NOMINMAX
    #define NOMINMAX 
    #endif
    #include <windows.h>
    #include <gl/gl.h>
    #include <gl/glu.h>
#endif

#ifdef X11
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include <renderer.h>
#include <log/consoletarget.h>
#include <window.h>
#include <os/windowhandles.h>
#include <mousecursor.h>
#include <keycodes.h>
#include <orthographiccamera.h>
#include <scene.h>
#include <pipeline/pipeline.h>
#include <pipeline/pipelineprocess.h>
#include <input/controls.h>
#include <input/events.h>

#include <3rdparty/glext/glext.h>
#include <opengl.h>
#include <renderer.h>
#include <glcontext.h>
#include <os/glheaders.h>

// Backend API
bool     ImGui_ImplOpenGL3_Init(const char* glsl_version = NULL);
void     ImGui_ImplOpenGL3_Shutdown();
void     ImGui_ImplOpenGL3_NewFrame();
void     ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);

// (Optional) Called by Init/NewFrame/Shutdown
bool     ImGui_ImplOpenGL3_CreateFontsTexture();
void     ImGui_ImplOpenGL3_DestroyFontsTexture();
bool     ImGui_ImplOpenGL3_CreateDeviceObjects();
void     ImGui_ImplOpenGL3_DestroyDeviceObjects();

bool sad::imgui::ImGui::m_event_processing_enabled = false;

bool sad::imgui::ImGui::m_pipeline_enabled = false;

std::vector<sad::imgui::ImGui::Process> sad::imgui::ImGui::m_pipeline;

bool sad::imgui::ImGui::add(std::function<void()> fn)
{
    Process p(fn);
    m_pipeline.push_back(fn);
    return true;
}

bool sad::imgui::ImGui::add(const std::string& name, std::function<void()> fn)
{
    if (sad::imgui::ImGui::hasProcess(name))
    {
        return false;
    }
    Process p(name, fn);
    m_pipeline.push_back(fn);
    return true;
}

void sad::imgui::ImGui::remove(const std::string& name)
{
    for(size_t i = 0; i < m_pipeline.size(); i++) 
    {
        if (m_pipeline[i].Name == name)
        {
            m_pipeline.erase(m_pipeline.begin() + i);
            --i;
        }
    }
}

bool sad::imgui::ImGui::hasProcess(const std::string& name)
{
    return sad::imgui::ImGui::findProcess(name) != -1;
}

int sad::imgui::ImGui::findProcess(const std::string& name)
{
    for(size_t i = 0; i < m_pipeline.size(); i++) 
    {
        if (m_pipeline[i].Name == name)
        {
            return i;
        }
    }
    return -1;
}

bool sad::imgui::ImGui::insertBefore(const std::string& name, const Process& p)
{
    if (p.Name.size())
    {
        if (sad::imgui::ImGui::hasProcess(p.Name))
        {
            return false;
        }
    }
    int pos = sad::imgui::ImGui::findProcess(name);
    if (pos == -1)
    {
        m_pipeline.push_back(p);
    }
    else
    {
        m_pipeline.insert(m_pipeline.begin() + pos, p);
    }
    return true;
}

bool sad::imgui::ImGui::insertAfter(const std::string& name, const Process& p)
{
    if (p.Name.size())
    {
        if (sad::imgui::ImGui::hasProcess(p.Name))
        {
            return false;
        }
    }
    int pos = sad::imgui::ImGui::findProcess(name);
    if (pos == -1)
    {
        m_pipeline.push_back(p);
    }
    else
    {
        if (pos == (m_pipeline.size() - 1))
        {
            m_pipeline.push_back(p);
        }
        else
        {
            m_pipeline.insert(m_pipeline.begin() + pos, p);
        }
    }
    return true;
}

bool sad::imgui::ImGui::insert(size_t pos, const Process& p)
{
    if (p.Name.size())
    {
        if (sad::imgui::ImGui::hasProcess(p.Name))
        {
            return false;
        }
    }
     if (pos >= m_pipeline.size())
    {
        m_pipeline.push_back(p);
    }
    else
    {
        m_pipeline.insert(m_pipeline.begin() + pos, p);
    }
    return true;
}

size_t sad::imgui::ImGui::pipelineSize()
{
    return m_pipeline.size();
}

void sad::imgui::ImGui::enable()
{
    sad::imgui::ImGui::toggleEventProcessingEnabled(true);
    sad::imgui::ImGui::togglePipelineEnabled(true);
}

void sad::imgui::ImGui::disable()
{
    sad::imgui::ImGui::toggleEventProcessingEnabled(false);
    sad::imgui::ImGui::togglePipelineEnabled(false);
}

void sad::imgui::ImGui::toggleEventProcessingEnabled(bool flag)
{
    m_event_processing_enabled = flag;
}

bool sad::imgui::ImGui::isEventProcessingEnabled()
{
    return m_event_processing_enabled;
}

void sad::imgui::ImGui::togglePipelineEnabled(bool flag)
{
    m_pipeline_enabled = flag;
}

bool sad::imgui::ImGui::isPipelineEnabled()
{
    return m_pipeline_enabled;
}

void sad::imgui::ImGui::runPipeline()
{
    for(size_t i = 0; i < m_pipeline.size(); i++)
    {
        m_pipeline[i].Function();
    }
}

static PFNGLUSEPROGRAMPROC glUseProgram;
static bool glUseProgramFetched = false;

/*! A rendering callback for ImGui
    \param[in] draw_data a data for drawing
 */
void render_draw_lists(ImDrawData* draw_data)
// Note that this implementation is little overcomplicated because we are saving/setting up/restoring every OpenGL state explicitly, in order to be able to run within any OpenGL engine that doesn't do so.
// If text or lines are blurry when integrating ImGui in your engine: in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
{

    if (sad::Renderer::ref()->context()->isOpenGL3compatible())
    {
        //ImGui::Render();
        //glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        //glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        return;
    }
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // We are using the OpenGL fixed pipeline to make the example code simpler to read!
    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers, polygon fill.
    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (!glUseProgramFetched)
    {
        glUseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>(getProcAdress("glUseProgram"));
        glUseProgramFetched = true;
    }
    if (glUseProgram)
    {
        glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    }
    // Setup viewport, orthographic projection matrix
    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Render command lists
    #define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
        const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;
        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + OFFSETOF(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
    #undef OFFSETOF

    // Restore modified state
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindTexture(GL_TEXTURE_2D, (GLuint)last_texture);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
    glPolygonMode(GL_FRONT, last_polygon_mode[0]); glPolygonMode(GL_BACK, last_polygon_mode[1]);
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
    glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}
/*! A local storage string for storing callback for objects
 */
static sad::String local_string;
/*! A font texture for storing font glyphs
 */
static GLuint       font_texture = 0;

/*! A callback for fetching clipboard text
    \param[in] user_data a user data
    \return result value
 */ 
static const char* get_clipboard_text(void* user_data)
{
    local_string = sad::Renderer::ref()->clipboard()->get();
    return local_string.c_str();
}

/*! Sets a clipboard text
    \param[in] user_data a user data
    \param[in] text a stored text
 */
static void set_clipboard_text(void* user_data, const char* text)
{
    sad::Renderer::ref()->clipboard()->set(text);
}

/*! Initializes ImGui state
 */
static bool create_device_objects()
{
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &font_texture);
    glBindTexture(GL_TEXTURE_2D, font_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Store our identifier
    io.Fonts->TexID = (void *)(intptr_t)font_texture;

    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);

    return true;
}

/*! Frees a font texture
 */
static void  invalidate_device_objects()
{
    if (font_texture)
    {
        glDeleteTextures(1, &font_texture);
        ImGui::GetIO().Fonts->TexID = 0;
        font_texture = 0;
    }
}

/*! A callback for freeing all data 
 */
static void shutdown()
{
    invalidate_device_objects();
    // Seems to be unused in new versios of ImGui
    // ImGui::Shutdown();
    const char* glsl_version = "#version 130";
    if (sad::Renderer::ref()->context()->isOpenGL3compatible())
    {
        ImGui_ImplOpenGL3_Shutdown();
    }
    ImGui::DestroyContext();
}

/*! Called on beginning of new frame
 */
void new_frame()
{
    bool should_init = false;
    if (!font_texture) 
    {
        const char* glsl_version = "#version 130";
        if (sad::Renderer::ref()->context()->isOpenGL3compatible())
        {
            ImGui_ImplOpenGL3_Init(glsl_version);
        }
        create_device_objects();
        should_init = true;
    }
    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    int w, h;
    sad::Rect2I r = sad::Renderer::ref()->window()->rect();
    w = static_cast<int>(r.width());
    h = static_cast<int>(r.height());

    io.DisplaySize = ImVec2((float)w, (float)h);
    io.DisplayFramebufferScale = ImVec2(1, 1);

    // Setup time step
    io.DeltaTime = static_cast<float>(1.0 / sad::Renderer::ref()->fps());

    // Setup inputs
    if ((sad::Renderer::ref()->window()->hidden() == false) && (sad::Renderer::ref()->window()->minimized() == false))
    {
        if (io.WantSetMousePos)
        {
            sad::Renderer::ref()->setCursorPosition(sad::Point2D((double)io.MousePos.x, (double)io.MousePos.y));
        }
        else
        {
            sad::MaybePoint3D mp = sad::Renderer::ref()->cursorPosition();
            if (mp.exists() == false)
            {
                io.MousePos = ImVec2(-FLT_MAX,-FLT_MAX);
                sad::Point3D p = mp.value();
            }
        }
    }
    else
    {
        io.MousePos = ImVec2(-FLT_MAX,-FLT_MAX);
    }


    // Hide OS mouse cursor if ImGui is drawing it
    if (io.MouseDrawCursor) {
        sad::Renderer::ref()->cursor()->hide();
    } else {
        sad::Renderer::ref()->cursor()->show();
    }
    if (sad::Renderer::ref()->context()->isOpenGL3compatible())
    {
        ImGui_ImplOpenGL3_NewFrame();
    }
    // Start the frame
    ImGui::NewFrame();
}

/*! A callback for mouse move event
    \param[in] ev event
 */
static void mouse_move_callback(const sad::input::MouseMoveEvent& ev)
{
    if (sad::imgui::ImGui::isEventProcessingEnabled())
    {
        sad::Rect2I r = sad::Renderer::ref()->window()->rect();
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2((float)(ev.Point.x()), (float)(ev.Point.y()));
    }
}

/*! A callback for key press event
    \param[in] ev event
 */
static void key_press_callback(const sad::input::KeyPressEvent& ev)
{
    if (sad::imgui::ImGui::isEventProcessingEnabled())
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[ev.key()] = true;

        if (ev.ReadableKey.exists())
        {
            io.AddInputCharactersUTF8(ev.ReadableKey.value().c_str());
        }

        io.KeyCtrl = ev.CtrlHeld;
        io.KeyShift = ev.ShiftHeld;
        io.KeyAlt = ev.AltHeld;
        io.KeySuper = false;
    }
}

/*! A callback for key release event
    \param[in] ev event
 */
static void key_release_callback(const sad::input::KeyReleaseEvent& ev)
{
    if (sad::imgui::ImGui::isEventProcessingEnabled())
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[ev.key()] = false;


        io.KeyCtrl = ev.CtrlHeld;
        io.KeyShift = ev.ShiftHeld;
        io.KeyAlt = ev.AltHeld;
        io.KeySuper = false;
    }
}

/*! A callback for mouse press event
    \param[in] ev event
 */
static void mouse_press_callback(const sad::input::MousePressEvent& ev)
{
    if (sad::imgui::ImGui::isEventProcessingEnabled())
    {
        ImGuiIO& io = ImGui::GetIO();
        switch (ev.Button)
        {
            case sad::MouseLeft: { io.MouseDown[0] = true; }
            case sad::MouseMiddle: { io.MouseDown[2] = true; }
            case sad::MouseRight: { io.MouseDown[1] = true; }
        };
        sad::Rect2I r = sad::Renderer::ref()->window()->rect();
        io.MousePos = ImVec2((float)(ev.Point.x()),  (float)(ev.Point.y()));
    }
}

/*! A callback for mouse double click event
    \param[in] ev event
 */
static void mouse_double_click_callback(const sad::input::MouseDoubleClickEvent& ev)
{
    if (sad::imgui::ImGui::isEventProcessingEnabled())
    {
        ImGuiIO& io = ImGui::GetIO();
        switch (ev.Button)
        {
            case sad::MouseLeft: { io.MouseDown[0] = true; }
            case sad::MouseMiddle: { io.MouseDown[2] = true; }
            case sad::MouseRight: { io.MouseDown[1] = true; }
        };
        sad::Rect2I r = sad::Renderer::ref()->window()->rect();
        io.MousePos = ImVec2((float)(ev.Point.x()), (float)(ev.Point.y()));
    }
}

/*! A callback for mouse release event
    \param[in] ev event
 */
static void mouse_release_callback(const sad::input::MousePressEvent& ev)
{
    if (sad::imgui::ImGui::isEventProcessingEnabled())
    {
        ImGuiIO& io = ImGui::GetIO();
        
        switch (ev.Button)
        {
            case sad::MouseLeft: { io.MouseDown[0] = false; }
            case sad::MouseMiddle: { io.MouseDown[2] = false; }
            case sad::MouseRight: { io.MouseDown[1] = false; }
        };
        sad::Rect2I r = sad::Renderer::ref()->window()->rect();
        io.MousePos = ImVec2((float)(ev.Point.x()), (float)(ev.Point.y()));
    }
}

/*! A callback for mouse wheel event
    \param[in] ev event
 */
static void mouse_wheel_callback(const sad::input::MouseWheelEvent& ev)
{
    if (sad::imgui::ImGui::isEventProcessingEnabled())
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel = static_cast<float>(ev.Delta);
    }
}


static void imgui_callback();

/*! Runs main imgui pipeline
 */ 
static void run_imgui_pipeline()
{
    if (sad::imgui::ImGui::isPipelineEnabled())
    {
        new_frame();
        sad::imgui::ImGui::runPipeline();
        ImGui::Render();
    }
}

void sad::imgui::ImGui::init()
{
    ::ImGui::CreateContext();
    ::ImGuiIO& io = ::ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab] = sad::Tab;                     // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_LeftArrow] = sad::KeyLeft;
    io.KeyMap[ImGuiKey_RightArrow] = sad::KeyRight;
    io.KeyMap[ImGuiKey_UpArrow] = sad::KeyUp;
    io.KeyMap[ImGuiKey_DownArrow] = sad::KeyDown;
    io.KeyMap[ImGuiKey_PageUp] = sad::PageUp;
    io.KeyMap[ImGuiKey_PageDown] = sad::PageDown;
    io.KeyMap[ImGuiKey_Home] = sad::Home;
    io.KeyMap[ImGuiKey_End] = sad::End;
    io.KeyMap[ImGuiKey_Delete] = sad::Delete;
    io.KeyMap[ImGuiKey_Backspace] = sad::Backspace;
    io.KeyMap[ImGuiKey_Enter] = sad::Enter;
    io.KeyMap[ImGuiKey_Escape] = sad::Esc;
    io.KeyMap[ImGuiKey_A] = sad::A;
    io.KeyMap[ImGuiKey_C] = sad::C;
    io.KeyMap[ImGuiKey_V] = sad::V;
    io.KeyMap[ImGuiKey_X] = sad::X;
    io.KeyMap[ImGuiKey_Y] = sad::Y;
    io.KeyMap[ImGuiKey_Z] = sad::Z;

    io.RenderDrawListsFn = render_draw_lists;      // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
    io.SetClipboardTextFn = set_clipboard_text;
    io.GetClipboardTextFn = get_clipboard_text;
    io.ClipboardUserData = sad::Renderer::ref();
#ifdef _WIN32
    io.ImeWindowHandle = sad::Renderer::ref()->window()->handles()->WND;
#endif

    sad::input::Controls* controls = sad::Renderer::ref()->controls();
    controls->add(*sad::input::ET_Quit, shutdown);
    controls->add(*sad::input::ET_MouseMove, mouse_move_callback);
    controls->add(*sad::input::ET_KeyPress, key_press_callback);
    controls->add(*sad::input::ET_KeyRelease, key_release_callback);
    controls->add(*sad::input::ET_MousePress, mouse_press_callback);
    controls->add(*sad::input::ET_MouseRelease, mouse_release_callback);
    controls->add(*sad::input::ET_MouseWheel, mouse_wheel_callback);
    controls->add(*sad::input::ET_MouseDoubleClick, mouse_double_click_callback);

    sad::pipeline::Process * step = new sad::pipeline::Process(run_imgui_pipeline);
    sad::Renderer::ref()->pipeline()->insertBefore("sad::MouseCursor::renderCursorIfNeedTo", step, "run_imgui_pipeline");
}


