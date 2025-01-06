#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    float value{0.f};

    quick_imgui::loop(
        "Quick ImGui",
        /* init: */ [&]() {},
        /* loop: */
        [&]() {
            ImGui::ShowDemoWindow(); // This opens a window which shows tons of examples of what you can do with ImGui. You should check it out! Also, you can use the "Item Picker" in the top menu of that demo window: then click on any widget and it will show you the corresponding code directly in your IDE!

            ImGui::Begin("Example");

            ImGui::SliderFloat("My Value", &value, 0.f, 3.f);

            if (ImGui::Button("1", ImVec2{50.f, 50.f}))
            {
                std::cout << "Clicked button 1\n";
            }
            ImGui::SameLine(); // Draw the next ImGui widget on the same line as the previous one. Otherwise it would be below it

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.f, 0.f, 0.f, 1.f}); // Changes the color of all buttons until we call ImGui::PopStyleColor(). There is also ImGuiCol_ButtonActive and ImGuiCol_ButtonHovered
            ImGui::Button("2", ImVec2{50.f, 50.f});

            ImGui::SameLine();
            ImGui::Button("3", ImVec2{50.f, 50.f});
            ImGui::PopStyleColor();

            ImGui::End();
        }
    );
}