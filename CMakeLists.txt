cmake_minimum_required(VERSION 3.12)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(TodoingLib C)
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

#project(TodoingCLI C)
#add_executable(metronome-cli
#    source/metronome-cli.c
#)
#target_include_directories(metronome-cli PRIVATE
#    3rd-party/miniaudio
#    3rd-party/cjson
#)
#if(UNIX)
#    target_link_libraries(metronome-cli PRIVATE
#        metronome
#        m
#        pthread
#    )
#endif()

project(Todoing_TUI C)
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
