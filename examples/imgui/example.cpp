#include <imgui/sadimgui.h>

#include "../game/game.h"

int main(int argc, char** argv)
{
    Game* game = new Game(sad::Renderer::ref());
    if (!game->trySetup())
    {
        delete game;
        return 1;
    }
    bool show_test_window = false;
    game->initialize();

    
    sad::imgui::ImGui::init();
    sad::imgui::ImGui::enable();
    sad::imgui::ImGui::add([game,&show_test_window]() -> void {
        ImGui::SetNextWindowSize(ImVec2(130, 130), ImGuiCond_FirstUseEver);
        ImGui::Begin("Cheating board");
        if (game->isInPlayingState())
        {
            if (ImGui::Button("+1 health")) game->increasePlayerHealth(1);
            if (ImGui::Button("+50 score")) game->increasePlayerScore(50);
            if (ImGui::Button("Kill enemies")) game->killEnemies();
        }
        if (ImGui::Button("Test Window")) show_test_window ^= 1;
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(320, 240), ImGuiCond_FirstUseEver);
        if (show_test_window)
        {
            ImGui::ShowDemoWindow(&show_test_window);
        }
    });
    // Initialize example with cyrillic fonts
    ImGui::GetIO().Fonts->AddFontFromFileTTF("./PTM55F.ttf", 14.0f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

    game->run();
    delete game;

    return 0;
}
