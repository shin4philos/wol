# ----------------------------------------
#  src/CMakeLists.txt for Wake-On-Lan
# ----------------------------------------

include_directories( ${PROJECT_SOURCE_DIR}/src )

add_executable( wol wol.cc )
add_custom_target( docs doxygen Doxyfile
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/docs
)

target_compile_options( wol PRIVATE -Wall -Wextra -Wno-missing-field-initializers
    $<$<CONFIG:Release>:-O2 -DNDEBUG >
    $<$<CONFIG:Debug>:-ggdb3 >
)
target_compile_features( wol PRIVATE cxx_std_20 )

# ----------------------------------------
#  end of file <src/CMakeLists.txt>
# ----------------------------------------
