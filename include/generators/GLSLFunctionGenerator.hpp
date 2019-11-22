#ifndef SHADER_NODES_GLSL_FUNCTION_GENERATOR_HPP_
#define SHADER_NODES_GLSL_FUNCTION_GENERATOR_HPP_

#include "ShaderGraph.hpp"

#include <string>

namespace shader_nodes {

DataType get_return_type(ShaderNode const& function_node);

std::string function_prototype(ShaderNode const& function_node);
std::string function_body(ShaderNode const& function_node);

}

#endif