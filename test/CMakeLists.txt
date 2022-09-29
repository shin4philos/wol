# ----------------------------------------
#  test/CMakeLists.txt for Wake-On-Lan
# ----------------------------------------

enable_testing()

include_directories( ${PROJECT_SOURCE_DIR}/src )

add_executable( wol_test wol_test.cc )

# define UNIT_TEST to evaluate it is unit test at compile time.

target_compile_options( wol_test PRIVATE -DUNIT_TEST=true
    $<$<CONFIG:Release>:-O2 -DNDEBUG >
    $<$<CONFIG:Debug>:-ggdb3 >
)
target_link_libraries( wol_test gtest_main gmock_main )

include( GoogleTest )
gtest_discover_tests( wol_test )

# ----------------------------------------
#  end of file <src/CMakeLists.txt>
# ----------------------------------------

