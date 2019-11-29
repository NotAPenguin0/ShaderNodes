#include "gui/NodeCanvas.hpp"

#include "gui/NodeBuilder.hpp"

#include <stb/stb_image.h>
#include <imgui_node_editor.h>
#include <imgui/imgui_internal.h>
#include <glad/glad.h>

#include <limits>
#include <algorithm>
#include <iostream>

namespace ImGui {
    bool BeginDragDropTargetCustom(const ImRect& bb, ImGuiID id);
}

namespace ed = ax::NodeEditor;

namespace {
    int next_link_id = 100000;
}

struct TexLoadResult {
    ImTextureID texture;
    ImVec2 size;
};

TexLoadResult load_texture(const char* path) {

    GLuint texture_handle = 0;

    stbi_set_flip_vertically_on_load(false);
    int w = 0, h = 0;
    std::int32_t desired_channel_count = 4;
    std::int32_t channels;
    unsigned char* image_data = stbi_load(path, &w, &h, &channels, desired_channel_count);
    if (image_data == nullptr) {
        std::cout << "Failed to load image data";
        std::cout.flush();
        return {nullptr, {0, 0}};
    }

    glGenTextures(1, &texture_handle);
    glBindTexture(GL_TEXTURE_2D, texture_handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);


    // memory is now on the GPU, we don't need it here anymore
    stbi_image_free(image_data);

    return {reinterpret_cast<ImTextureID>(texture_handle), ImVec2(w, h)};
}

namespace shader_nodes::gui {

void NodeCanvas::init() {
    auto tex_load_result = load_texture("config/textures/header_background.png");
    node_header = tex_load_result.texture;
    node_header_size = tex_load_result.size;

    tex_load_result = load_texture("config/textures/circle_icon.png");
    pin_tex = tex_load_result.texture;
}

void NodeCanvas::show(ShaderGraph& graph) {
    (void)col;
    
    if (ImGui::Begin("Node Canvas", &shown)) {
        node_to_add = std::nullopt;
        handle_drag_drop(graph);
        ed::Begin("ShaderNode Editor");
            if (node_to_add) {
                ShaderNode const& node = graph.add_node(*node_to_add);
                auto const id = node_to_gui_id(node.id);
                ed::SetNodePosition(id, ImGui::GetMousePos());
            }
            show_nodes(graph);
        ed::End();
    }

    ImGui::End();
}

void NodeCanvas::hide() {
    shown = false;
}


void NodeCanvas::show_nodes(ShaderGraph& graph) { 
    static ImVec4 header_red = ImVec4(218, 7, 7, 255);    
    for(auto const&[id, node] : graph.get_nodes()) {
        NodeBuilder builder(node);

        builder.input_pins(node.get_inputs(), pin_tex)
               .output_pins(node.get_outputs(), pin_tex)
               .header({node_header, node_header_size}, 20, header_red)
               .title(node.description)
               .render();
    }

    // Submit links
    for (auto const& link : links) {
        ed::Link(link.id, link.input_id, link.output_id);
    }

    handle_editor_actions(graph);
}  

void NodeCanvas::handle_editor_actions(ShaderGraph& graph) {
    if (ed::BeginCreate())
    {
        ed::PinId input_id, output_id;
        if (ed::QueryNewLink(&input_id, &output_id))
        {
            // Validate ID's
            if (input_id && output_id)
            {
                if (ed::AcceptNewItem())
                {
                    links.push_back({ ed::LinkId(++next_link_id), input_id, output_id });
                    // Add link to graph
                    connect(
                        graph.get_pin(pin_from_gui_id((size_t)input_id)), 
                        graph.get_pin(pin_from_gui_id((size_t)output_id))
                    );
                    // Draw new link.
                    ed::Link(links.back().id, links.back().input_id, links.back().output_id);
                }
            }
        }
    }
    ed::EndCreate(); 


    // Handle deletion action
    if (ed::BeginDelete())
    {
        ed::LinkId deleted_id;
        while (ed::QueryDeletedLink(&deleted_id))
        {
            if (ed::AcceptDeletedItem())
            {
                for (auto& link : links)
                {
                    if (link.id == deleted_id)
                    {
                        disconnect(
                            graph.get_pin(pin_from_gui_id((size_t)link.input_id)), 
                            graph.get_pin(pin_from_gui_id((size_t)link.output_id))
                        );
                        links.erase(std::remove(links.begin(), links.end(), link), 
                                    links.end());
                        break;
                    }
                }
            }
        }
        ed::NodeId deleted_node_id;
        while(ed::QueryDeletedNode(&deleted_node_id)) {
            if (ed::AcceptDeletedItem()) {
                ShaderNode& node = graph.get_node(node_from_gui_id((size_t)deleted_node_id));
                // delete possibly related links
                for (auto& link : links) {
                    // grab required data
                    auto& inputs = node.get_inputs();
                    auto& outputs = node.get_outputs();
                    auto in_id = pin_from_gui_id((size_t)link.input_id);
                    auto out_id = pin_from_gui_id((size_t)link.output_id);
                    // if the link is an element of this node's input list, or a member of this
                    // link's output list, remove it
                    if (std::find(inputs.begin(), inputs.end(), in_id) != inputs.end()
                        || std::find(outputs.begin(), outputs.end(), out_id) != outputs.end()) {
                            links.erase(std::remove(links.begin(), links.end(), link), 
                                    links.end());
                        }
                }
                graph.delete_node(node_from_gui_id((size_t)deleted_node_id));
            }
        }
    }
    ed::EndDelete();
}


void NodeCanvas::handle_drag_drop(ShaderGraph& graph) {
    ImVec2 const size = ImGui::GetWindowSize();
    // magical values that seem to make it work
    ImVec2 const pos = ImGui::GetWindowPos();
    const float offset = 8;
    ImRect bounds;
    bounds.Min = {pos.x + offset, pos.y + offset};
    bounds.Max = {pos.x + size.x - offset, pos.y + size.y - offset};
    if (ImGui::BeginDragDropTargetCustom(bounds, 1)) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("p_new_node"); 
            payload) {
            
            node_func const func = *reinterpret_cast<node_func*>(payload->Data);
            node_to_add = func;
        }
        ImGui::EndDragDropTarget();
    }
}

} // namespace shader_nodes::gui