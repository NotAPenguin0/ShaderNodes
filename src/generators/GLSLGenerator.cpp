#include "generators/GLSLGenerator.hpp"

#include "GraphVisitor.hpp"
#include "types.hpp"

#include <cassert>

namespace shader_nodes {

namespace {

using namespace std::literals::string_literals;

struct generator {
    ShaderGraph const* graph;
    std::string result;

    void init() {
        // initialize as vertex shader for now
        result = 
        "#version 330 core\n"
        "layout(location = 0) in vec3 iPos;\n"
        "void main() {\n"
        "   gl_Position = vec4(iPos, 1.0);\n"
        "}\n";
    }

    std::string get_variable_name(node_pin_id pid) {
        NodePin const& pin = graph->get_pin(pid);
        return pin.name + std::to_string(pid);
    }

    void operator()(ShaderNode const& node) {
        if (node.type == nodes::input_value) {
            // create variable
            auto const& outputs = node.get_outputs();
            assert(outputs.size() == 1);
            NodePin const& pin = graph->get_pin(outputs[0]);
            std::string pin_name = get_variable_name(outputs[0]);
            std::string_view pin_type = data_type_string(pin.data_type);
            
            result += pin_type.data() + " "s + pin_name + " = "s + pin_type.data() + "();\n";
        }

        if (node.type == nodes::multiply) {
            // grab the "name" of the input pins
            auto const& inputs = node.get_inputs();
            auto const& outputs = node.get_outputs();
            assert(inputs.size() == 2);
            assert(outputs.size() == 1);
            std::string input_name_a = get_variable_name(inputs[0]);
            std::string input_name_b = get_variable_name(inputs[1]);
            std::string output_name = get_variable_name(outputs[0]);
            result += output_name + " = " + input_name_a + " * " + input_name_b + "\n";
        }

        if (node.type == nodes::output_value) {
            auto const& inputs = node.get_inputs();
            assert(inputs.size() == 1);
            std::string input_name = get_variable_name(inputs[0]);
            result += "output: " + input_name + "\n";
        }
    }
};

}

std::string GLSLGenerator::generate(ShaderGraph const& graph) {

    generator generator;
    generator.graph = &graph;

    generator.init();

    visit_graph(graph, generator);

    return generator.result;
}

}