cmake_minimum_required(VERSION 3.10)
project(untitled_project)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR})

add_executable(untitled_project
		src/main.c
		src/window_api.h
		src/win32/window_api_win32.c
)
