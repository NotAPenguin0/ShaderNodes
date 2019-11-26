#include <iostream>
#include <vector>
#include <string>

#include "ShaderGraph.hpp"
#include "GraphVisitor.hpp"
#include "GraphVerification.hpp"
#include "generators/GLSLGenerator.hpp"
#include "generators/FunctionImplDatabase.hpp"

#include "gui/Editor.hpp"




int main() {
    shader_nodes::presets::init();

    shader_nodes::initialize_function_impl_database();

    shader_nodes::gui::init();
    shader_nodes::gui::Editor editor;
    editor.run();

    // auto& gl_position = graph.add_node();
    // auto& merge_node = graph.add_node(shader_nodes::node_func::add);
    // auto& begin_node = graph.add_node();
    // auto& begin_node2 = graph.add_node();
    // auto& out_node = graph.add_node();
    // out_node.func = shader_nodes::node_func::output_value;
    // auto& constant = graph.add_node();
    // constant.func = shader_nodes::node_func::constant;
    // constant.value = 42.0;
    // begin_node.description = "Begin";
    // begin_node.func = shader_nodes::node_func::constant;
    // begin_node.value = 5.0;
    // begin_node2.description = "Begin2";
    // begin_node2.func = shader_nodes::node_func::constant;  
    // begin_node2.value = 9.0;
    // gl_position.description = "gl_Position";
    // gl_position.func = shader_nodes::node_func::builtin_out;

    // auto& begin_out = begin_node.add_output_pin();
    // begin_out.name = "input";
    // begin_out.data_type = shader_nodes::DataType::Float;
    // auto& begin2_out = begin_node2.add_output_pin();
    // begin2_out.name = "input";
    // begin2_out.data_type = shader_nodes::DataType::Float;
    // auto& merge_in1 = graph.get_pin(merge_node.get_inputs()[0]);
    // auto& merge_in2 = graph.get_pin(merge_node.get_inputs()[1]);
    // auto& merge_out = graph.get_pin(merge_node.get_outputs()[0]);
    // auto& end_in = gl_position.add_input_pin();
    // end_in.name = "gl_Position";
    // end_in.data_type = shader_nodes::DataType::Float4;

    // auto& out_pin = out_node.add_input_pin();
    // out_pin.name = "out_val";
    // out_pin.data_type = shader_nodes::DataType::Float;
    // auto& const_pin = constant.add_output_pin();
    // const_pin.name = "unset";
    // const_pin.data_type = shader_nodes::DataType::Float;

    // shader_nodes::connect(begin_out, merge_in1);
    // shader_nodes::connect(begin2_out, merge_in2);
    // shader_nodes::connect(merge_out, end_in);
    // shader_nodes::connect(out_pin, const_pin);

    // verify_graph(graph, std::cout);

    // std::cout << "\n\n";
    // std::cout << "Generated GLSL:\n";
    // std::cout << "----------------------\n";

    // shader_nodes::GLSLGenerator generator;
    // std::cout << generator.generate(graph);
}