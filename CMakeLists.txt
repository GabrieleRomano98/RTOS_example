# Each Makefile lines are reported in this file with "->" 
# and the CMake implementation follows

project(PdS_project)
cmake_minimum_required(VERSION 3.5)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build)
set(CMAKE_VERBOSE_MAKEFILE ON)


########## FreeRTOS library definition ##########
include_directories(
    .
    Source/include
    Source/portable
)
FILE(GLOB FREERTOS_SOURCES
    Source/*.c
)
list(APPEND FREERTOS_SOURCES "Source/portable/MemMang/heap_4.c")
list(APPEND FREERTOS_SOURCES "Source/portable/port.c")
##############################################


########## Plant library definition ##########
include_directories(
    Plant_system
)
FILE(GLOB PLANT_SOURCES
    Plant_system/*.cpp
)
##############################################


########## Control library definition ##########
include_directories(
    ControlTasks
)
FILE(GLOB CONTROL_SOURCES
    ControlTasks/*.cpp
)
##############################################


########## Interface library definition ##########
include_directories(
    Interface
)
FILE(GLOB INTERF_SOURCES
    Interface/*.cpp
)
##############################################


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb3 -O0")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -ggdb3 -O0")


# Create executable with demo code files
add_executable(${PROJECT_NAME}
    PdS_project.cpp
    FreeRTOSConfig.h
    PdS_project.h
 )

# Link FreeRTOS and plant as static libraries
add_library(freertos STATIC ${FREERTOS_SOURCES} )
add_library(plant STATIC ${PLANT_SOURCES} )
add_library(control STATIC ${CONTROL_SOURCES} )
add_library(interf STATIC ${INTERF_SOURCES} )
target_link_libraries(${PROJECT_NAME} freertos plant control interf)