#ifndef SHADER_NODES_NODE_ID_HPP_
#define SHADER_NODES_NODE_ID_HPP_

#include "types.hpp"

#include <limits>

namespace shader_nodes::gui {


// Nodes have a NodeEditor id of their own ID, 
// pins have a NodeEditor id of ID_MAX - their id.

inline int node_to_gui_id(node_id id) {
    return id;
}

inline int pin_to_gui_id(node_pin_id id) { 
    return std::numeric_limits<int>::max() - id;
}

inline node_id node_from_gui_id(int id) {
    return id;
}

inline node_pin_id pin_from_gui_id(int id) {
    return std::numeric_limits<int>::max() - id;
}

}

#endif