#ifndef SHADER_NODES_TYPES_HPP_
#define SHADER_NODES_TYPES_HPP_

#include <cstdint>

namespace shader_nodes {
    
using node_id = uint64_t;
using node_pin_id = uint64_t;

enum class DataType {
    Void,
    Float,
    Float2,
    Float3,
    Float4
};


}

#endif