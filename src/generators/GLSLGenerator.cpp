#include "generators/GLSLGenerator.hpp"

#include "GraphVisitor.hpp"
#include "types.hpp"

#include "generators/GLSLFunctionGenerator.hpp"

#include "mustache/mustache.hpp"

#include <cassert>
#include <sstream>
#include <iostream> // debug

namespace shader_nodes {

namespace {

using namespace std::literals::string_literals;

using namespace kainjow;

// Generator is currently limited to one output pin per function.
// This is because a glsl function cannot define multiple return values.
struct generator {
private:
    std::vector<ShaderNode const*> function_nodes;

    void get_function_nodes() {

        auto grab_functions = [this](ShaderNode const& node) {
            if (is_function(node.func)) {
                // Make sure to add each functional node only once
                for (auto const& n : function_nodes) {
                    if (n->func == node.func) {
                        return;
                    }
                }
                function_nodes.push_back(&node);
            }
        };
        
        // Execute the lambda for each node in the graph
        visit_graph(*graph, grab_functions);
    }

    void create_functions() {
        // prototypes
        for (auto const& node : function_nodes) {
            std::string prototype = function_prototype(*node);
            writer << prototype << "\n";
        }
        // function body
        for (auto const& node : function_nodes) {
            std::string impl = function_body(*node);
            writer << impl << "\n";
        }
    }

public:
    ShaderGraph const* graph;
    std::ostringstream writer;

    std::vector<node_id> shader_outputs;

    // Assumes graph pointer has been set already
    void init() {
//        get_function_nodes();
//        create_functions();
    }

    std::string get_variable_name(node_pin_id pid) {
        NodePin const& pin = graph->get_pin(pid);
        return pin.name + std::to_string(pid);
    }

    void gen_binary_op(ShaderNode const& node) {
            // 1. Get input pins
            auto const& inputs = node.get_inputs();            
            // Verify input size. A binary operation must always have exactly two inputs
            assert(inputs.size() == 2);
            NodePin const& lhs_input = graph->get_pin(inputs[0]);
            NodePin const& rhs_input = graph->get_pin(inputs[1]);

            // 2. Get output pins connecting to these inputs
            NodePin const& lhs = graph->get_pin(lhs_input.connection);
            NodePin const& rhs = graph->get_pin(rhs_input.connection);

            // 3. The names of these outputs will be the input names
            std::string lhs_name = get_variable_name(lhs.id);
            std::string rhs_name = get_variable_name(rhs.id);
            // 4. Get output pin name
            auto const& outputs = node.get_outputs();
            // Verify output size
            assert(outputs.size() == 1);
            NodePin const& out_pin = graph->get_pin(outputs[0]);
            std::string out_name = get_variable_name(out_pin.id);
            // 5. Make binary operation
            std::string out_type = data_type_string(out_pin.data_type).data();
            
            char op_token = binary_op_token(node.func);

            writer << "\t";
            writer << out_type << " " << out_name << " = ";
            writer << lhs_name << " " << op_token << " " << rhs_name << ";\n";
    }

    void write_as_float(std::ostringstream& oss, NodeValue const& value) {
        float val = std::get<float>(value.value);
        oss << val;
    }

    void write_as_float2(std::ostringstream& oss, NodeValue const& value) {
        float2 val = std::get<float2>(value.value);
        oss << val.x << ", " << val.y;
    }

    void write_as_float3(std::ostringstream& oss, NodeValue const& value) {
        float3 val = std::get<float3>(value.value);
        oss << val.x << ", " << val.y << ", " << val.z;
    }

    void write_as_float4(std::ostringstream& oss, NodeValue const& value) {
        float4 val = std::get<float4>(value.value);
        oss << val.x << ", " << val.y << ", " << val.z << ", " << val.w;
    }

    std::string get_value_string(ShaderNode const& node) {
        auto const& outputs = node.get_outputs();
        NodePin const& out_pin = graph->get_pin(outputs[0]);
        DataType const type = out_pin.data_type;

        std::ostringstream out;

        switch(type) {
            case DataType::Float:  
                write_as_float(out, node.value);
                break;
            case DataType::Float2:
                write_as_float2(out, node.value);
                break;
            case DataType::Float3:
                write_as_float3(out, node.value);
                break;
            case DataType::Float4:
                write_as_float4(out, node.value);
                break;
            default:
                out << "0.0";
        }

        return out.str();
    }

    void gen_constant_decl(ShaderNode const& node) {
        // get outputs, verify that size is 1
        auto const& outputs = node.get_outputs();
        assert(outputs.size() == 1);
        // Get output pin. We need this to know the name of the variable to generate
        NodePin const& pin = graph->get_pin(outputs[0]);
        std::string var_name = get_variable_name(outputs[0]);
        std::string_view var_type = data_type_string(pin.data_type);

        std::string value_str = get_value_string(node);

        writer << "\t";
        writer << var_type << " " << var_name << " = ";
        writer << var_type << "(" << value_str << ");\n";
    }

    void gen_builtin_output(NodePin const& pin) {
        node_pin_id input_pid = pin.connection;
        std::string input_name = get_variable_name(input_pid);

        writer << "\t";
        writer << pin.name << " = " << input_name << ";\n";
    }

    void gen_stage_output(ShaderNode const& node) {
            shader_outputs.push_back(node.id);
            auto const& inputs = node.get_inputs();
            assert(inputs.size() == 1);
            NodePin const& pin = graph->get_pin(inputs[0]);
            writer << "\tvs_out." << pin.name << " = "
                   << get_variable_name(pin.connection) << ";\n";
    }

    void operator()(ShaderNode const& node) {
        if (node.func == node_func::constant) {
            gen_constant_decl(node);
        } else if (is_binary_op(node.func)) { 
            gen_binary_op(node);
        } else if (node.func == node_func::output_value) {
            gen_stage_output(node);
        } else if (node.func == node_func::builtin_vars) {
            // no need to generate any code for these, as they are
            // declared automatically
        } else if (node.func == node_func::builtin_out) {
            // Write output
            auto const& inputs = node.get_inputs();
            for (auto pid : inputs) {
                gen_builtin_output(graph->get_pin(pid));
            }
        }
    }

    void add_shader_out_data(mustache::data& data) {
        data["OutVars"] = mustache::data::type::list;
        for (auto out_id : shader_outputs) {
            ShaderNode const& node = graph->get_node(out_id);
            auto const& inputs = node.get_inputs();
            NodePin const& pin = graph->get_pin(inputs[0]);
            std::string decl = 
                "\t"s + data_type_string(pin.data_type).data() + " "s + pin.name + ";";
            data["OutVars"].push_back(mustache::data("Decl", decl));
        }
    }

    std::string finalize() {
        static const std::string mustache_base = 
        "#version 330 core\n"
        "layout(location = 0) in vec3 iPos;\n"
        "out VS_OUT {\n"
        "{{#OutVars}}"
        "{{{Decl}}}"
        "{{/OutVars}}\n"
        "} vs_out;\n\n"
        "void main() {\n"
        "{{{Main}}}\n"
        "}\n";

        mustache::mustache must(mustache_base);
        mustache::data data = mustache::data::type::object;
        // write data for main
        data["Main"] = writer.str();
        
        add_shader_out_data(data);

        return must.render(data);
    }
};

}

std::string GLSLGenerator::generate(ShaderGraph const& graph) {

    generator generator;
    generator.graph = &graph;

    generator.init();

    visit_graph(graph, generator);

    return generator.finalize();
}

} // namespace shader_nodes