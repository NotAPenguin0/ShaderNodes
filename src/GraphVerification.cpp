#include "GraphVerification.hpp"

#include "GraphVisitor.hpp"
#include "Nodes.hpp"

namespace shader_nodes {

namespace {

struct diagnostic_logger {
private:
    ShaderGraph const& graph;
    std::ostream& out;

    void log_node(ShaderNode const& node) {
        out << "At node (id: " << node.id << ")\n";
        out << "Node function: " << node_func_string(node.func) << "\n";
    }

public:
    diagnostic_logger(ShaderGraph const& graph, std::ostream& out) 
        : graph(graph), out(out) {}

    void report_type_mismatch(NodePin const& out_pin, NodePin const& in_pin) {
        ShaderNode const& in_node = graph.get_node(in_pin.owning_node);
        // Report error at input node
        log_node(in_node);
        out << "Error: Type mismatch: cannot convert from '" 
            << data_type_string(out_pin.data_type)
            << "' to '" << data_type_string(in_pin.data_type) << "'\n"
            << "Note: connection between pins:\n"
            << out_pin.name << " --> " << in_pin.name << "\n";
    }
};

// Type verification makes sure all pins that are connected have the same type.
struct type_verification_visitor {
private:
    ShaderGraph const& graph;
    diagnostic_logger& logger;
public:
    type_verification_visitor(ShaderGraph const& graph, diagnostic_logger& logger) 
        : graph(graph), logger(logger) {}


    void operator()(ShaderNode const& node) {
        // Verifying all outputs is enough to make sure
        // types match everywhere
        auto const& outputs = node.get_outputs();
        for(auto pid : outputs) {
            NodePin const& out_pin = graph.get_pin(pid);
            // If the output pin does not have a connection, we do not need 
            // to verify anything
            if (!out_pin.connection) { continue; }
            NodePin const& in_pin = graph.get_pin(out_pin.connection);

            // Do the type check
            if (out_pin.data_type != in_pin.data_type) {
                logger.report_type_mismatch(out_pin, in_pin);
            }
        }
    }
};

void verify_types(ShaderGraph const& graph, diagnostic_logger& logger) {
    type_verification_visitor visitor(graph, logger);
    visit_graph(graph, visitor);
}

} // namespace


void verify_graph(ShaderGraph const& graph, std::ostream& out) {
    diagnostic_logger logger(graph, out);
    verify_types(graph, logger);

}

} // namespace shader_nodes