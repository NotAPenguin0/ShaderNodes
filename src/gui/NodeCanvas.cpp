#include "gui/NodeCanvas.hpp"

#include <imgui_node_editor.h>
#include <limits>
#include <algorithm>

namespace ed = ax::NodeEditor;

namespace {
    int next_link_id = 100000;
}

namespace shader_nodes::gui {

// Nodes have a NodeEditor id of their own ID, 
// pins have a NodeEditor id of ID_MAX - their id.

int node_to_gui_id(node_id id) {
    return id;
}

int pin_to_gui_id(node_pin_id id) { 
    return std::numeric_limits<int>::max() - id;
}

node_id node_from_gui_id(int id) {
    return id;
}

node_pin_id pin_from_gui_id(int id) {
    return std::numeric_limits<int>::max() - id;
}

void NodeCanvas::show(ShaderGraph& graph) {
    if (ImGui::Begin("Node Canvas", &shown)) {

        ed::Begin("ShaderNode Editor");

        show_nodes(graph);

        ed::End();
    }

    ImGui::End();
}

void NodeCanvas::hide() {
    shown = false;
}

void NodeCanvas::show_nodes(ShaderGraph& graph) {
    for(auto const&[id, node] : graph.get_nodes()) {
        int ed_id = node_to_gui_id(id);
        ed::BeginNode(ed_id);
            ImGui::Text("%s", node.description.c_str());
            // input pins
            for (auto pid : node.get_inputs()) {
                ed::BeginPin(pin_to_gui_id(pid), ed::PinKind::Input);
                    NodePin const& pin = graph.get_pin(pid);
                    ImGui::Text("-> %s", pin.name.c_str());
                ed::EndPin();
            }
            ImGui::SameLine();
            // output pins
            for (auto pid : node.get_outputs()) {
                ed::BeginPin(pin_to_gui_id(pid), ed::PinKind::Output);
                    NodePin const& pin = graph.get_pin(pid);
                    ImGui::Text("%s ->", pin.name.c_str());
                ed::EndPin();
            }

        ed::EndNode();
    }

    // Submit links
    for (auto const& link : links) {
        ed::Link(link.Id, link.input_id, link.output_id);
    }

    handle_editor_actions(graph);
}  

void NodeCanvas::handle_editor_actions(ShaderGraph& graph) {
    if (ed::BeginCreate())
    {
        ed::PinId input_id, output_id;
        if (ed::QueryNewLink(&input_id, &output_id))
        {
            // Validate ID's
            if (input_id && output_id)
            {
                if (ed::AcceptNewItem())
                {
                    links.push_back({ ed::LinkId(next_link_id++), input_id, output_id });

                    // Add link to graph
                    connect(
                        graph.get_pin(pin_from_gui_id((size_t)input_id)), 
                        graph.get_pin(pin_from_gui_id((size_t)output_id))
                    );
                    // Draw new link.
                    ed::Link(links.back().Id, links.back().input_id, links.back().output_id);
                }
            }
        }
    }
    ed::EndCreate(); 


    // Handle deletion action
    if (ed::BeginDelete())
    {
        ed::LinkId deleted_id;
        while (ed::QueryDeletedLink(&deleted_id))
        {
            if (ed::AcceptDeletedItem())
            {
                for (auto& link : links)
                {
                    if (link.Id == deleted_id)
                    {
                        links.erase(std::remove(links.begin(), links.end(), link), 
                                    links.end());
                        disconnect(
                            graph.get_pin(pin_from_gui_id((size_t)link.input_id)), 
                            graph.get_pin(pin_from_gui_id((size_t)link.output_id))
                        );
                        break;
                    }
                }
            }

        }
    }
    ed::EndDelete();
}

}