file(GLOB source_files *.cpp)

add_library(swayipc_cpp_static STATIC ${source_files})

target_link_libraries(
    swayipc_cpp_static
    PUBLIC
    Threads::Threads
    nlohmann_json::nlohmann_json
)

target_include_directories(
    swayipc_cpp_static
    PUBLIC
    ${CMAKE_SOURCE_DIR}/swayipc-cpp/include
)
