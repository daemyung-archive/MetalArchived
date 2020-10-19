#
# This file is part of the "Metal" project
# See "LICENSE" for license information.
#

add_library(external
    STATIC include/fmt/core.h
           include/fmt/format.h
           include/fmt/format-inl.h
               src/format.cc)

target_include_directories(external
    PUBLIC include)

target_compile_features(external
    PUBLIC cxx_variadic_templates)


