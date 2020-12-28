# require minimum cmake version
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

#define project
project(KLMC_SAC)

# find Geant4, activate all available UI and vis drivers by default
# can set WITH_GEANT4_UIVIS to OFF with cmake (-DWITH_GEANT4_UIVIS=OFF)
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
	find_package(Geant4 REQUIRED ui_all vis_all)
else()
	find_package(Geant4 REQUIRED)
endif()

# configure KLMC_SAC to use Geant4 and B1 headers
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# locate headers and sources for this project
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

# add the executable, and link it to the Geant4 libraries
add_executable(KLMC_SAC KLMC_SAC.cc ${headers} ${sources})
target_link_libraries(KLMC_SAC ${Geant4_LIBRARIES})

# add Physics to build
add_subdirectory(${PROJECT_SOURCE_DIR}/Physics) # add Physics subdirectory to build
include_directories(${PROJECT_SOURCE_DIR}/Physics/include) # specify Physics include directory
target_link_libraries(KLMC_SAC Physics) # link with KLMC_SAC libraries

# add PMT to build
# add_subdirectory(${PROJECT_SOURCE_DIR}/PMT) # add PMT subdirectory to build
# include_directories(${PROJECT_SOURCE_DIR}/PMT/include) # specify PMT include directory
# target_link_libraries(KLMC_SAC PMT) # link with KLMC_SAC libraries

# add SAC to build
add_subdirectory(${PROJECT_SOURCE_DIR}/SAC) # add SAC subdirectory to build
include_directories(${PROJECT_SOURCE_DIR}/SAC/include) # specify SAC include directory
target_link_libraries(KLMC_SAC SAC) # link with KLMC_SAC libraries

# copy over macro files
file(COPY macros DESTINATION .)

# install executable
install(TARGETS KLMC_SAC DESTINATION bin)
