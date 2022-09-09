# ----------------------------------------
#  src/CMakeLists.txt for Wake-On-Lan
# ----------------------------------------

include_directories( ${MyProject_SOURCE_DIR}/src )

add_executable( wol wol.cc )
set( CMAKE_CXX_FLAGS "-Wall -O2 -std=c++20" )

# ----------------------------------------
#  end of file <src/CMakeLists.txt>
# ----------------------------------------
