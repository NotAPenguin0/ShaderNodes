#ifndef SHADER_NODES_NODES_HPP_
#define SHADER_NODES_NODES_HPP_

#include "types.hpp"

#include <string_view>

namespace shader_nodes {
    
enum class node_func {  
    constant = 0,
    output_value,
    // Writeable built-in variables like gl_Position.
    ///A node with this function has input pins for these built-in variables.
    builtin_out,
    // built in variables like gl_InstanceID.
    // A node with this function has output pins for these built-in variables.
    builtin_vars,

    add,
    subtract,
    multiply,
    divide,

    MAX_NODES
};

bool is_function(node_func node_t);
bool is_binary_op(node_func node_t);
char binary_op_token(node_func op);

std::string_view node_func_string(node_func const node_t);

}

#endif