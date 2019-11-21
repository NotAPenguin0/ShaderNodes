#ifndef SHADER_NODES_GRAPH_VISITOR_HPP_
#define SHADER_NODES_GRAPH_VISITOR_HPP_

#include <functional>

#include "ShaderGraph.hpp"

namespace shader_nodes {


// A node is a starting node if it has no inputs
bool is_start_node(ShaderNode const& node);

// A node is an ending node if it has no outputs
bool is_end_node(ShaderNode const& node);

namespace detail {

struct visitor_data {
    std::unordered_map<node_id, bool> visited;
    ShaderGraph const* graph;
};

bool ready_to_advance(visitor_data& data, ShaderNode const& dest, NodePin const& cur_input);

template<typename F>
void visit_node_and_children(visitor_data& data, ShaderNode const& node, F&& f) {
    // exit if node has been visited already
    if (data.visited[node.id]) { return; }

    ShaderGraph const& graph = *data.graph;
    // visit current node
    f(node);
    data.visited[node.id] = true;
    // Note that we don't need to check if it's an ending node, as this loop will 
    // not execute then anyway
    for (auto out_pin_id : node.get_outputs()) {
        NodePin const& out_pin = graph.get_pin(out_pin_id);
        // Only visit next child if the output pin has a connection
        if (out_pin.connection) {
            NodePin const& next_input = graph.get_pin(out_pin.connection);
            ShaderNode const& next_node = graph.get_node(next_input.owning_node);

            // Make sure all other input nodes to this node have been evaulated already 
            // before processing it.
            if (!ready_to_advance(data, next_node, next_input)) {
                continue;
            }
            visit_node_and_children(data, next_node, std::forward<F>(f));
        }
    }
}

}

// Visit all nodes in order from start to end. A node is considered a start node if it has
// no input pins, and an end node if it has no output pins
template<typename F>
void visit_graph(ShaderGraph const& graph, F&& f) {
    detail::visitor_data data;
    data.graph = &graph;
    // Loop over all nodes to find start nodes
    for (auto const& [id, node] : graph.get_nodes()) {
        if (is_start_node(node)) {
            // start visiting children of this node
            detail::visit_node_and_children(data, node, std::forward<F>(f));
        }
    }
}


}

#endif