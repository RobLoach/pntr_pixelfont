# Add the cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)

find_package(pntr)

# pntr_pixelfonts_test
add_executable(pntr_pixelfonts_test pntr_pixelfonts_test.c)
target_link_libraries(pntr_pixelfonts_test PUBLIC
    pntr
    pntr_pixelfonts
)
set_property(TARGET pntr_pixelfonts_test PROPERTY C_STANDARD 11)

# Set up the test
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(NAME pntr_pixelfonts_test COMMAND pntr_pixelfonts_test)
