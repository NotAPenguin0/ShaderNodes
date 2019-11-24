#include "gui/NodeList.hpp"

#include "imgui/imgui.h"
#include <string>

#include <iostream>

namespace shader_nodes::gui {

void NodeList::show() {
    if (!shown) return;

    if (ImGui::Begin("Node List", &shown)) {

        // Loop over all nodes
        constexpr size_t max_nodes = static_cast<size_t>(node_func::MAX_NODES);
        for (size_t i = 0; i < max_nodes; ++i) {
            // Get enum value of node function
            node_func func = static_cast<node_func>(i);
            display_node_func(func);
        }

    }

    ImGui::End();
}

void NodeList::hide() {
    shown = false;
}

void NodeList::display_node_func(node_func func) { 
    std::string func_name = node_func_string(func).data();
    ImGui::Selectable(func_name.c_str());
}

}