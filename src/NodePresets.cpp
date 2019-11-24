#include "NodePresets.hpp"

namespace shader_nodes::presets{ 

std::unordered_map<node_func, NodePreset> presets;

static NodePreset::PinPreset make_pin(std::string_view name, DataType type) {
    return NodePreset::PinPreset {name.data(), type};
}

void make_binary_node(node_func func, std::string_view name, DataType type) {
    presets[func] = {
        name.data(),
        {make_pin("a", type), make_pin("b", type)},
        {make_pin("out", type)}
    };
}

void init() {
    make_binary_node(node_func::multiply, "Multiply", DataType::Float);
    make_binary_node(node_func::add, "Add", DataType::Float);
    make_binary_node(node_func::subtract, "Subtract", DataType::Float);
    make_binary_node(node_func::divide, "Divide", DataType::Float);
}

}