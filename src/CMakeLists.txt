# ----------------------------------------
#  src/CMakeLists.txt for Wake-On-Lan
# ----------------------------------------

include_directories( ${PROJECT_SOURCE_DIR}/src )

add_executable( wol wol.cc )
target_compile_options(
    wol PRIVATE -Wall -Wextra -Wno-missing-field-initializers
    $<$<CONFIG:Release>:-O2 -DNDEBUG >
    $<$<CONFIG:Debug>:-ggdb3 >
)
target_compile_features( wol PRIVATE cxx_std_20 )

add_custom_target( docs doxygen Doxyfile		# make documents
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/docs
)
add_custom_target( tags gtags				# make tags
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src
)

# -- additinal clean files
set_property( TARGET wol APPEND
  PROPERTY ADDITIONAL_CLEAN_FILES ${PROJECT_SOURCE_DIR}/src/GTAGS
)
set_property( TARGET wol APPEND
  PROPERTY ADDITIONAL_CLEAN_FILES ${PROJECT_SOURCE_DIR}/src/GPATH
)
set_property( TARGET wol APPEND
  PROPERTY ADDITIONAL_CLEAN_FILES ${PROJECT_SOURCE_DIR}/src/GRTAGS
)
set_property( TARGET wol APPEND
  PROPERTY ADDITIONAL_CLEAN_FILES ${PROJECT_SOURCE_DIR}/docs/html/
)

# ----------------------------------------
#  end of file <src/CMakeLists.txt>
# ----------------------------------------
