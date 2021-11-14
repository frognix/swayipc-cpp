file(GLOB source_files *.cpp)

add_library(swayipc_cpp_static STATIC ${source_files})

target_link_libraries(
    swayipc_cpp_static
    PUBLIC
    nlohmann_json::nlohmann_json
    PRIVATE
    Threads::Threads
)

target_include_directories(
    swayipc_cpp_static
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/swayipc-cpp/include>
)

set(CMAKE_INSTALL_INCLUDEDIR include)
set(CMAKE_INSTALL_BINDIR bin)
set(CMAKE_INSTALL_LIBDIR lib)
set(CMAKE_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake)

install(
    TARGETS swayipc_cpp_static
    EXPORT swayipc_cpp_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/swayipc-cpp/include
    DESTINATION .
)

install(
    EXPORT swayipc_cpp_targets
    NAMESPACE swayipc_cpp::
    DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
)

file(
    WRITE
    ${CMAKE_BINARY_DIR}/Config.cmake.in
    "@PACKAGE_INIT@\ninclude(\"\${CMAKE_CURRENT_LIST_DIR}/swayipc_cpp_targets.cmake\")"
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_BINARY_DIR}/Config.cmake.in
    swayipc_cpp-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
    PATH_VARS CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/swayipc_cpp_version.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

### Install Config and ConfigVersion files
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/swayipc_cpp-config.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/swayipc_cpp_version.cmake
    DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
)
