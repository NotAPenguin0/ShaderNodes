#ifndef SHADER_NODES_NODES_HPP_
#define SHADER_NODES_NODES_HPP_

#include "types.hpp"

#include <string_view>

namespace shader_nodes {
namespace nodes {

inline const node_func constant = 0;
inline const node_func multiply = 1;
inline const node_func output_value = 2;

}

bool is_function(node_func node_t);
bool is_binary_op(node_func node_t);
char binary_op_token(node_func op);

std::string_view node_func_string(node_func const node_t);

}

#endif