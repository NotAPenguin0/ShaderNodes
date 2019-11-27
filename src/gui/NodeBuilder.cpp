#include "gui/NodeBuilder.hpp"

#include <imgui_node_editor.h>

#include <cassert>
#include <iostream> // debug

namespace ed = ax::NodeEditor;

namespace shader_nodes::gui {

using namespace detail;

NodeBuilder::NodeBuilder(ShaderNode const& node) 
    : node(node), graph(node.get_graph()) {

    ed::BeginNode(node_to_gui_id(node.id));
}

NodeBuilder::~NodeBuilder() { }

NodeBuilder& NodeBuilder::header(HeaderTexture texture, float height, ImVec4 color) {
    // Check mask and set it
    assert(!(set_sections & static_cast<mask_t>(NodeSection::Top)) 
            && "Header already set!");
    set_sections |= static_cast<mask_t>(NodeSection::Top);

    display_data.header_texture = texture.texture;
    display_data.header_texture_size = texture.size;
    display_data.header_height = height;
    display_data.header_color = color;

    return *this;
}

NodeBuilder& NodeBuilder::title(std::string_view text) {
    // Check mask and set it
    assert(!(set_sections & static_cast<mask_t>(NodeSection::Title)) 
            && "Title already set!");
    set_sections |= static_cast<mask_t>(NodeSection::Title);

    display_data.title_text = text.data();

    return *this;
}

NodeBuilder& NodeBuilder::input_pins(std::vector<node_pin_id> pins) {
    // Check mask and set it
    assert(!(set_sections & static_cast<mask_t>(NodeSection::Left)) 
            && "Input pins already set!");
    set_sections |= static_cast<mask_t>(NodeSection::Left);
    // Store pins
    display_data.input_pins = std::move(pins);

    return *this;
}

NodeBuilder& NodeBuilder::output_pins(std::vector<node_pin_id> pins) {
    // Check mask and set it
    assert(!(set_sections & static_cast<mask_t>(NodeSection::Right)) 
            && "Output pins already set!");
    set_sections |= static_cast<mask_t>(NodeSection::Right);
    // Store pins
    display_data.output_pins = std::move(pins);

    return *this;
}

// TODO: Move this to imgui helper library
void ImGui_TextCentered(std::string_view text, ImVec2 container_size) {
    auto const text_size = ImGui::CalcTextSize(text.data());
    // Set cursor pos to center
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 
        (container_size.x - text_size.x) / 2 - ed::GetStyle().NodePadding.x);
    ImGui::Text("%s", text.data());
}

void NodeBuilder::render() {
    ed::NodeId const id = node_to_gui_id(node.id);
    ImVec2 const node_size = ed::GetNodeSize(id);
    // Dummy because the node editor library screws things up when we don't supply
    // any element before starting with input and output pins
    float spacing_before_body = 0;
    if (has_section(NodeSection::Top)) {
        spacing_before_body += display_data.header_height - 5;
    }

    ImGui::Dummy({1, spacing_before_body});

    ImGui_TextCentered(display_data.title_text, node_size);

    // Render input pins
    for (auto pid : display_data.input_pins) {
        NodePin const& pin = graph.get_pin(pid);
        
        ed::BeginPin(pin_to_gui_id(pid), ed::PinKind::Input);
            ImGui::Text("-> %s", pin.name.c_str());
        ed::EndPin();
    }

    // Make sure input and output pins are displayed next to each other,
    // but only if there were actually input pins. TODO: refine this so
    // output pins get placed on the top instead of at the bottom
    if (!display_data.input_pins.empty()) { ImGui::SameLine(); }

    // Render output pins
    for (auto pid : display_data.output_pins) {
        NodePin const& pin = graph.get_pin(pid);
        
        ed::BeginPin(pin_to_gui_id(pid), ed::PinKind::Output);
            ImGui::Text("%s ->", pin.name.c_str());
        ed::EndPin();
    }

    ed::EndNode();

    // If the builder had a header section supplied, display it
    if (has_section(NodeSection::Top)) {
        render_header();
    }
}

bool NodeBuilder::has_section(NodeSection section) const {
    return set_sections & static_cast<mask_t>(section);
}

void NodeBuilder::render_header() {
    // Get information needed to render header
    ed::NodeId const id = node_to_gui_id(node.id);
    ImVec2 const node_pos = ed::GetNodePosition(id);
    ImVec2 const node_size = ed::GetNodeSize(id);
    // Grab the draw list
    ImDrawList* draw_list = ed::GetNodeBackgroundDrawList(id);
    float const node_border_width = ed::GetStyle().NodeBorderWidth;

    ImVec2 const min_pos = ImVec2(node_pos.x + node_border_width, 
                                  node_pos.y - node_border_width + 1);
    ImVec2 const max_pos = ImVec2(node_pos.x + node_size.x - node_border_width, 
                                  node_pos.y + display_data.header_height + 1);
    ImVec2 const& tex_size = display_data.header_texture_size;
    ImVec2 const min_uv = ImVec2(0, 0);
    ImVec2 const max_uv = ImVec2(node_size.x / (tex_size.x * 2.0f),
                                 display_data.header_height / (tex_size.y * 1.5f));
    ImVec4 const& color = display_data.header_color;

    draw_list->AddImageRounded(
        display_data.header_texture,
        min_pos,
        max_pos,
        min_uv,
        max_uv,
        IM_COL32(color.x, color.y, color.z, color.w),
        ed::GetStyle().NodeRounding,
        ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_TopRight);
}

}