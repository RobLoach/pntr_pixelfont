find_package(pntr)

# pntr_pixelfont_example
add_executable(pntr_pixelfont_example pntr_pixelfont_example.c)
target_link_libraries(pntr_pixelfont_example PUBLIC
    pntr
    pntr_pixelfont
)
set_property(TARGET pntr_pixelfont_example PROPERTY C_STANDARD 99)

if(MSVC)
    target_compile_options(pntr_pixelfont_example PRIVATE /W4 /WX)
else()
    target_compile_options(pntr_pixelfont_example PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
