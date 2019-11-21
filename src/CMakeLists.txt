set(SHADER_NODES_SOURCES
    ${SHADER_NODES_SOURCES}
    
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ShaderGraph.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GraphVisitor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/NodePin.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/generators/GLSLGenerator.cpp"

    PARENT_SCOPE
)