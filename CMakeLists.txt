cmake_minimum_required(VERSION 3.28)
project(szklane-domy LANGUAGES CXX)

include_directories(src src/components src/events src/visuals src/imports src/commands src/data-management src/managers src/playerstates textures data shaders)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glew)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glm)

add_executable(szklane-domy src/main.cpp "src/game-object.h" "src/visuals/renderer.cpp" "src/components/component.h" "src/components/graphics-component.h" "src/components/graphics-component.cpp"   "src/game-object.cpp"  "src/components/message.h"   "src/components/component.cpp"  "src/game.h" "src/game.cpp" "src/visuals/renderer.h" "src/data-management/texture-data.h"  "src/visuals/camera.h" "src/visuals/camera.cpp" "src/events/event-observer.h" "src/events/event-subject.h" "src/events/event-subject.cpp" "src/grid.h" "src/grid.cpp" "src/managers/input-manager.h" "src/managers/input-manager.cpp" "src/components/tile-component.h" "src/building.h"  "src/data-management/data-parser.h" "src/data-management/data-parser.cpp" "src/imports/stb-image.h" "src/imports/stb-image-implementation.h" "src/city-data.h" "src/commands/command.h" "src/commands/place-building-command.h" "src/components/tile-component.cpp" "src/building.cpp" "src/commands/place-building-command.cpp" "src/commands/command-history.h" "src/commands/command-history.cpp" "src/managers/construction-manager.h" "src/managers/construction-manager.cpp" "src/data-management/building-data.h" "src/managers/icomponent-manager.h" "src/managers/tile-component-manager.h" "src/managers/tile-component-manager.cpp" "src/zone.h" "src/data-management/variable-data.h" "src/commands/remove-building-command.h" "src/commands/remove-building-command.cpp" "src/playerstates/player-state.h" "src/playerstates/building-player-state.h" "src/playerstates/building-player-state.cpp" "src/playerstates/zone-add-player-state.h" "src/player.h" "src/player.cpp" "src/playerstates/zone-add-player-state.cpp" "src/components/movement-component.h" "src/components/movement-component.cpp" "src/components/hover-component.h" "src/components/hover-component.cpp")
target_compile_features(szklane-domy PRIVATE cxx_std_17)
target_link_libraries(szklane-domy PRIVATE SFML::Graphics nlohmann_json::nlohmann_json libglew_static glm::glm)

# Tracks all shader files
file(GLOB_RECURSE SHADER_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*"
)

file(GLOB_RECURSE TEXTURE_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/textures/*"
)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/textures ${OUTPUT_DIR}/textures
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${OUTPUT_DIR}/data
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${OUTPUT_DIR}/shaders
    DEPENDS ${SHADER_FILES}
    COMMENT "Copying resources (textures, data, shaders) to output directory"
)

add_dependencies(szklane-domy copy_resources)

