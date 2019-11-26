#ifndef SHADER_NODES_NODE_CANVAS_HPP_
#define SHADER_NODES_NODE_CANVAS_HPP_

#include "ShaderGraph.hpp"

#include <vector>

#include <imgui_node_editor.h>
#include <imgui/imgui.h>

namespace ed = ax::NodeEditor;

namespace shader_nodes::gui {

class NodeCanvas {
public:
    void init();

    void show(ShaderGraph& graph);
    void hide();

private:
    bool shown = true;

    struct LinkInfo
    {
        ed::LinkId id;
        ed::PinId input_id;
        ed::PinId output_id;

        inline bool operator==(LinkInfo const& rhs) {
            return id == rhs.id;
        }
    };

    std::vector<LinkInfo> links;

    ImTextureID node_header;

    void show_nodes(ShaderGraph& graph);
    void handle_editor_actions(ShaderGraph& graph);
};

}

#endif