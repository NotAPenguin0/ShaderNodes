#ifndef SHADER_NODES_EDITOR_HPP_
#define SHADER_NODES_EDITOR_HPP_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gui/NodeList.hpp"
#include "gui/MenuBar.hpp"
#include "gui/NodeCanvas.hpp"

namespace shader_nodes {

namespace gui {

class Editor {
public:
    Editor();
    ~Editor();

    void run();
    void quit();

private:
    GLFWwindow* window;
    bool running = true;

    void frame();
    void create_dockspace();

    NodeList node_list;
    MenuBar menu_bar;
    NodeCanvas canvas;
};

// Must be called before instantiating the Editor
void init();


} // namespace gui
} // namespace shader_nodes

#endif