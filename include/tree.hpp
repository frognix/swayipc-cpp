#include "serialization.hpp"

class tree {
public:
    tree(node_s node);

    node_s* find_focused();
private:
    static node_s* find(node_s&, std::function<bool(const node_s&)>);
    node_s m_node;
};
