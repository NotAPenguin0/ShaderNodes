add_subdirectory(glfw)


set(GLAD_GL_VERSION
    430
)

set(SHADER_NODES_SOURCES
    ${SHADER_NODES_SOURCES}
    # compile glad sources
    "${CMAKE_CURRENT_SOURCE_DIR}/glad${GLAD_GL_VERSION}/src/glad.c"
    # compile imgui sources
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_impl_opengl3.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"

    PARENT_SCOPE
)


set(SHADER_NODES_INCLUDE_DIRS
    ${SHADER_NODES_INCLUDE_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR}/glad${GLAD_GL_VERSION}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImNodes/NodeEditor/Include"
    PARENT_SCOPE
)


set(IM_NODES_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/ImNodes/NodeEditor/Source/crude_json.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImNodes/NodeEditor/Source/imgui_canvas.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImNodes/NodeEditor/Source/imgui_node_editor_api.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImNodes/NodeEditor/Source/imgui_node_editor.cpp"
)

add_library(ImNodes STATIC 
    ${IM_NODES_SOURCES}
)

target_compile_options(ImNodes PRIVATE
    -W -std=c++17
)

target_include_directories(ImNodes PUBLIC
    ${SHADER_NODES_INCLUDE_DIRS}    
    "${CMAKE_CURRENT_SOURCE_DIR}/ImNodes/NodeEditor/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(SHADER_NODES_LINK_LIBRARIES
    ${SHADER_NODES_LINK_LIBRARIES}
    glfw dl ImNodes
    PARENT_SCOPE
)

