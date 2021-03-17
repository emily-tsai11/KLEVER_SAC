include_directories(include)
file(GLOB headers include/*.hh)
file(GLOB sources src/*.cc)
add_library(SAC ${headers} ${sources})
