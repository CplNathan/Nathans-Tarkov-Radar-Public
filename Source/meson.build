project('tarkovradar', 'cpp', 'c', version : '1.0', default_options : ['c_std=c99', 'cpp_std=c++17'])

compile_args = ['-D_POSIX_C_SOURCE=200809L', '-D_DEFAULT_SOURCE', '-pedantic', '-DMVERBOSE=0', '-DMTR_ENABLED', '-DREAD_CHECK', '-DASIO_STANDALONE', '-pthread']
compile_args_external = ['-DLMODE=MODE_EXTERNAL', '-fsanitize=address']
compile_args_internal = ['-DLMODE=MODE_QEMU_INJECT', '-DMVERBOSE=0']
link_args_external = ['-lasan']
link_args_internal = []
link_args = []
cpp_compile_args = []
c_compile_args = []

try_args = ['-Weverything', '-Wno-padded', '-Wno-sign-conversion', '-Wno-padded', '-Wno-documentation-unknown-command', '-Wno-missing-variable-declarations', '-Wno-c++98-compat', '-Wno-c++98-compat-pedantic', '-Wno-weak-vtables']
try_args_c = ['-Wno-missing-prototypes']
try_args_cpp = ['-Wno-padded', '-Wno-c++98-compat', '-Wno-padded', '-Wno-c++98-compat-pedantic', '-Wno-weak-vtables', '-Wno-documentation-unknown-command', '-Wno-old-style-cast']

cxx = meson.get_compiler('cpp')
cc = meson.get_compiler('c')

foreach arg : try_args
  if cc.has_argument(arg)
    c_compile_args += arg
  endif
  if cxx.has_argument(arg)
    cpp_compile_args += arg
  endif
endforeach

foreach arg : try_args_cpp
  if cxx.has_argument(arg)
    cpp_compile_args += arg
  endif
endforeach

foreach arg : try_args_c
  if cc.has_argument(arg)
    c_compile_args += arg
  endif
endforeach

dl = cc.find_library('dl', required : false)
thread = cc.find_library('pthread', required : false)
threadcxx = cxx.find_library('pthread', required : true)

vmread_files = ['lib/vmread/mem.c', 'lib/vmread/wintools.c', 'lib/vmread/pmparser.c', 'lib/vmread/vmmem.c']
hlapi_files = ['lib/vmread/hlapi/windll.cpp', 'lib/vmread/hlapi/winprocess.cpp', 'lib/vmread/hlapi/winprocesslist.cpp']
socketio_files = ['lib/socketio/src/sio_client.cpp', 'lib/socketio/src/sio_socket.cpp', 'lib/socketio/src/internal/sio_client_impl.cpp', 'lib/socketio/src/internal/sio_packet.cpp']
tarkov_files = ['main.cpp', 'tarkov/Tarkov/Tarkov.cpp', 'tarkov/Helper/pattern_scan.cpp']

inccustomlibs = [include_directories('tarkov'), include_directories('socket'), include_directories('lib/asio/asio/include'), include_directories('lib/catch/include'), include_directories('lib/rapidjson/include'), include_directories('lib/websocketpp'), include_directories('lib/socketio/src'), include_directories('lib/vmread')]

TarkovInjector = executable(
    'TarkovInjector',
    files(vmread_files + hlapi_files + socketio_files + tarkov_files),
  include_directories : [inccustomlibs],
  c_args : c_compile_args + compile_args + compile_args_external,
  cpp_args : cpp_compile_args + compile_args + compile_args_external,
  link_args : compile_args + compile_args_external + link_args + link_args_external,
  dependencies: [threadcxx]
)