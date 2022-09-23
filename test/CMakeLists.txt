# ----------------------------------------
#  test/CMakeLists.txt for Wake-On-Lan
# ----------------------------------------

enable_testing()

include_directories( ${PROJECT_SOURCE_DIR}/src )

add_executable( wol_test wol_test.cc )
target_link_libraries( wol_test GTest::gtest_main )

include( GoogleTest )
gtest_discover_tests( wol_test )

# ----------------------------------------
#  end of file <src/CMakeLists.txt>
# ----------------------------------------

