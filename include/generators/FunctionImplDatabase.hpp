#ifndef SHADER_NODES_FUNCTION_IMPL_DATABASE_HPP_
#define SHADER_NODES_FUNCTION_IMPL_DATABASE_HPP_

#include "Nodes.hpp"

#include <string>

namespace shader_nodes {

void initialize_function_impl_database();

std::string const& get_function_impl(node_func node_t);

}

#endif