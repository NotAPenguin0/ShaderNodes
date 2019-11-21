#include <iostream>
#include <vector>
#include <string>

enum class Type {
    Vec1,
    Vec2, 
    Vec3, 
    Vec4,
    Texture
    // etc
};

struct NodePin;
using NodePinRef = NodePin*;

struct ShaderNode;

struct NodePin {
    ShaderNode* owning_node;

    Type data_type;
    std::string name;
    // For an input pin, this will point to the pin this is coming from.
    // For an output pin, this will point to the pin it is going to.
    NodePinRef connection = nullptr;
    // or eventually a std::vector<NodePinRef> for multiple connections.          
    // We can support this later
};

struct ShaderNode {
    std::vector<NodePin> inputs;
    std::vector<NodePin> outputs;
    std::string description;
    // etc
};

std::ostream& operator<<(std::ostream& out, ShaderNode const& node) {
    out << "Node description:\n" << node.description << "\n";
    out << "---------------\n";
    out << "Input pins:\n";
    for(auto const& pin : node.inputs) {
        out << pin.name << "\n";
    }
    out << "----------------\n";
    out << "Output pins:\n";
    for(auto const& pin : node.outputs) {
        out << pin.name << "\n";
    }

    return out;
}

class ShaderGraph {
public: 
    // should be private of course
    std::vector<ShaderNode> nodes;
};

// A node is a starting node if it has no inputs
bool is_start_node(ShaderNode const& node) {
    
    return node.inputs.empty();
}

// A node is an ending node if it has no outputs
bool is_end_node(ShaderNode const& node) {
    return node.outputs.empty();
}

template<typename F>
void visit_node_and_children(ShaderNode const& node, F&& f) {
    // visit current node
    f(node);
    // Note that we don't need to check if it's an ending node, as this loop will 
    // not execute then anyway
    for (auto const& out_pin : node.outputs) {
        // Only continue visiting it the output pin has a connection
        if (out_pin.connection) {
            NodePin const& next_input = *out_pin.connection;
            ShaderNode const& next_node = *next_input.owning_node;
            visit_node_and_children(next_node, std::forward<F>(f));
        }
    }
}

// Visit all nodes in order from start to end. A node is considered a start node if it has
// no input pins, and an end node if it has no output pins
template<typename F>
void visit_graph(ShaderGraph const& graph, F&& f) {
    // Loop over all nodes to find start nodes
    for (auto const& node : graph.nodes) {
        if (is_start_node(node)) {
            // start visiting children of this node
            visit_node_and_children(node, std::forward<F>(f));
        }
    }

}

struct graph_printer {
    void operator()(ShaderNode const& node) {
        std::cout << node << "\n";
    }
};

int main() {
    ShaderGraph graph;
    graph.nodes.reserve(16);
    graph.nodes.emplace_back();
    // The starting node
    ShaderNode& node = graph.nodes.back();
    node.description = "Start node";
    NodePin start_output;
    start_output.data_type = Type::Vec1;
    start_output.name = "StartOut";
    start_output.owning_node = &node;
    // A middle node taking one input
    graph.nodes.emplace_back();
    ShaderNode& middle_node = graph.nodes.back();
    middle_node.description = "Middle node";
    NodePin middle_input;
    middle_input.data_type= Type::Vec1;
    middle_input.name = "MidIn";
    middle_input.connection = &start_output;
    middle_input.owning_node = &middle_node;
    middle_node.inputs.push_back(middle_input);
    start_output.connection = &middle_node.inputs.back();
    node.outputs.push_back(start_output);

    NodePin middle_output;
    middle_output.data_type = Type::Vec2;
    middle_output.name = "MiddleOut";
    middle_output.owning_node = &middle_node;
    middle_node.outputs.push_back(middle_output);
    graph.nodes.emplace_back();
    ShaderNode& end_node = graph.nodes.back();
    end_node.description = "End Node";
    end_node.inputs.emplace_back();
    NodePin& end_in = end_node.inputs.back();
    end_in.data_type = Type::Vec2;
    end_in.name = "EndIn";
    end_in.connection = &middle_node.outputs.back();
    end_in.owning_node = &end_node;
    middle_node.outputs.back().connection = &end_in;

    

    visit_graph(graph, graph_printer{});
}