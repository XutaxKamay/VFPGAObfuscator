project('FPGASimulator', 'cpp', default_options : ['cpp_std=c++20', 'warning_level=3', 'werror=true'])

src_common = [
  'src/simulator/FPGA.cpp',
  'src/simulator/Port.cpp',
  'src/simulator/LogicGate.cpp',
  'src/simulator/Timer.cpp'
]

src_test = [
  'src/simulator/Test.cpp'
]
src_test += src_common

tbb = dependency('tbb')
library('FPGASimulator', src_common, dependencies : tbb)
executable('Test', src_test, dependencies : tbb)
