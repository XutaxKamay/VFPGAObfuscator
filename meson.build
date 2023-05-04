project('FPGASimulator', 'cpp', default_options : ['cpp_std=c++20', 'warning_level=3', 'werror=true'])

src_simulator = [
  'src/simulator/FPGA.cpp',
  'src/simulator/Port.cpp',
  'src/simulator/LogicGate.cpp',
  'src/simulator/Timer.cpp'
]

src_simulator_test = [
  'src/simulator/Test.cpp'
]
src_simulator_test += src_simulator

tbb = dependency('tbb')
library('FPGASimulator', src_simulator, dependencies : tbb)
executable('Test', src_simulator_test, dependencies : tbb)
