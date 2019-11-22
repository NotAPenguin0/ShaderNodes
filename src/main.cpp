#include <iostream>
#include <vector>
#include <string>

 #include "ShaderGraph.hpp"
 #include "GraphVisitor.hpp"
 #include "generators/SPIRVGenerator.hpp"

std::ostream& operator<<(std::ostream& out, shader_nodes::ShaderNode const& node) {

    out << "Node description:\n" << node.description << "\n";
    out << "Input pins:\n";
    for(auto pin_id : node.get_inputs()) {
        auto const& pin = node.get_graph().get_pin(pin_id);
        out << pin.name << "\n";
    }
    out << "Output pins:\n";
    for(auto pin_id : node.get_outputs()) {
        auto const& pin = node.get_graph().get_pin(pin_id);
        out << pin.name << "\n";
    }

    return out;
}

struct graph_printer {
    shader_nodes::ShaderGraph const& graph;
    void operator()(shader_nodes::ShaderNode const& node) {
        std::cout << node << std::endl << std::endl;
    }
};


int main() {
    shader_nodes::ShaderGraph graph;

    auto& end_node = graph.add_node();
    auto& merge_node = graph.add_node();
    auto& begin_node = graph.add_node();
    begin_node.description = "Begin";
    begin_node.type = shader_nodes::nodes::input_value;
    auto& begin_node2 = graph.add_node();
    begin_node2.description = "Begin2";
    begin_node2.type = shader_nodes::nodes::input_value;  
    merge_node.description = "Mulitplies two values";
    merge_node.type = shader_nodes::nodes::multiply;
    end_node.description = "End";
    end_node.type = shader_nodes::nodes::output_value;

    auto& begin_out = begin_node.add_output_pin();
    begin_out.name = "input";
    auto& begin2_out = begin_node2.add_output_pin();
    begin2_out.name = "input";
    auto& merge_in1 = merge_node.add_input_pin();
    merge_in1.name = "a";
    auto& merge_in2 = merge_node.add_input_pin();
    merge_in2.name = "b";
    auto& merge_out = merge_node.add_output_pin();
    merge_out.name = "mult_result";
    auto& end_in = end_node.add_input_pin();
    end_in.name = "output";

    shader_nodes::connect(begin_out, merge_in1);
    shader_nodes::connect(begin2_out, merge_in2);
    shader_nodes::connect(merge_out, end_in);

    shader_nodes::SPIRVGenerator generator;
    std::cout << generator.generate(graph);
}