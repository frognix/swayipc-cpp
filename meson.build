project('swayipc-cpp', 'cpp',
        default_options : ['cpp_std=c++17'])

incdir = include_directories('include')

src = ['./src/client.cpp', './src/socket_wrapper.cpp',
       './src/sway_socket.cpp', './src/container.cpp']

json_dep = dependency('nlohmann_json')
thread_dep = dependency('threads')

swayipc_lib = static_library('swayipc-cpp',
                             src,
                             include_directories : incdir,
                             dependencies : [json_dep, thread_dep],
                             override_options : ['cpp_std=c++17'])

swayipc_dep = declare_dependency(link_with : swayipc_lib, include_directories : incdir)

#build examples

examples = [
  'connecting',
  'event_handling'
]

foreach example : examples
  executable('example-' + example, './examples' / (example + '.cpp'),
             dependencies: swayipc_dep,
             build_by_default: false)
endforeach
