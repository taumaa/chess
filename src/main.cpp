#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"
#include "Chess.hpp"
#include "glimac/FilePath.hpp"
#include "glimac/Program.hpp"
#include "Shader.hpp"
#include <Camera.hpp>
#include <glm/glm.hpp>
#include "glm/ext/matrix_clip_space.hpp"
#include "Piece.hpp"
#include <glm/gtc/matrix_transform.hpp>
int main()
{
    // Create the chess game
    Chess chess;
    bool shouldExit = false;
    int window_width  = 1280;
    int window_height = 720;

    glmax::Shader shader;
    glmax::Camera camera{true};

    quick_imgui::loop(
        "Chess Game",
        {
            .init = [&]() {
                // Set window size
                
                shader.load_shader("model.vs.glsl", "model.fs.glsl");

                chess.draw_board_3D();
                chess.load_pieces_3D();

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
                    // Check if we should exit
                    if (chess.shouldExitGame()) {
                        shouldExit = true;
                    }


                    glClearColor(0.f, 0.f, 0.f, 1.f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    glEnable(GL_DEPTH_TEST);

                    glm::mat4 projection = glm::perspective(glm::radians(60.0f), static_cast<float>(window_width) / static_cast<float>(window_height), 0.1f, 200.0f);
                    shader.use();

                    //MVP
                    shader.set_uniform_matrix_4fv("model", glm::mat4(1.0f));
                    shader.set_uniform_matrix_4fv("view", camera.get_view_matrix());
                    shader.set_uniform_matrix_4fv("projection", projection);

                    shader.set_uniform_3fv("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
                    shader.set_uniform_3fv("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

                    //CAMERA SETTINGS
                    shader.set_uniform_3fv("viewPos", camera.get_position());

                    chess.render(shader);

                },
                .key_callback             = [](int key, int scancode, int action, int mods) { std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Mods: " << mods << '\n'; },
                .mouse_button_callback    = [](int button, int action, int mods) { std::cout << "Button: " << button << " Action: " << action << " Mods: " << mods << '\n'; },
                .cursor_position_callback = [&](double xpos, double ypos) { camera.track_ball_move_callback(-xpos, -ypos); },
                .scroll_callback          = [&](double xoffset, double yoffset) { camera.process_scroll(yoffset); },
                .window_size_callback     = [](int width, int height) { std::cout << "Resized: " << width << ' ' << height << '\n'; },
        }
    );
}