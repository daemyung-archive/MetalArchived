#
# This file is part of the "Metal" project
# See "LICENSE" for license information.
#

add_library(external
    STATIC include/imconfig.h
           include/imgui.h
           include/imgui_impl_metal.h
           include/imgui_impl_osx.h
           include/fmt/core.h
           include/fmt/format.h
           include/fmt/format-inl.h
               src/imgui_internal.h
               src/imstb_truetype.h
               src/imstb_textedit.h
               src/imstb_rectpack.h
               src/imgui.cpp
               src/imgui_draw.cpp
               src/imgui_widgets.cpp
               src/imgui_demo.cpp
               src/imgui_impl_metal.mm
               src/imgui_impl_osx.mm
               src/format.cc)

target_include_directories(external
    PUBLIC include)

target_compile_features(external
    PUBLIC cxx_variadic_templates)


