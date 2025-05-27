cmake_minimum_required(VERSION 3.10)
project(untitled_project)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_executable(untitled_project src/main.c src/graphics.h src/graphics.c
		src/vwnd.h src/screen.h src/screen.c resource.rc)

target_include_directories(untitled_project PRIVATE res)
