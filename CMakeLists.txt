cmake_minimum_required(VERSION 3.12)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(TodoingLib C)

if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

add_library(todoing-txt
    source/todoing-txt.c
)
target_include_directories(todoing-txt PRIVATE
    #3rd-party/
)
if(UNIX)
    target_link_libraries(todoing-txt PRIVATE
        m
        pthread
    )
endif()

project(Todoing_TUI C)
if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

add_executable(todoingtxt-tui
    source/todoing-txt-tui.c
)
target_include_directories(todoingtxt-tui PRIVATE
    3rd-party/miniaudio
    3rd-party/cjson
)
if(UNIX)
    target_link_libraries(todoingtxt-tui PRIVATE
        todoing-txt
        ncurses
        m
        pthread
    )
endif()
