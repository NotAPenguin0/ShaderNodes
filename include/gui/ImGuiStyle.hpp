#ifndef SHADER_NODES_IMGUI_STYLE_HPP_
#define SHADER_NODES_IMGUI_STYLE_HPP_

namespace shader_nodes::gui {

enum class Style { ImGuiDark = 0, Grey = 1, None};

void set_editor_style(Style style);

void init_font();
void clear_font();
void push_font();
void push_font_bold();

}

#endif // ifndef SHADER_NODES_IMGUI_STYLE_HPP_