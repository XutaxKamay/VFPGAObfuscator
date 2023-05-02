project('FPGASimulator', 'cpp', default_options : ['cpp_std=c++20', 'warning_level=3', 'werror=true'])

src_common = [
  'FPGA.cpp',
  'Port.cpp',
  'LogicGate.cpp',
  'Timer.cpp'
]

src_test = [
  'Test.cpp'
]
src_test += src_common

tbb = dependency('tbb')
library('FPGASimulator', src_common, dependencies : tbb)
executable('Test', src_test, dependencies : tbb)
