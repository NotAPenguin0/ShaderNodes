#include "gui/Editor.hpp"

#include "generators/GLSLGenerator.hpp"
#include "GraphVerification.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "gui/ImGuiStyle.hpp"

#include <imgui_node_editor.h>

#define GL_VERSION_STRING "#version 330"
#define GL_VERSION_MAJOR 3
#define GL_VERSION_MINOR 3

namespace ed = ax::NodeEditor;

static ed::EditorContext* node_context = nullptr;

#include <iostream>

namespace shader_nodes::gui {

void make_dbg_graph(ShaderGraph& graph) {
    auto& gl_position = graph.add_node();
    auto& merge_node = graph.add_node(shader_nodes::node_func::add);
    (void)merge_node;
    auto& begin_node = graph.add_node();
    auto& begin_node2 = graph.add_node();
    auto& out_node = graph.add_node();
    out_node.func = shader_nodes::node_func::output_value;
    auto& constant = graph.add_node();
    constant.func = shader_nodes::node_func::constant;
    constant.value = 42.0;
    begin_node.description = "Begin";
    begin_node.func = shader_nodes::node_func::constant;
    begin_node.value = 5.0;
    begin_node2.description = "Begin2";
    begin_node2.func = shader_nodes::node_func::constant;  
    begin_node2.value = 9.0;
    gl_position.description = "gl_Position";
    gl_position.func = shader_nodes::node_func::builtin_out;

    auto& begin_out = begin_node.add_output_pin();
    begin_out.name = "input";
    begin_out.data_type = shader_nodes::DataType::Float;
    auto& begin2_out = begin_node2.add_output_pin();
    begin2_out.name = "input";
    begin2_out.data_type = shader_nodes::DataType::Float;
    auto& end_in = gl_position.add_input_pin();
    end_in.name = "gl_Position";
    end_in.data_type = shader_nodes::DataType::Float4;

    auto& out_pin = out_node.add_input_pin();
    out_pin.name = "out_val";
    out_pin.data_type = shader_nodes::DataType::Float;
    auto& const_pin = constant.add_output_pin();
    const_pin.name = "unset";
    const_pin.data_type = shader_nodes::DataType::Float;

}

void init() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Editor::Editor() {
    window = glfwCreateWindow(800, 600, "Shader Nodes", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        std::cout << "Failed to initialize GLAD\n";
//        return;
    }

    // Initialize imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingWithShift = false;
//    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GL_VERSION_STRING);

    set_editor_style(Style::Grey);
    init_font();

    node_context = ed::CreateEditor();

    make_dbg_graph(graph);
}

Editor::~Editor() {
    ed::DestroyEditor(node_context);
    glfwTerminate();
}

void Editor::run() {

    while(running) {
        if (glfwWindowShouldClose(window)) {
            running = false;
        }

        glClearColor(0.2, 0.4, 0.8, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // set font for next frame
        push_font();

        create_dockspace();

        // Render the frame
        frame();

        // clear font
        clear_font();

        ImGui::End();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        auto& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void Editor::frame() {
    ed::SetCurrentEditor(node_context);

    menu_bar.show();
    node_list.show();
    canvas.show(graph);

    static bool last_pressed = false;
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS
        && !last_pressed) {
        verify_graph(graph, std::cout);

        std::cout << "\n\n";
        std::cout << "Generated GLSL:\n";
        std::cout << "----------------------\n";

        shader_nodes::GLSLGenerator generator;
        std::cout << generator.generate(graph);
        last_pressed = true;
    } else {
        last_pressed = false;
    }
}

void Editor::create_dockspace() {
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Main_DockSpace", nullptr, flags);
    ImGui::PopStyleVar(3);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("Main_DockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f),
                         ImGuiDockNodeFlags_None);
    }
}

void Editor::quit() {
    glfwSetWindowShouldClose(window, true);
}

} // namespace shader_nodes::gui