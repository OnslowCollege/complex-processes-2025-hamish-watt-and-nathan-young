cmake_minimum_required(VERSION 3.10)

project(kdemulate C ASM_NASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(ASM_SOURCES src/asm/fillcolor.asm src/asm/fillgradient.asm)
set(C_SOURCES src/main.c src/graphics.c src/screen.c src/vwnd.c src/utils.c src/msg.c src/elements/elements.c src/applications/applications.c src/applications/notepad.c src/applications/kfind.c src/applications/taskbar.c src/applications/desktop.c src/applications/topbar.c src/applications/welcome.c resource.rc)

add_executable(kdemulate ${C_SOURCES} ${ASM_SOURCES})

set_target_properties(kdemulate PROPERTIES ASM_NASM_OBJECT_FORMAT win32)
set(CMAKE_ASM_NASM_FLAGS "-f win32")

option(DEBUG "Show debug information" OFF)

if(DEBUG)
    target_compile_definitions(kdemulate PRIVATE DEBUG)
endif()

if(CMAKE_HOST_WIN32)
    # run natively if on windows
    add_custom_target(run
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kdemulate.exe
        DEPENDS kdemulate
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
else()
    # otherwise attempt to run through wine
    add_custom_target(run
        COMMAND wine ${CMAKE_CURRENT_BINARY_DIR}/kdemulate.exe
        DEPENDS kdemulate
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

add_custom_target(iso
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/buildiso.sh
    DEPENDS kdemulate
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(kdemulate PRIVATE res)
