#include "ShaderGraph.hpp"

namespace shader_nodes {

namespace {

    template<typename T, typename Tag>
    struct id_generator {
        static T cur;

        static T next() {
            return ++cur;
        }
    };

    template<typename T, typename Tag>
    T id_generator<T, Tag>::cur = T{};
}

// Implementation for class: ShaderNode

NodePin& ShaderNode::add_input_pin() {
    auto& pin = graph->add_pin();
    pin.owning_node = id;
    inputs.push_back(pin.id);
    return pin;
}

NodePin& ShaderNode::add_output_pin() {
    auto& pin = graph->add_pin();
    pin.owning_node = id;
    outputs.push_back(pin.id);
    return pin;
}

std::vector<node_pin_id> const& ShaderNode::get_inputs() const {
    return inputs;
}

std::vector<node_pin_id> const& ShaderNode::get_outputs() const {
    return outputs;
}

ShaderGraph& ShaderNode::get_graph() {
    return *graph;
}

ShaderGraph const& ShaderNode::get_graph() const {
    return *graph;
}

void connect(NodePin& first, NodePin& second) {
    first.connection = second.id;
    second.connection = first.id;
}

// Implementation for class: ShaderGraph

namespace {

struct NodeTag {};
struct PinTag {};

}

ShaderGraph::ShaderGraph(ShaderGraph const& rhs) {
    nodes.clear();
    pins.clear();

    for (auto const& [id, node] : rhs.nodes) {
        // insert returns a pair<iterator, bool>
        auto node_it = nodes.insert({id, node}).first;
        ShaderNode& my_node = node_it->second;
        my_node.graph = this;
    }

    pins = rhs.pins;
}

ShaderGraph::ShaderGraph(ShaderGraph&& rhs) {

    nodes = std::move(rhs.nodes);
    pins = std::move(rhs.pins);

    for (auto& [id, node] : nodes) {
        // set graph for moved nodes
        node.graph = this;
    }
}

ShaderGraph& ShaderGraph::operator=(ShaderGraph const& rhs) {

    if (this != &rhs) {
        nodes.clear();
        pins.clear();

        for (auto const& [id, node] : rhs.nodes) {
            // insert returns a pair<iterator, bool>
            auto node_it = nodes.insert({id, node}).first;
            ShaderNode& my_node = node_it->second;
            my_node.graph = this;
        }

        pins = rhs.pins;
    }

    return *this;
}

ShaderGraph& ShaderGraph::operator=(ShaderGraph&& rhs) {

    if (this != &rhs) {
        nodes = std::move(rhs.nodes);
        pins = std::move(rhs.pins);

        for (auto& [id, node] : nodes) {
            // set graph for moved nodes
            node.graph = this;
        }
    }

    return *this;
}

ShaderNode& ShaderGraph::add_node() {
    node_id id = id_generator<node_id, NodeTag>::next();
    // Create a new node. This is abusing the fact that unordered_map's operator[] 
    // creates a new entry when an invalid key is supplied
    auto& node = nodes[id];
    node.id = id;
    node.graph = this;
    return node;
}

ShaderNode& ShaderGraph::get_node(node_id id) {
    return nodes.at(id);
}

ShaderNode const& ShaderGraph::get_node(node_id id) const {
    return nodes.at(id);
}

NodePin& ShaderGraph::get_pin(node_pin_id id) {
    return pins.at(id);
}

NodePin const& ShaderGraph::get_pin(node_pin_id id) const {
    return pins.at(id);
}

std::unordered_map<node_id, ShaderNode> const& ShaderGraph::get_nodes() const {
    return nodes;
}

NodePin& ShaderGraph::add_pin() {
    node_pin_id id = id_generator<node_pin_id, PinTag>::next();
    // Same principle as add_node
    auto& pin = pins[id];
    pin.id = id;
    return pin;
}

}