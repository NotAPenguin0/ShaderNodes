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
        case nodes::multiply:
            return true;
        default:
            return false;
    }
}

char binary_op_token(node_func op) {
    assert(is_binary_op(op));
    switch(op) {
        case nodes::multiply:
            return '*';
        default:
            return ' ';
    }
}

std::string_view node_func_string(node_func const node_t) {
    switch(node_t) {
        case nodes::multiply:
            return "multiply";
        case nodes::constant:
            return "constant";
        case nodes::output_value:
            return "output";
        default:
            return "unknown_node_type";
    }
}


}