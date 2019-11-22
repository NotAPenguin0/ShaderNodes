#include "generators/FunctionImplDatabase.hpp"

#include <unordered_map>

namespace {

std::unordered_map<shader_nodes::node_func, std::string> functions;

}

namespace shader_nodes {

void initialize_function_impl_database() {
    functions[nodes::multiply] = "return a * b;";
}

std::string const& get_function_impl(node_func node_t) {
    return functions[node_t];
}

}