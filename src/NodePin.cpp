#include "NodePin.hpp"

namespace shader_nodes {

std::string_view data_type_string(DataType type) {
    switch(type) {
        case DataType::Float:
            return "float";
        case DataType::Float2:
            return "vec2";
        case DataType::Float3:
            return "vec3";
        case DataType::Float4:
            return "vec4";
    }
}

}