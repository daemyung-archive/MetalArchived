#
# This file is part of the "Metal" project
# See "LICENSE" for license information.
#

add_library(common
    STATIC include/common/utility.h
           include/common/window.h
           include/common/example.h
               src/window.cpp
               src/example.cpp)

target_include_directories(common
    PUBLIC  include
    PRIVATE include/common)

target_compile_features(common
    PUBLIC cxx_std_20)

target_compile_options(common
    PUBLIC
        -fobjc-arc
        -xobjective-c++)

target_link_libraries(common
    PUBLIC external
           "-framework AppKit"
           "-framework QuartzCore"
           "-framework Metal")
