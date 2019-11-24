#include "gui/NodeCanvas.hpp"

#include <imgui_node_editor.h>

namespace ed = ax::NodeEditor;

namespace shader_nodes::gui {

void NodeCanvas::show() {
    if (ImGui::Begin("Node Canvas", &shown)) {

        ed::Begin("ShaderNode Editor");

        int uniqueId = 1;

        // Start drawing nodes.
        ed::BeginNode(uniqueId++);
            ImGui::Text("Node A");
            ed::BeginPin(uniqueId++, ed::PinKind::Input);
                ImGui::Text("-> In");
            ed::EndPin();
            ImGui::SameLine();
            ed::BeginPin(uniqueId++, ed::PinKind::Output);
                ImGui::Text("Out ->");
            ed::EndPin();
        ed::EndNode();

        ed::BeginNode(uniqueId++);
            ImGui::Text("Node B");
            ed::BeginPin(uniqueId++, ed::PinKind::Input);
                ImGui::Text("-> In2");
            ed::EndPin();
        ed::EndNode();

        ed::End();


        if (ed::BeginCreate()) {


        }
        ed::EndCreate();
        if (ed::BeginDelete()) {

        }
        ed::EndDelete();

    }

    ImGui::End();
}

void NodeCanvas::hide() {
    shown = false;
}

}