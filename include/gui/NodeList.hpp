#ifndef SHADER_NODES_GUI_NODE_LIST_HPP_
#define SHADER_NODES_GUI_NODE_LIST_HPP_

#include "Nodes.hpp"

#include <string>

namespace shader_nodes::gui {

class NodeList {
public:
    NodeList();

    void show();
    void hide();

private:
    void display_node_func(node_func func);

    bool shown = true;
    std::string filter;

    static constexpr size_t filter_buf_size = 128;
};

}

#endif