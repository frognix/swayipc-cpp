#pragma once

#define DEFAULT_FROM(v1) get_or(nlohmann_json_j, (#v1), nlohmann_json_t.v1);

#define DEFAULT_TO(v1) set_or(nlohmann_json_j, (#v1), nlohmann_json_t.v1);

#define DEFINE_TYPE(Type, To, From, ...) \
       void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(To, __VA_ARGS__)) } \
       void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(From, __VA_ARGS__)) }

#define DEFINE_TYPE_DEFAULT(Type, ...)  DEFINE_TYPE(Type, DEFAULT_TO, DEFAULT_FROM, __VA_ARGS__)

#define DEFINE_STRICT_TYPE(Type, ...) DEFINE_TYPE(Type, NLOHMANN_JSON_TO, NLOHMANN_JSON_FROM, __VA_ARGS__)

#define DEFINE_ENUM(Type, ...)                                          \
    void to_json(nlohmann::json& json, const Type& value) {             \
        const std::unordered_map<Type, std::string_view> map __VA_ARGS__; \
        auto it = map.find(value);                                      \
        json = ((it != std::end(map)) ? it : std::begin(map))->second;  \
    }                                                                   \
    void from_json(const nlohmann::json& json, Type& value) {           \
        const std::vector<std::pair<Type, std::string_view>> set __VA_ARGS__; \
        auto str = json.get<std::string_view>();                        \
        auto it = std::find_if(set.begin(), set.end(),                  \
                               [str](const auto& pair) { return pair.second == str; }); \
        value = ((it != std::end(set)) ? it : std::begin(set))->first;  \
    }
