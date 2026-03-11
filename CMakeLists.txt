cmake_minimum_required(VERSION 3.16)
project(envhub_app C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_subdirectory(src/ui)

add_executable(envhub-app
  src/main.c
  src/services/power_service.c
  src/services/sensor_service.c
  src/services/status_service.c
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