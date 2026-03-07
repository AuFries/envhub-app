if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  cmake_minimum_required(VERSION 3.10)
  project(LVGLProject C)
endif()

set(LIB_NAME lib-ui)

include(${CMAKE_CURRENT_LIST_DIR}/file_list_gen.cmake)

file(GLOB_RECURSE EXTRA_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_LIST_DIR}/fonts/*.c"
)

set(UI_SOURCES "")
if(DEFINED LV_EDITOR_PROJECT_SOURCES AND NOT "${LV_EDITOR_PROJECT_SOURCES}" STREQUAL "")
  # LVGL Editor / preview build path
  set(UI_SOURCES ${LV_EDITOR_PROJECT_SOURCES})
elseif(DEFINED PROJECT_SOURCES AND NOT "${PROJECT_SOURCES}" STREQUAL "")
  # Normal generated-project path
  set(UI_SOURCES ${PROJECT_SOURCES})
endif()

list(APPEND UI_SOURCES ${EXTRA_SOURCES})

if("${UI_SOURCES}" STREQUAL "")
  message(FATAL_ERROR "UI_SOURCES is empty.")
endif()

add_library(${LIB_NAME} STATIC ${UI_SOURCES})

target_include_directories(${LIB_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/screens
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
)

#
# Preview-editor path:
# If the editor already created an LVGL target, use it directly.
#
if(TARGET lvgl)
  target_link_libraries(${LIB_NAME} PUBLIC lvgl)
endif()

#
# Buildroot / target path:
# Use the installed LVGL from the sysroot.
#
if(EXISTS "${CMAKE_SYSROOT}/usr/include/lvgl/lv_conf.h")
  target_include_directories(${LIB_NAME}
    PUBLIC
      "${CMAKE_SYSROOT}/usr/include"
      "${CMAKE_SYSROOT}/usr/include/libdrm"
  )

  #
  # Force the correct lv_conf.h first, then your compatibility header.
  # Using SHELL:-include avoids CMake splitting the option weirdly.
  #
  target_compile_options(${LIB_NAME}
    PRIVATE
      "SHELL:-include ${CMAKE_SYSROOT}/usr/include/lvgl/lv_conf.h"
      "SHELL:-include ${CMAKE_CURRENT_SOURCE_DIR}/lvgl_ui_compat.h"
  )

  #
  # If we're not in preview mode, the lvgl target usually does not exist.
  # So find the installed library in the sysroot and link it.
  #
  if(NOT TARGET lvgl)
    find_library(LVGL_LIB
      NAMES lvgl liblvgl
      PATHS
        "${CMAKE_SYSROOT}/usr/lib"
        "${CMAKE_SYSROOT}/lib"
      NO_DEFAULT_PATH
    )

    if(NOT LVGL_LIB)
      message(FATAL_ERROR "Could not find LVGL library in sysroot")
    endif()

    target_link_libraries(${LIB_NAME} PUBLIC ${LVGL_LIB})
  endif()
endif()