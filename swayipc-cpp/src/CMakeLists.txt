file(GLOB source_files *.cpp)

add_library(swayipc_cpp ${source_files})

target_link_libraries(
    swayipc_cpp
    PUBLIC
    nlohmann_json::nlohmann_json
    PRIVATE
    Threads::Threads
)

target_include_directories(
    swayipc_cpp
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/swayipc-cpp/include>
)

set(CMAKE_INSTALL_INCLUDEDIR include)
set(CMAKE_INSTALL_BINDIR bin)
set(CMAKE_INSTALL_LIBDIR lib)
set(CMAKE_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/swayipc_cpp)

install(
    TARGETS swayipc_cpp
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

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/swayipc_cppConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/swayipc_cppConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
    PATH_VARS CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/swayipc_cppConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

### Install Config and ConfigVersion files
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/swayipc_cppConfig.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/swayipc_cppConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
)
