#include "gui/MenuBar.hpp"

#include "imgui/imgui.h"
#include <sstream>

#include "GraphVerification.hpp"
#include "GraphVisitor.hpp"
#include "generators/GLSLGenerator.hpp"

namespace shader_nodes::gui {

#include <iostream>

std::ostream& operator<<(std::ostream& out, shader_nodes::ShaderNode const& node) {

    out << "Node description:\n" << node.description << "\n";
    out << "Input pins:\n";
    for(auto pin_id : node.get_inputs()) {
        auto const& pin = node.get_graph().get_pin(pin_id);
        out << pin.name << "\n";
    }
    out << "Output pins:\n";
    for(auto pin_id : node.get_outputs()) {
        auto const& pin = node.get_graph().get_pin(pin_id);
        out << pin.name << "\n";
    }

    return out;
}

struct graph_printer {
    void operator()(shader_nodes::ShaderNode const& node) {
        std::cout << node << std::endl << std::endl;
    }
};

void MenuBar::show(ShaderGraph const& graph) {

    static bool glsl_out_shown = false;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Shader")) {
            if (ImGui::MenuItem("Generate glsl")) {
                std::ostringstream diagnostics_log;
                verify_graph(graph, diagnostics_log);

//                visit_graph(graph, graph_printer{});

                shader_nodes::GLSLGenerator generator;
                glsl = generator.generate(graph);
                glsl_out_shown = true;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (glsl_out_shown)
    {
        if (ImGui::Begin("Generated GLSL code", &glsl_out_shown)) {
            ImGui::TextWrapped("%s", glsl.c_str());
        }
        ImGui::End();
    }

}

}