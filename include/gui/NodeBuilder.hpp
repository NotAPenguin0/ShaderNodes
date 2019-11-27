#ifndef SHADER_NODES_NODE_BUILDER_HPP_
#define SHADER_NODES_NODE_BUILDER_HPP_

#include "ShaderGraph.hpp"

#include <limits>
#include <vector>
#include <type_traits>
#include <string>
#include <string_view>

#include <imgui/imgui.h>

namespace shader_nodes::gui {

// Nodes have a NodeEditor id of their own ID, 
// pins have a NodeEditor id of ID_MAX - their id.

inline int node_to_gui_id(node_id id) {
    return id;
}

inline int pin_to_gui_id(node_pin_id id) { 
    return std::numeric_limits<int>::max() - id;
}

inline node_id node_from_gui_id(int id) {
    return id;
}

inline node_pin_id pin_from_gui_id(int id) {
    return std::numeric_limits<int>::max() - id;
}

namespace detail {

enum class NodeSection {
    // Bar across the whole width of the node, displayed at the top of 
    // the node
    Top = 1,
    // Area just below the top bar, spanning the whole width of the node.
    Title = 2,
    // This is typically where you would see input pins go
    Left = 4,
    // This is the area between the left and right sections
    Middle = 8,
    // Typically, the output pins go here
    Right = 16,
    // Bar across the whole width of the node, displayed at the bottom of
    // the node
    Bottom = 32
};

struct NodeDisplayData {
    ImTextureID header_texture;
    ImVec2 header_texture_size;
    ImVec4 header_color;
    float header_height;

    std::string title_text;

    std::vector<node_pin_id> input_pins;
    std::vector<node_pin_id> output_pins;
};

}

struct HeaderTexture {
    ImTextureID texture;
    ImVec2 size;
};

class NodeBuilder {
public:
    NodeBuilder(ShaderNode const& node);
    ~NodeBuilder();

    NodeBuilder& header(HeaderTexture texture, float height, ImVec4 color = {255, 255, 255, 255});
    NodeBuilder& title(std::string_view text);
    NodeBuilder& input_pins(std::vector<node_pin_id> pins);
    NodeBuilder& output_pins(std::vector<node_pin_id> pins);

    // Call when you want to submit the current node to ImGui
    void render();

private:
    using mask_t = std::underlying_type_t<detail::NodeSection>;
    ShaderNode const& node;
    ShaderGraph const& graph;
    detail::NodeDisplayData display_data;
    mask_t set_sections = 0;

    bool has_section(detail::NodeSection section) const;

    void render_header();
};

}

#endif