file(GLOB source_files *.cpp)

add_custom_target(swayipc_cpp_examples)

foreach(file ${source_files})
    get_filename_component(name ${file} NAME_WE)
    set(example swayipc_cpp_example_${name})

    add_executable(${example} ${file})

    add_dependencies(swayipc_cpp_examples ${example})

    target_link_libraries(
        ${example}
        PRIVATE
        swayipc_cpp_static
    )
endforeach()
