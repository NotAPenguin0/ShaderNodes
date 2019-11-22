#ifndef SHADER_NODES_NODE_VALUE_HPP_
#define SHADER_NODES_NODE_VALUE_HPP_

#include <variant>

namespace shader_nodes {

struct float2 {
    float x, y;
};

struct float3 {
    float x, y, z;
};

struct float4 {
    float x, y, z, w;
};

struct NodeValue {
    template<typename T>
    void operator=(T val) {
        value = val;
    }

    std::variant<float, float2, float3, float4> value;
};

}

#endif