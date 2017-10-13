#ifdef WIN32
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <renderer.h>
#include <log/consoletarget.h>
#include <imgui.h>
#include <window.h>
#include <os/windowhandles.h>
#include <keycodes.h>


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
}

int main(int argc, char** argv)
{
	sad::Renderer* renderer = sad::Renderer::ref();    
	sad::log::ConsoleTarget * consoletarget = new sad::log::ConsoleTarget(
        "{0}: [{1}] {3}{2}{4}", 0, true
    );
    renderer->log()->addTarget(consoletarget);
    renderer->init(sad::Settings(640,480,false));
    renderer->setWindowTitle("ImGui demo");
	init(renderer);
	
    // Set window size to be fixed
    renderer->makeFixedSize();
	renderer->run();
	
	return 0;
}