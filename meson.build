project('scanner', 'cpp', default_options : ['cpp_std=c++20', 'warning_level=3', 'werror=true'])

src_common = [
  'LogicGate.cpp',
  'FPGA.cpp',
  'Cryptography.cpp',
  'TinyCompiler.cpp'
]

src_test = [
  'Test.cpp'
]
src_test += src_common

library('FPGAObfuscator', src_common)
executable('Test', src_test)
