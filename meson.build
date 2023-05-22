project('VFPGASimulator', 'cpp', default_options : ['cpp_std=c++20', 'warning_level=everything', 'werror=true'])

add_project_arguments([
    '-DPARALLEL',
    '-Wno-effc++',
    '-Wno-c++98-compat',
    '-Wno-c++98-compat-pedantic',
    '-Wno-c++20-compat',
    '-fno-rtti',
    '-fno-exceptions' ## Goal would be to make it freestanding code
], language : 'cpp')

include_library = [include_directories('src/library'), include_directories('src')]

src_library = [
  'src/library/Deserializer.cpp',
  'src/library/Error.cpp',
  'src/library/Serializer.cpp'
]

src_simulator = [
  'src/simulator/VFPGA.cpp',
  'src/simulator/Port.cpp',
  'src/simulator/LogicGate.cpp',
  'src/simulator/Timer.cpp'
]

src_simulator += src_library

src_language = [
]

src_language += src_library

src_language_test = [
  'src/language/Test.cpp'
]

src_language_test += src_simulator

src_simulator_test = [
  'src/simulator/Test.cpp'
]

src_simulator_test += src_simulator
src_language_test += src_language

tbb = dependency('tbb')

library('VFPGASimulator', src_simulator, dependencies : tbb, include_directories : include_library)
library('VFPGALanguage', src_language, dependencies : tbb, include_directories : include_library)

executable('TestSimulator', src_simulator_test, dependencies : tbb, include_directories : include_library)
executable('TestLanguage', src_language_test, dependencies : tbb, include_directories : include_library)
