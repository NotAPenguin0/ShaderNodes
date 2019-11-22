#ifndef SHADER_NODES_GLSL_GENERATOR_HPP_
#define SHADER_NODES_GLSL_GENERATOR_HPP_

#include <ShaderGraph.hpp>
#include <Nodes.hpp>

#include <string>
#include <string_view>

namespace shader_nodes{ 

class GLSLGenerator {
public:
    // Should no longer be std::string eventually
    std::string generate(ShaderGraph const& graph);
};

}

#endif