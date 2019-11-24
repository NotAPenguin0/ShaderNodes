#include "Nodes.hpp"

#include <cassert>

namespace shader_nodes {

bool is_function(node_func node_t) {
    switch(node_t) {
        default:
            return false;
    }
}

bool is_binary_op(node_func node_t) {
    switch(node_t) {
        case node_func::multiply:
        case node_func::add:
        case node_func::subtract:
        case node_func::divide:
            return true;
        default:
            return false;
    }
}

char binary_op_token(node_func op) {
    assert(is_binary_op(op));
    switch(op) {
        case node_func::multiply:
            return '*';
        case node_func::add:
            return '+';
        case node_func::subtract:
            return '-';
        case node_func::divide:
            return '/';
        default:
            return ' ';
    }
}

std::string_view node_func_string(node_func const node_t) {
    switch(node_t) {
        case node_func::multiply:
            return "multiply";
        case node_func::add:
            return "add";
        case node_func::subtract:
            return "subtract";
        case node_func::divide:
            return "divide";
        case node_func::constant:
            return "constant";
        case node_func::output_value:
            return "output";
        case node_func::builtin_out:
            return "builtin_out";
        case node_func::builtin_vars:
            return "builtin_vars";
        default:
            return "unknown_node_function";
    }
}


}