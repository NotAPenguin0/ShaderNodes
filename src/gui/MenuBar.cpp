#include "gui/MenuBar.hpp"

#include "imgui/imgui.h"

namespace shader_nodes::gui {

void MenuBar::show() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("View")) {
            

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

}