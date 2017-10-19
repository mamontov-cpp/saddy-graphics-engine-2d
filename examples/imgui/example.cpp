#ifdef WIN32
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include <renderer.h>
#include <log/consoletarget.h>
#include <imgui.h>
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


void render_draw_lists(ImDrawData* draw_data)
// Note that this implementation is little overcomplicated because we are saving/setting up/restoring every OpenGL state explicitly, in order to be able to run within any OpenGL engine that doesn't do so. 
// If text or lines are blurry when integrating ImGui in your engine: in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
{
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
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound

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

sad::Hash<sad::Renderer*, sad::String> hash;

static const char* get_clipboard_text(void* user_data)
{
    sad::Renderer* ref = static_cast<sad::Renderer*>(user_data);
    if (hash.contains(ref))
    {
        hash[ref]  = ref->clipboard()->get();
    }
    else
    {
        hash.insert(ref, ref->clipboard()->get());
    }
    return hash[ref].c_str();
}

static void set_clipboard_text(void* user_data, const char* text)
{
    sad::Renderer* ref = static_cast<sad::Renderer*>(user_data);
    ref->clipboard()->set(text);
}

static GLuint       font_texture = 0;

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

static void  invalidate_device_objects()
{
    if (font_texture)
    {
        glDeleteTextures(1, &font_texture);
        ImGui::GetIO().Fonts->TexID = 0;
        font_texture = 0;
    }
}

static void shutdown()
{
    invalidate_device_objects();
    ImGui::Shutdown();
}


void new_frame()
{
    if (!font_texture)
        create_device_objects();
    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    int w, h;
    sad::Rect2I r = sad::Renderer::ref()->window()->rect();
    w = r.width();
    h = r.height();
    
    io.DisplaySize = ImVec2((float)w, (float)h);
    io.DisplayFramebufferScale = ImVec2(1, 1);

    // Setup time step
    io.DeltaTime = 1.0 / sad::Renderer::ref()->fps();

    // Setup inputs
    // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
    if ((sad::Renderer::ref()->window()->hidden() == false) && (sad::Renderer::ref()->window()->minimized() == false))
    {
        if (io.WantMoveMouse)
        {
            sad::Renderer::ref()->setCursorPosition(sad::Point2D((double)io.MousePos.x, (double)io.MousePos.y));
        }
        else
        {
            double mouse_x, mouse_y;
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

    // Start the frame
    ImGui::NewFrame();
}

static void mouse_move_callback(const sad::input::MouseMoveEvent& ev)
{
    sad::Rect2I r = sad::Renderer::ref()->window()->rect();
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2((float)(ev.pos2D().x()), r.height() - (float)(ev.pos2D().y()));
}

static void key_press_callback(const sad::input::KeyPressEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[ev.key()] = true;

    if (ev.ReadableKey.exists())
    {
        printf("Bytes: ");
        for(size_t i = 0; i < ev.ReadableKey.value().size(); i++)
        {
            printf("%d ", ev.ReadableKey.value().c_str()[i]);
        }
        printf("\n");
        io.AddInputCharactersUTF8(ev.ReadableKey.value().c_str());
    }

    io.KeyCtrl = ev.CtrlHeld;
    io.KeyShift = ev.ShiftHeld;
    io.KeyAlt = ev.AltHeld;
    io.KeySuper = false;
}

static void key_release_callback(const sad::input::KeyReleaseEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[ev.key()] = false;


    io.KeyCtrl = ev.CtrlHeld;
    io.KeyShift = ev.ShiftHeld;
    io.KeyAlt = ev.AltHeld;
    io.KeySuper = false;
}

static void mouse_press_callback(const sad::input::MousePressEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    printf("Press\n");
    switch (ev.Button)
    {
        case sad::MouseLeft: { io.MouseDown[0] = true; }
        case sad::MouseMiddle: { io.MouseDown[2] = true; }
        case sad::MouseRight: { io.MouseDown[1] = true; }
    };
    sad::Rect2I r = sad::Renderer::ref()->window()->rect();
    io.MousePos = ImVec2((float)(ev.pos2D().x()), r.height() - (float)(ev.pos2D().y()));
}

static void mouse_double_click_callback(const sad::input::MouseDoubleClickEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    printf("Double click\n");
    switch (ev.Button)
    {
        case sad::MouseLeft: { io.MouseDown[0] = true; }
        case sad::MouseMiddle: { io.MouseDown[2] = true; }
        case sad::MouseRight: { io.MouseDown[1] = true; }
    };
    sad::Rect2I r = sad::Renderer::ref()->window()->rect();
    io.MousePos = ImVec2((float)(ev.pos2D().x()), r.height() - (float)(ev.pos2D().y()));
}

static void mouse_release_callback(const sad::input::MousePressEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    printf("Release\n");

    switch (ev.Button)
    {
        case sad::MouseLeft: { io.MouseDown[0] = false; }
        case sad::MouseMiddle: { io.MouseDown[2] = false; }
        case sad::MouseRight: { io.MouseDown[1] = false; }
    };
    sad::Rect2I r = sad::Renderer::ref()->window()->rect();
    io.MousePos = ImVec2((float)(ev.pos2D().x()), r.height() - (float)(ev.pos2D().y()));
}

static void mouse_wheel_callback(const sad::input::MouseWheelEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheel = ev.Delta;
}


bool show_test_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

static void imgui_callback()
{
    static float f = 0.0f;
    char buf[1000] = "";
    ImGui::Text("Hello, world!");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float*)&clear_color);
    if (ImGui::Button("Test Window")) show_test_window ^= 1;
    if (ImGui::Button("Another Window")) show_another_window ^= 1;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::InputText("string", buf, 256);

    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello from another window!");
        ImGui::End();
    }
    
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
    if (show_test_window)
    {
        ImGui::ShowTestWindow(&show_test_window);
    }
}

static void run_imgui_pipeline()
{
    new_frame();

    imgui_callback();

    ImGui::Render();

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDoubleClicked[0] = io.MouseDoubleClicked[1]  =  io.MouseDoubleClicked[2] = false;
}

void init(sad::Renderer* r)
{
    ImGuiIO& io = ImGui::GetIO();
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
    io.ClipboardUserData = r;
#ifdef _WIN32
    io.ImeWindowHandle = r->window()->handles()->WND;
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
 
    io.Fonts->AddFontFromFileTTF("PTM55F.ttf", 14.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
}

int main(int argc, char** argv)
{
    sad::Renderer* renderer = sad::Renderer::ref();
    sad::log::ConsoleTarget * consoletarget = new sad::log::ConsoleTarget(
        "{0}: [{1}] {3}{2}{4}", 0, true
    );
    renderer->log()->addTarget(consoletarget);
    renderer->init(sad::Settings(1024,768,false));
    renderer->setWindowTitle("ImGui demo");

    // Setup proper matrices
    sad::Scene* s = new sad::Scene();
    renderer->add(s);
    s->setCamera(new sad::OrthographicCamera());
     
    init(renderer);
    
    // Set window size to be fixed
    renderer->makeFixedSize();
    renderer->run();
    
    return 0;
}