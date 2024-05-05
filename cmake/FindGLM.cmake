include(FindPackageHandleStandardArgs)

find_library(GLM_LIBRARY NAMES GLM)
find_path(GLM_INCLUDE_DIR NAMES GLM/GLM.h)

find_package_handle_standard_args(GLM REQUIRED_VARS GLM_LIBRARY GLM_INCLUDE_DIR)

if (GLM_FOUND)
    mark_as_advanced(GLM_LIBRARY)
    mark_as_advanced(GLM_INCLUDE_DIR)
endif()

if (GLM_FOUND AND NOT TARGET GLM::glm)
    add_library(GLM::glm IMPORTED STATIC)
    set_property(TARGET GLM::glm PROPERTY IMPORTED_LOCATION ${GLM_LIBRARY})
endif()