#ifndef SHADER_NODES_GRAPH_VERIFICATION_HPP_
#define SHADER_NODES_GRAPH_VERIFICATION_HPP_

#include "ShaderGraph.hpp"

#include <iostream>

namespace shader_nodes {

void verify_graph(ShaderGraph const& graph, std::ostream& out);

}

#endif