#ifndef SATURN_SHADER_NODES_NODE_PIN_HPP_
#define SATURN_SHADER_NODES_NODE_PIN_HPP_

#include "types.hpp"

#include <string>
#include <string_view>

namespace shader_nodes {

enum class DataType {
    Float,
    Float2,
    Float3,
    Float4
};

std::string_view data_type_string(DataType type);

struct NodePin {
    std::string name = "[None]";
    DataType data_type;
    node_id owning_node = 0;
    node_pin_id connection = 0;
    node_pin_id id;
};

}

#endif