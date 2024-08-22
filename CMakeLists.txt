cmake_minimum_required(VERSION 3.29)
project(PaperCraft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

add_link_options(/SUBSYSTEM:WINDOWS)
add_subdirectory(./thirdparty/PaperRenderer)

add_compile_definitions(UNICODE _UNICODE)
add_compile_options(/wd4067 /wd4819)

file(GLOB_RECURSE PaperHeader "./include/*.h" "./include/*/*.h")
file(GLOB_RECURSE PaperSource "./source/*/*.cpp" "./source/*.cpp")

include_directories(./)
include_directories(./thirdparty/)
include_directories(./thirdparty/mINI/src)

add_executable(PaperCraft main.cpp ${PaperHeader} ${PaperSource}
        include/game/component/Object.h
        source/game/component/Object.cpp
        include/SkiaPaperBase.h
        source/SkiaPaperBase.cpp
        include/renderer/BlockRender.h)

target_link_libraries(PaperCraft PRIVATE LibPRenderer)