cmake_minimum_required(VERSION 3.6)
project(sumOfNumberSeries C)

set(CMAKE_C_STANDARD 11)

find_package(MPI REQUIRED)

add_executable(sumOfNumberSeries main.c)

target_link_libraries(sumOfNumberSeries ${MPI_LIBRARIES})

include_directories(${MPI_INCLUDE_PATH})

if(MPI_COMPILE_FLAGS)
    set_target_properties(sumOfNumberSeries PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()
if(MPI_LINK_FLAGS)
    set_target_properties(sumOfNumberSeries PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()