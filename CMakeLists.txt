cmake_minimum_required(VERSION 3.10)

project(untitled_project C ASM_NASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(ASM_SOURCES src/fillcolor.asm)
set(C_SOURCES src/main.c src/graphics.c src/screen.c resource.rc)


add_executable(untitled_project ${C_SOURCES} ${ASM_SOURCES})

set_target_properties(untitled_project PROPERTIES ASM_NASM_OBJECT_FORMAT win32)
set(CMAKE_ASM_NASM_FLAGS "-f win32")

target_include_directories(untitled_project PRIVATE res)
