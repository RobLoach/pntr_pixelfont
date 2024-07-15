# Add the cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/.cmake)

find_package(pntr)

# pntr_pixelfont_test
add_executable(pntr_pixelfont_test pntr_pixelfont_test.c)
target_link_libraries(pntr_pixelfont_test PUBLIC
    pntr
    pntr_pixelfont
)
set_property(TARGET pntr_pixelfont_test PROPERTY C_STANDARD 11)

# Set up the test
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(NAME pntr_pixelfont_test COMMAND pntr_pixelfont_test)
