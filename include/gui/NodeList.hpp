#ifndef SHADER_NODES_GUI_NODE_LIST_HPP_
#define SHADER_NODES_GUI_NODE_LIST_HPP_

#include "Nodes.hpp"

namespace shader_nodes::gui {

class NodeList {
public:
    void show();
    void hide();

private:
    void display_node_func(node_func func);

    bool shown = true;
};

}

#endif