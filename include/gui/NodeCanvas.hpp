#ifndef SHADER_NODES_NODE_CANVAS_HPP_
#define SHADER_NODES_NODE_CANVAS_HPP_

namespace shader_nodes::gui {

class NodeCanvas {
public:
    void show();
    void hide();
private:
    bool shown = true;
};

}

#endif