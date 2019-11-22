#include "generators/GLSLFunctionGenerator.hpp"
#include "generators/FunctionImplDatabase.hpp"

#include "Nodes.hpp"

#include <cassert>
#include <sstream>

namespace shader_nodes {

DataType get_return_type(ShaderNode const& function_node) {
    // Require that the node has at least one output. Only the first one 
    // will be used though.
    assert(!function_node.get_outputs().empty());
    ShaderGraph const& graph = function_node.get_graph();
    node_pin_id const pid = function_node.get_outputs()[0];
    NodePin const& out_pin = graph.get_pin(pid);

    return out_pin.data_type;
}

std::string function_prototype(ShaderNode const& function_node) {
    ShaderGraph const& graph = function_node.get_graph();

    DataType ret_type = get_return_type(function_node);
    node_func type = function_node.func;

    struct FunctionParameter {
        DataType type;
        std::string name;
    };

    std::vector<FunctionParameter> params;
    // reserve memory for inputs, slight optimization
    params.reserve(function_node.get_inputs().size());

    for (auto pid : function_node.get_inputs()) {
        NodePin const& pin = graph.get_pin(pid);
        params.push_back({pin.data_type, pin.name});
    }

    std::ostringstream writer;
    writer << data_type_string(ret_type) << " ";
    writer << node_func_string(type) << "(";

    for (int i = 0; i < params.size(); ++i) {
        FunctionParameter const& param = params[i];
        writer << data_type_string(param.type) << " ";
        writer << param.name;
        // only write comma for next parameter if this is not the last one
        if (i != params.size() - 1) {
            writer << ", ";
        }
    }

    writer << ");";

    return writer.str();
}

std::string function_body(ShaderNode const& node) {
    std::string prototype = function_prototype(node);
    // remove trailing semicolon from prototype
    prototype.pop_back();
    
    std::ostringstream writer;
    writer << prototype << " {\n";

    writer << get_function_impl(node.func) << "\n";

    // close block
    writer << "}";
    return writer.str();
}

} // namespace shader_nodes