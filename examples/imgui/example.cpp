#include <renderer.h>
#include <log/consoletarget.h>
#include <imgui.h>

int main(int argc, char** argv)
{
	sad::Renderer* renderer = sad::Renderer::ref();    
	sad::log::ConsoleTarget * consoletarget = new sad::log::ConsoleTarget(
        "{0}: [{1}] {3}{2}{4}", 0, true
    );
    renderer->log()->addTarget(consoletarget);
    renderer->init(sad::Settings(640,480,false));
    renderer->setWindowTitle("ImGui demo");    
    // Set window size to be fixed
    renderer->makeFixedSize();
	renderer->run();
	
	return 0;
}