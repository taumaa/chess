#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"
#include "Chess.hpp"

int main()
{
    // Create the chess game
    Chess chess;
    bool shouldExit = false;



    quick_imgui::loop(
        "Chess Game",
        {
            .init = [&]() {
                // Set window size
                ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);
            },
            .loop =
                [&]() {
                    // Exit if requested
                    if (shouldExit) {
                        std::exit(0); // Force exit
                    }
                    
                    // Check for keyboard shortcuts
                    if (ImGui::IsKeyPressed(ImGuiKey_N) && ImGui::GetIO().KeyCtrl) {
                        chess.resetGame();
                    }
                    if (ImGui::IsKeyPressed(ImGuiKey_F11)) {
                        chess.toggleFullscreen();
                    }
                    if (ImGui::IsKeyPressed(ImGuiKey_Escape) && chess.isFullscreen()) {
                        chess.toggleFullscreen();
                    }
                    
                    // Draw the chess board
                    chess.draw();

                    // Créer une fenêtre vide nommée "Cube"
                    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
                    ImGui::SetNextWindowPos(ImVec2(650, 50), ImGuiCond_FirstUseEver);
                    if (ImGui::Begin("Cube")) {
                        // Cette fenêtre est intentionnellement laissée vide
                        ImGui::End();
                    }
                    
                    // Check if we should exit
                    if (chess.shouldExitGame()) {
                        shouldExit = true;
                    }
                },
                .key_callback             = [](int key, int scancode, int action, int mods) { std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Mods: " << mods << '\n'; },
                .mouse_button_callback    = [](int button, int action, int mods) { std::cout << "Button: " << button << " Action: " << action << " Mods: " << mods << '\n'; },
                .cursor_position_callback = [](double xpos, double ypos) { std::cout << "Position: " << xpos << ' ' << ypos << '\n'; },
                .scroll_callback          = [](double xoffset, double yoffset) { std::cout << "Scroll: " << xoffset << ' ' << yoffset << '\n'; },
                .window_size_callback     = [](int width, int height) { std::cout << "Resized: " << width << ' ' << height << '\n'; },
        }
    );
}