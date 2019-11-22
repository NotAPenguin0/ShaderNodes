#include <iostream>
#include <vector>
#include <string>

 #include "ShaderGraph.hpp"
 #include "GraphVisitor.hpp"
 #include "generators/GLSLGenerator.hpp"
 #include "generators/FunctionImplDatabase.hpp"

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
    shader_nodes::initialize_function_impl_database();
    shader_nodes::ShaderGraph graph;

    auto& end_node = graph.add_node();
    auto& merge_node = graph.add_node();
    auto& begin_node = graph.add_node();
    auto& begin_node2 = graph.add_node();
    begin_node.description = "Begin";
    begin_node.func = shader_nodes::nodes::constant;
    begin_node.value = 5.0;
    begin_node2.description = "Begin2";
    begin_node2.func = shader_nodes::nodes::constant;  
    begin_node2.value = 9.0;
    merge_node.description = "Mulitplies two values";
    merge_node.func = shader_nodes::nodes::multiply;
    end_node.description = "End";
    end_node.func = shader_nodes::nodes::output_value;

    auto& begin_out = begin_node.add_output_pin();
    begin_out.name = "input";
    begin_out.data_type = shader_nodes::DataType::Float;
    auto& begin2_out = begin_node2.add_output_pin();
    begin2_out.name = "input";
    begin2_out.data_type = shader_nodes::DataType::Float;
    auto& merge_in1 = merge_node.add_input_pin();
    merge_in1.name = "a";
    merge_in1.data_type = shader_nodes::DataType::Float;
    auto& merge_in2 = merge_node.add_input_pin();
    merge_in2.name = "b";
    merge_in2.data_type = shader_nodes::DataType::Float;
    auto& merge_out = merge_node.add_output_pin();
    merge_out.data_type = shader_nodes::DataType::Float;
    merge_out.name = "mult_result";
    auto& end_in = end_node.add_input_pin();
    end_in.name = "output";

    shader_nodes::connect(begin_out, merge_in1);
    shader_nodes::connect(begin2_out, merge_in2);
    shader_nodes::connect(merge_out, end_in);

    shader_nodes::GLSLGenerator generator;
    std::cout << generator.generate(graph);
}