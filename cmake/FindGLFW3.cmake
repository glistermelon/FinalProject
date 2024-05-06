include(FindPackageHandleStandardArgs)

find_library(GLFW3_LIBRARY NAMES GLFW3)
find_path(GLFW3_INCLUDE_DIR NAMES GLFW/glfw3.h)

find_package_handle_standard_args(GLFW3 REQUIRED_VARS GLFW3_LIBRARY GLFW3_INCLUDE_DIR)

if (GLFW3_FOUND)
    mark_as_advanced(GLFW3_LIBRARY)
    mark_as_advanced(GLFW3_INCLUDE_DIR)
endif()

if (GLFW3_FOUND AND NOT TARGET GLFW3::glfw)
    add_library(GLFW3::glfw IMPORTED STATIC)
    set_property(TARGET GLFW3::glfw PROPERTY IMPORTED_LOCATION ${GLFW3_LIBRARY})
endif()