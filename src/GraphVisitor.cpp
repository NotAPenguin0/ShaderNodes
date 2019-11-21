#include "GraphVisitor.hpp"


namespace shader_nodes {

bool is_start_node(ShaderNode const& node) {
    return node.get_inputs().empty();
}

bool is_end_node(ShaderNode const& node) {
    return node.get_outputs().empty();
}

namespace detail {

bool ready_to_advance(visitor_data& data, ShaderNode const& dest, NodePin const& cur_input) {
    auto const& inputs = dest.get_inputs();
    // do not check last node
    for (int i = 0; i < inputs.size(); ++i) {
        // if the pin we're checking is the current input pin, we can skip this
        if (inputs[i] == cur_input.id) { continue; }

        NodePin const& pin = data.graph->get_pin(inputs[i]);
        if (pin.connection) {
            NodePin const& prev_output = data.graph->get_pin(pin.connection);
            ShaderNode const& prev_node = data.graph->get_node(prev_output.owning_node);
            if (!data.visited[prev_node.id]) {
                return false;
            }
        }
    }
    return true;
}

}

}