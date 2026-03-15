cmake_minimum_required(VERSION 3.16)
project(envhub_app C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Export compile_commands.json for clang-tidy / editor tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)

# ------------------------------------------------------------------------------
# Compiler warnings
# ------------------------------------------------------------------------------
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wformat=2
    -Wundef
    -Wstrict-prototypes
    -Wmissing-prototypes
    -Werror=implicit-function-declaration
    -Wdouble-promotion
    -Wconversion
    -Wsign-conversion
  )
endif()

add_subdirectory(src/ui)

add_executable(envhub-app
  src/main.c
  src/power_ctrl.c
  src/sensors.c
  src/status.c
  src/data_logger.c
  src/touch_input.c
)

target_include_directories(envhub-app
  PRIVATE
    src
    src/include
)

if(EXISTS "${CMAKE_SYSROOT}/usr/include/lvgl/lv_conf.h")
  target_include_directories(envhub-app
    PRIVATE
      "${CMAKE_SYSROOT}/usr/include"
      "${CMAKE_SYSROOT}/usr/include/libdrm"
  )

  target_compile_definitions(envhub-app
    PRIVATE
      _DEFAULT_SOURCE
      LV_CONF_PATH="${CMAKE_SYSROOT}/usr/include/lvgl/lv_conf.h"
  )
endif()

target_link_libraries(envhub-app
  PRIVATE
    lib-ui
    Threads::Threads
    drm
    gpiod
)

install(TARGETS envhub-app DESTINATION /usr/bin)

# ------------------------------------------------------------------------------
# Lint / format support
# ------------------------------------------------------------------------------

find_program(CLANG_FORMAT_EXE NAMES clang-format)
find_program(CLANG_TIDY_EXE NAMES clang-tidy)

file(GLOB_RECURSE ENVHUB_ALL_C_FILES
  "${CMAKE_SOURCE_DIR}/src/*.c"
  "${CMAKE_SOURCE_DIR}/src/*.h"
)

# Exclude generated and preview files/directories
set(ENVHUB_LINT_FILES "${ENVHUB_ALL_C_FILES}")

list(FILTER ENVHUB_LINT_FILES EXCLUDE REGEX ".*/preview-build/.*")
list(FILTER ENVHUB_LINT_FILES EXCLUDE REGEX ".*/preview-bin/.*")
list(FILTER ENVHUB_LINT_FILES EXCLUDE REGEX ".*/.*_gen\\.[ch]$")

if(CLANG_FORMAT_EXE)
  add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXE} -i ${ENVHUB_LINT_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Formatting EnvHub source files with clang-format"
  )
endif()

if(CLANG_TIDY_EXE)
  add_custom_target(lint
    COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} ${ENVHUB_LINT_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Linting EnvHub source files with clang-tidy"
  )
endif()