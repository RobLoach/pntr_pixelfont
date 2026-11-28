find_package(pntr)

# pntr_pixelfont_test
add_executable(pntr_pixelfont_test pntr_pixelfont_test.c)
target_link_libraries(pntr_pixelfont_test PUBLIC
    pntr
    pntr_pixelfont
)
set_property(TARGET pntr_pixelfont_test PROPERTY C_STANDARD 99)

if(MSVC)
    target_compile_options(pntr_pixelfont_test PRIVATE /W4 /WX)
else()
    target_compile_options(pntr_pixelfont_test PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Set up the test
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(NAME pntr_pixelfont_test COMMAND pntr_pixelfont_test)

# pntr_pixelfont_utf8_test: the same fonts with pntr's PNTR_ENABLE_UTF8.
add_executable(pntr_pixelfont_utf8_test pntr_pixelfont_utf8_test.c)
target_link_libraries(pntr_pixelfont_utf8_test PUBLIC
    pntr
    pntr_pixelfont
)
set_property(TARGET pntr_pixelfont_utf8_test PROPERTY C_STANDARD 99)

if(MSVC)
    target_compile_options(pntr_pixelfont_utf8_test PRIVATE /W4 /WX)
else()
    target_compile_options(pntr_pixelfont_utf8_test PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_test(NAME pntr_pixelfont_utf8_test COMMAND pntr_pixelfont_utf8_test)
