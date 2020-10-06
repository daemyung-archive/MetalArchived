#
# This file is part of the "Metal" project
# See "LICENSE" for license information.
#

add_executable(template src/template.cpp)

target_link_libraries(template
    PUBLIC common)