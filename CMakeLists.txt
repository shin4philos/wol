# ----------------------------------------
#  CMakeLists.txt for Wake-On-Lan
# ----------------------------------------

cmake_minimum_required( VERSION 3.14 )

project ( WakeOnLan )
set( CMAKE_CXX_STANDARD 20 )

include( FetchContent )
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
set( gtest_force_shared_crt ON CACHE BOOL "" FORCE )
FetchContent_MakeAvailable( googletest )
FetchContent_GetProperties( googletest )

enable_testing()
find_package( Threads REQUIRED )

set( GTEST_INCLUDE_DIR ${gtest_SOURCE_DIR}/include )
set( GMOCK_INCLUDE_DIR ${gmock_SOURCE_DIR}/include )
include_directories( ${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR})

add_custom_target( docs doxygen Doxyfile		# make documents
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/docs
)
add_custom_target( tags gtags				# make tags
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src
)
add_custom_target( coverage
  COMMAND lcov --capture --directory . --output-file all.info --quiet
  COMMAND lcov --extract all.info "/home/shin/git/wol/\\*" -o wol.info --quiet
  COMMAND lcov --remove wol.info "/home/shin/git/wol/build/\\*" -o notest.info --quiet
  COMMAND genhtml notest.info -o coverage --quiet
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/CMakeFiles/wol_test.dir
)

# -- additinal clean files
set_property( TARGET tags APPEND
  PROPERTY ADDITIONAL_CLEAN_FILES ${PROJECT_SOURCE_DIR}/src/GTAGS
)
set_property( TARGET tags APPEND
  PROPERTY ADDITIONAL_CLEAN_FILES ${PROJECT_SOURCE_DIR}/src/GPATH
)
set_property( TARGET tags APPEND
  PROPERTY ADDITIONAL_CLEAN_FILES ${PROJECT_SOURCE_DIR}/src/GRTAGS
)
set_property( TARGET tags APPEND
  PROPERTY ADDITIONAL_CLEAN_FILES ${PROJECT_SOURCE_DIR}/docs/html/
  )

add_subdirectory( src )
add_subdirectory( test )

# ----------------------------------------
#  end of file <CMakeLists.txt>
# ----------------------------------------
