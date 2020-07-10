# CMakeLists.txt
# --------------------------------------------------------------
# History:
#
# Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-10
# --------------------------------------------------------------

include_directories(include)
file(GLOB headers include/*.hh)
file(GLOB sources src/*.cc)
add_library(SAC ${headers} ${sources})
