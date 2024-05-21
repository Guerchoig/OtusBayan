cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "0" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bayan VERSION ${PROJECT_VESRION})

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )
# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# configure_file(config.h.in config.h)
set(Boost_USE_STATIC_LIBS ON)

FIND_PACKAGE( Boost COMPONENTS program_options filesystem regex REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
add_executable(bayan bayan.cpp)
# add_library(bayan_lib bayan_lib.cpp)
# add_executable(test_bayan test_bayan.cpp)

set_target_properties(bayan  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(bayan
    PRIVATE "${CMAKE_BINARY_DIR}"
)

TARGET_LINK_LIBRARIES( bayan PUBLIC ${Boost_LIBRARIES} )


# target_link_libraries(bayan PRIVATE bayan_lib)
# target_link_libraries(test_bayan
#     GTest::gtest_bayan
#     bayan_lib
# )

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
    #  target_compile_options(test_bayan PRIVATE
    #     /W4
    # )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    
endif()



install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT alex-guerchoig@yandex.ru)

include(CPack)



# enable_testing()
# include(GoogleTest)
# gtest_discover_tests(test_bayan)
# add_test(test_bayan  test_bayan)



