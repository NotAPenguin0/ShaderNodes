#include "gui/NodeBuilder.hpp"

#include <imgui_node_editor.h>

#include <cassert>

namespace ed = ax::NodeEditor;

namespace shader_nodes::gui {

using namespace detail;

NodeBuilder::NodeBuilder(ShaderNode const& node) : node(node) {
    ed::BeginNode(node_to_gui_id(node.id));
}

NodeBuilder::~NodeBuilder() {
    ed::EndNode();
}

NodeBuilder& NodeBuilder::input_pins(std::vector<node_pin_id> const& pins) {
    assert(!(set_sections & static_cast<mask_t>(NodeSection::Left)) 
            && "Input pins already set!");

    

    return *this;
}

}