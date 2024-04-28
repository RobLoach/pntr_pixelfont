cmake_minimum_required(VERSION 3.11)
project(pntr_pixelfonts
    DESCRIPTION "pntr_pixelfonts"
    HOMEPAGE_URL "https://github.com/robloach/pntr_pixelfonts"
    VERSION 0.0.1
    LANGUAGES C
)

# pntr_pixelfonts
add_library(pntr_pixelfonts INTERFACE)
target_include_directories(pntr_pixelfonts INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(PNTR_PIXELFONTS_IS_MAIN TRUE)
else()
    set(PNTR_PIXELFONTS_IS_MAIN FALSE)
endif()

option(PNTR_PIXELFONTS_BUILD_TESTS "pntr_pixelfonts: Tests" ${PNTR_PIXELFONTS_IS_MAIN})

if (PNTR_PIXELFONTS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Testing
if (PNTR_PIXELFONTS_BUILD_TESTS)
    include(CTest)
    enable_testing()
    if (BUILD_TESTING)
        set(CTEST_CUSTOM_TESTS_IGNORE
            pkg-config--static
        )
        # Always print verbose output when tests fail if run using `make test`.
        list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
        add_subdirectory(test)
    endif()
endif()
