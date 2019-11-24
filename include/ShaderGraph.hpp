#ifndef SHADER_NODES_SHADER_GRAPH_HPP_
#define SHADER_NODES_SHADER_GRAPH_HPP_

#include "types.hpp"
#include "NodePin.hpp"
#include "NodeValue.hpp"
#include "Nodes.hpp"
#include "NodePresets.hpp"

#include <unordered_map>
#include <vector>

namespace shader_nodes {

class ShaderGraph;

struct ShaderNode {
    std::string description;

    node_func func;
    node_id id;

    NodeValue value;

    NodePin& add_input_pin();
    NodePin& add_output_pin();

    std::vector<node_pin_id> const& get_inputs() const;
    std::vector<node_pin_id> const& get_outputs() const;

    ShaderGraph& get_graph();
    ShaderGraph const& get_graph() const;

private:
    friend class ShaderGraph;

    std::vector<node_pin_id> inputs;
    std::vector<node_pin_id> outputs;

    ShaderGraph* graph = nullptr;
};

void connect(NodePin& first, NodePin& second);

class ShaderGraph {
public:
    ShaderGraph() = default;
    ShaderGraph(ShaderGraph const& rhs);
    ShaderGraph(ShaderGraph&& rhs);

    ShaderGraph& operator=(ShaderGraph const& rhs);
    ShaderGraph& operator=(ShaderGraph&& rhs);

    ~ShaderGraph() = default;

    ShaderNode& add_node();
    ShaderNode& add_node(node_func const func);

    ShaderNode& get_node(node_id id);
    ShaderNode const& get_node(node_id id) const;

    NodePin& get_pin(node_pin_id id);
    NodePin const& get_pin(node_pin_id id) const;

    std::unordered_map<node_id, ShaderNode> const& get_nodes() const;

private:
    friend struct ShaderNode;

    NodePin& add_pin();

    std::unordered_map<node_id, ShaderNode> nodes;
    std::unordered_map<node_pin_id, NodePin> pins;
};

}

#endif