cmake_minimum_required(VERSION 2.8.6)
project(engine)
add_compile_options(-std=gnu++11)
add_compile_options(-Wdeprecated-declarations)
SET(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        #	FORCE
        )

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

find_package(DART REQUIRED COMPONENTS optimizer-ipopt CONFIG)
add_compile_options(-DHAVE_CSTDDEF)
include_directories(${DART_INCLUDE_DIRS})

file(GLOB srcs "*.h" "*.cpp")

add_library(engine SHARED ${srcs})
target_link_libraries(engine ${DART_LIBRARIES})
