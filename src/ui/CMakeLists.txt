# This basic file is used to compile the runtime for the Editor preview.
# Keep preview target name exactly as expected.

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
  set(UI_SOURCES ${LV_EDITOR_PROJECT_SOURCES})
elseif(DEFINED PROJECT_SOURCES AND NOT "${PROJECT_SOURCES}" STREQUAL "")
  set(UI_SOURCES ${PROJECT_SOURCES})
endif()

list(APPEND UI_SOURCES ${EXTRA_SOURCES})

message(STATUS "LV_EDITOR_PROJECT_SOURCES = ${LV_EDITOR_PROJECT_SOURCES}")
message(STATUS "PROJECT_SOURCES          = ${PROJECT_SOURCES}")
message(STATUS "EXTRA_SOURCES            = ${EXTRA_SOURCES}")
message(STATUS "UI_SOURCES (final)       = ${UI_SOURCES}")

if("${UI_SOURCES}" STREQUAL "")
  message(FATAL_ERROR "UI_SOURCES is empty.")
endif()

add_library(${LIB_NAME} ${UI_SOURCES})

target_include_directories(${LIB_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/screens
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
)

if(TARGET lvgl)
  target_link_libraries(${LIB_NAME} PUBLIC lvgl)
endif()