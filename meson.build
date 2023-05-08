project('FPGASimulator', 'cpp', default_options : ['cpp_std=c++20', 'warning_level=everything', 'werror=true'])
add_project_arguments(['-DPARALLEL', '-Wno-effc++', '-Wno-c++98-compat'], language : 'cpp')

src_simulator = [
  'src/simulator/Deserializer.cpp',
  'src/simulator/Error.cpp',
  'src/simulator/FPGA.cpp',
  'src/simulator/Port.cpp',
  'src/simulator/LogicGate.cpp',
  'src/simulator/Serializer.cpp',
  'src/simulator/Timer.cpp'
]

src_simulator_test = [
  'src/simulator/Test.cpp'
]
src_simulator_test += src_simulator

tbb = dependency('tbb')
library('FPGASimulator', src_simulator, dependencies : tbb)
executable('Test', src_simulator_test, dependencies : tbb)
