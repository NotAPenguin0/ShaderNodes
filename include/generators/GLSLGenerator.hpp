#ifndef SHADER_NODES_GLSL_GENERATOR_HPP_
#define SHADER_NODES_GLSL_GENERATOR_HPP_

#include <ShaderGraph.hpp>

#include <string>

namespace shader_nodes{ 

namespace nodes {
    
inline node_type input_value = 0;
inline node_type multiply = 1;
inline node_type output_value = 2;

}

class GLSLGenerator {
public:
    // Should no longer be std::string eventually
    std::string generate(ShaderGraph const& graph);
};

}

#endif