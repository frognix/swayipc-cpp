#pragma once

#include "serialization.hpp"

namespace swayipc {

class tree {
public:
    tree(data::node_s node);

    data::node_s* find_focused();
private:
    static data::node_s* find(data::node_s&, std::function<bool(const data::node_s&)>);
    data::node_s m_node;
};

}
