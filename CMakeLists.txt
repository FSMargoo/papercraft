cmake_minimum_required(VERSION 3.29)
project(PaperCraft)

set(CMAKE_CXX_STANDARD 23)
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
        include/game/object/Object.h
        source/game/component/Object.cpp
        include/SkiaPaperBase.h
        source/SkiaPaperBase.cpp
        include/renderer/BlockRender.h
        source/renderer/BlockRender.cpp
        include/renderer/Camera.h
        source/renderer/Camera.cpp
        include/renderer/LightRenderer.h
        source/renderer/LightRenderer.cpp
        include/renderer/Shader.h
        source/renderer/Shader.cpp
        include/renderer/BloomRender.h
        source/renderer/BloomRender.cpp
        include/renderer/BlendRender.h
        source/renderer/BlendRender.cpp
        include/renderer/Renderer.h
        source/renderer/Renderer.cpp
        include/game/component/LightSourceComponent.h
        include/game/component/CollisionComponent.h
        source/game/component/CollisionComponent.cpp
        include/game/manager/CollisionManager.h
        source/game/manager/CollisionManager.cpp
        include/game/component/VelocityComponent.h
        include/game/component/CollisionReactionComponent.h
        include/game/component/PauseCollisionComponent.h
        source/game/component/CollisionReactionComponent.cpp
        source/game/component/PauseCollisionComponent.cpp
        source/game/component/VelocityComponent.cpp
        include/game/manager/BlockIDMapManager.h
        source/game/manager/BlockIDMapManager.cpp
        include/game/chunk/Section.h
        source/game/chunk/Section.cpp
        include/game/chunk/IChunkProvider.h
        include/game/chunk/Chunk.h
        source/game/chunk/Chunk.cpp
        include/game/chunk/ChunkSerialization.h
        source/game/chunk/ChunkSerialization.cpp)

target_link_libraries(PaperCraft PRIVATE LibPRenderer)

add_executable(TestBlockCombination tester/TestBlockCombinationRendering.cpp ${PaperHeader} ${PaperSource})
target_link_libraries(TestBlockCombination PRIVATE LibPRenderer)

add_executable(TestLightMaskRendering tester/TestLightMaskRendering.cpp ${PaperHeader} ${PaperSource})
target_link_libraries(TestLightMaskRendering PRIVATE LibPRenderer)
