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

using json = nlohmann::json;

template <class T>
inline void get_or(const json& j, const char* name, T& val) {
    if (j.contains(name) && !j[name].is_null()) val = j[name].get<T>();
}

template <class T>
inline void set_or(json& j, const char* name, const T& val) {
    auto jval = json(val);
    if (!jval.is_null()) {
        j[name] = jval;
    } else {
        j.erase(name);
    }
}

template <class T>
using opt = std::optional<T>;

#define DEFAULT_FROM(v1) get_or(nlohmann_json_j, (#v1), nlohmann_json_t.v1);

#define DEFAULT_TO(v1) set_or(nlohmann_json_j, (#v1), nlohmann_json_t.v1);

#define DEFINE_TYPE(Type, To, From, ...) \
       inline void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(To, __VA_ARGS__)) } \
       inline void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(From, __VA_ARGS__)) }

#define DEFINE_TYPE_DEFAULT(Type, ...)  DEFINE_TYPE(Type, DEFAULT_TO, DEFAULT_FROM, __VA_ARGS__)

#define DEFINE_STRICT_TYPE(Type, ...) DEFINE_TYPE(Type, NLOHMANN_JSON_TO, NLOHMANN_JSON_FROM, __VA_ARGS__)

#define DEFINE_ENUM(Type, ...) NLOHMANN_JSON_SERIALIZE_ENUM(Type, __VA_ARGS__)

struct rect_s {
    int x;
    int y;
    int width;
    int height;
};
DEFINE_STRICT_TYPE(rect_s, x, y, width, height)

}
