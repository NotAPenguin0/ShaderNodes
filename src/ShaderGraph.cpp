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

void disconnect(NodePin& first, NodePin& second) {
    first.connection = 0;
    second.connection = 0;
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

ShaderNode& ShaderGraph::add_node(node_func func) {
    auto& node = add_node();

    // Obtain preset
    auto const& preset = presets::presets[func];
    // Set node data
    node.description = preset.description;
    node.func = func;
    // Create input and output pins from preset
    for (auto const& in : preset.in_pins) {
        auto& pin = node.add_input_pin();
        pin.data_type = in.data_type;
        pin.name = in.name;
    }
    for (auto const& out : preset.out_pins) {
        auto& pin = node.add_output_pin();
        pin.data_type = out.data_type;
        pin.name = out.name;
    }

    return node;
}

ShaderNode& ShaderGraph::get_node(node_id id) {
    return nodes.at(id);
}

ShaderNode const& ShaderGraph::get_node(node_id id) const {
    return nodes.at(id);
}

void ShaderGraph::delete_node(node_id id) {
    ShaderNode& node = get_node(id);
    // Remove connections for input and output pins
    for(auto pid : node.get_inputs()) {
        NodePin& first = get_pin(pid);
        if (!first.connection) { continue; }
        NodePin& second = get_pin(first.connection);
        disconnect(first, second);
    }

    for(auto pid : node.get_outputs()) {
        NodePin& first = get_pin(pid);
        if (!first.connection) { continue; }
        NodePin& second = get_pin(first.connection);
        disconnect(first, second);
    }
    // erase node from list
    nodes.erase(id);
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