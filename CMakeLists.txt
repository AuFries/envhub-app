cmake_minimum_required(VERSION 3.16)
project(envhub_app C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_subdirectory(src/ui)

add_executable(envhub-app
  src/main.c
  src/services/time_service.c
)

target_include_directories(envhub-app
  PRIVATE
    src
    src/include
)

target_link_libraries(envhub-app
  PRIVATE
    envhub-ui
    Threads::Threads
    drm
)