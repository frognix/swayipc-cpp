#include "tree.hpp"

using namespace swayipc::data;

namespace swayipc {

tree::tree(node_s node)
    : m_node(std::move(node)) {}

node_s* tree::find_focused() {
    return find(m_node, [](auto n){ return n.focused; });
}

node_s* tree::find(node_s& node, std::function<bool(const node_s&)> pred) {
    if (pred(node)) return &node;
    else {
        for (auto& subnode : node.nodes) {
            if (auto res = find(subnode, pred); res)
                return res;
        }
        return nullptr;
    }
}

}
