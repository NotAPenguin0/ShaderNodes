#ifndef SHADER_NODES_NODES_HPP_
#define SHADER_NODES_NODES_HPP_

#include "types.hpp"

#include <string_view>

namespace shader_nodes {
namespace nodes {

inline constexpr node_func constant = 0;
inline constexpr node_func output_value = 1;
// Writeable built-in variables like gl_Position.
///A node with this function has input pins for these built-in variables.
inline constexpr node_func builtin_out = 2;
// built in variables like gl_InstanceID.
// A node with this function has output pins for these built-in variables.
inline constexpr node_func builtin_vars = 3;

inline constexpr node_func add = 4;
inline constexpr node_func subtract = 5;
inline constexpr node_func multiply = 6;
inline constexpr node_func divide = 7;

}

bool is_function(node_func node_t);
bool is_binary_op(node_func node_t);
char binary_op_token(node_func op);

std::string_view node_func_string(node_func const node_t);

}

#endif