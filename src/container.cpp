#include "container.hpp"

using namespace swayipc::data;

container* container::find(std::function<bool(const container&)> pred) {
    std::function<container*(container&)> inner_find;
    inner_find = [&inner_find, pred](container& cont) -> container* {
        if (pred(cont)) return &cont;
        for (auto& subnode : cont.nodes) {
            if (auto res = inner_find(subnode); res)
                return res;
        }
        return nullptr;
    };
    return inner_find(*this);
}

std::vector<container*> container::find_many(std::function<bool(const container&)> pred) {
    std::vector<container*> res;
    std::function<void(container&)> inner_find;
    inner_find = [&res, &inner_find, pred](container& cont) mutable {
        if (pred(cont)) res.push_back(&cont);
        for (auto& subnode : cont.nodes) {
            inner_find(subnode);
        }
    };
    inner_find(*this);
    return res;
}
