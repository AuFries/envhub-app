# This basic file is used to compile the runtime for the Editor preview.
# Generated file, do not edit.

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  cmake_minimum_required(VERSION 3.10)
  project(LVGLProject)
endif()

# Target name for the UI library
if (DEFINED LVGL_COMPONENT_LIB_NAME)
  set(LIB_NAME ${LVGL_COMPONENT_LIB_NAME})
else()
  set(LIB_NAME lib-ui)
endif()

# Pull in the generated list of .c files (variable name depends on editor version)
include(${CMAKE_CURRENT_LIST_DIR}/file_list_gen.cmake)

# Collect your additional sources (fonts, aliases, etc.)
file(GLOB_RECURSE EXTRA_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_LIST_DIR}/fonts/*.c"
)

# Determine which variable the generator populated
set(UI_SOURCES "")
if(DEFINED LV_EDITOR_PROJECT_SOURCES AND NOT "${LV_EDITOR_PROJECT_SOURCES}" STREQUAL "")
  set(UI_SOURCES ${LV_EDITOR_PROJECT_SOURCES})
elseif(DEFINED PROJECT_SOURCES AND NOT "${PROJECT_SOURCES}" STREQUAL "")
  set(UI_SOURCES ${PROJECT_SOURCES})
endif()

# Append extras
list(APPEND UI_SOURCES ${EXTRA_SOURCES})

# Debug prints (you WANT these right now)
message(STATUS "LV_EDITOR_PROJECT_SOURCES = ${LV_EDITOR_PROJECT_SOURCES}")
message(STATUS "PROJECT_SOURCES          = ${PROJECT_SOURCES}")
message(STATUS "EXTRA_SOURCES            = ${EXTRA_SOURCES}")
message(STATUS "UI_SOURCES (final)       = ${UI_SOURCES}")

# Ensure we actually have UI code, not just fonts
if("${UI_SOURCES}" STREQUAL "" OR UI_SOURCES STREQUAL EXTRA_SOURCES)
  message(FATAL_ERROR "UI_SOURCES is empty (or only extras). file_list_gen.cmake did not provide generated UI sources.")
endif()

add_library(${LIB_NAME} ${UI_SOURCES})

if(TARGET lvgl)
  target_link_libraries(${LIB_NAME} PUBLIC lvgl)
endif()

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${LIB_NAME} PRIVATE ${COMMON_INCLUDE_DIRS})