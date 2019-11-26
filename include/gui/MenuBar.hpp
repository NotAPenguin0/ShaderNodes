#ifndef SHADER_NODES_MENU_BAR_HPP_
#define SHADER_NODES_MENU_BAR_HPP_

#include "ShaderGraph.hpp"

namespace shader_nodes::gui {

class MenuBar {
public:
    void show(ShaderGraph const& graph);
private:
    std::string glsl;
};

}

#endif
