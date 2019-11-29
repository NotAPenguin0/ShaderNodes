#include "gui/NodeList.hpp"

#include "imgui/imgui.h"
#include <string>

#include <iostream>

namespace shader_nodes::gui {

NodeList::NodeList() {
    filter.resize(filter_buf_size);
}

void NodeList::show() {
    if (!shown) return;

    if (ImGui::Begin("Node List", &shown)) {
        ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
        size_t const width = ImGui::GetWindowSize().x - 20;
        ImGui::PushItemWidth(width);
        ImGui::InputTextWithHint("##filter_nodes", "Search ...", filter.data(), filter_buf_size);
        ImGui::PopItemWidth();
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

static bool match_filter(std::string const& str, std::string const& filter) {
    // Everything matches the filter if the filter is empty
    if (filter[0] == '\0') return true;

    size_t const real_len = filter.find_first_of('\0');
    std::string real_filter = filter.substr(0, real_len);
    return str.find(real_filter) != std::string::npos;
}

void NodeList::display_node_func(node_func func) { 
    std::string func_name = node_func_string(func).data();
    // If the node doesn't match the filter, exit early
    if (!match_filter(func_name, filter)) { return; }
    // Display node
    ImGui::Selectable(func_name.c_str());
    // ImGui DragDrop to add a node to the screen
    if (ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload(
           "p_new_node", 
           reinterpret_cast<void*>(&func), sizeof(func));
        ImGui::Text("%s", func_name.c_str());
        ImGui::EndDragDropSource();
    }
}

}