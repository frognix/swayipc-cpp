project('swayipc-cpp', 'cpp',
        default_options : ['cpp_std=c++2a'])

incdir = include_directories('include')

src = ['./src/connection.cpp', './src/socket_wrapper.cpp',
       './src/tree.cpp', './src/sway_socket.cpp']

json_dep = dependency('nlohmann_json')
thread_dep = dependency('threads')

swayipc_lib = static_library('swayipc-cpp',
                             src,
                             include_directories : incdir,
                             dependencies : [json_dep, thread_dep],
                             override_options : ['cpp_std=c++2a'])

swayipc_dep = declare_dependency(link_with : swayipc_lib, include_directories : incdir)
