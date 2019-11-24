#ifndef SHADER_NODES_NODE_PRESETS_HPP_
#define SHADER_NODES_NODE_PRESETS_HPP_

#include <string>
#include <vector>
#include <unordered_map>

#include "types.hpp"
#include "Nodes.hpp"

namespace shader_nodes {
    
namespace presets {

struct NodePreset {
    struct PinPreset {
        std::string name;
        DataType data_type;
    };

    std::string description;

    std::vector<PinPreset> in_pins;
    std::vector<PinPreset> out_pins;
};

extern std::unordered_map<node_func, NodePreset> presets;

void init();

}

}

#endif