#pragma once

#include <nlohmann/json.hpp>
#include <optional>

namespace nlohmann {
template <typename T>
struct adl_serializer<std::optional<T>> {
  static void to_json(json& j, const std::optional<T>& opt) {
     if (opt == std::nullopt) j = nullptr;
     else j = *opt;
  }
  static void from_json(const json& j, std::optional<T>& opt) {
    if (j.is_null()) opt = std::nullopt;
    else opt = j.get<T>();
  }
};
}

namespace swayipc::data {

template <class T>
inline void get_or(const nlohmann::json& j, const char* name, T& val) {
    if (j.contains(name) && !j[name].is_null()) val = j[name].get<T>();
}

template <class T>
inline void set_or(nlohmann::json& j, const char* name, const T& val) {
    auto jval = nlohmann::json(val);
    if (!jval.is_null()) {
        j[name] = jval;
    } else {
        j.erase(name);
    }
}

#define JSON_TYPE_HEADER(type)                                  \
    void to_json(nlohmann::json&, const type&);                 \
    void from_json(const nlohmann::json&, type&);

} // namespace swayipc::data
