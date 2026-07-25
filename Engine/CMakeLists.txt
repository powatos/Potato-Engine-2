# Engine

find_package(SDL3 REQUIRED CONFIG)
find_package(nlohmann_json REQUIRED CONFIG)
find_package(fmt REQUIRED CONFIG)
find_package(utf8cpp REQUIRED CONFIG)

add_library(PotatoEngine2 STATIC)

target_include_directories(PotatoEngine2
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory(src/Core)
add_subdirectory(src/Debug)
add_subdirectory(src/Game)
add_subdirectory(src/Systems)
add_subdirectory(src/UI)
add_subdirectory(src/Util)

target_link_libraries(PotatoEngine2
    PUBLIC
        fmt::fmt
    PRIVATE
        SDL3::SDL3
        nlohmann_json::nlohmann_json
        utf8cpp::utf8cpp
)

target_compile_features(PotatoEngine2 PUBLIC cxx_std_20)

function(ConfigureAssets target)

	execute_process(
		COMMAND ${CMAKE_COMMAND}
			-DSOURCE_DIR=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Assets
			-DDEST_DIR=${CMAKE_CURRENT_SOURCE_DIR}/Assets
			-P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../CMake/MirrorAssetsDirectory.cmake
	)

	file(GLOB_RECURSE engine_asset_files LIST_DIRECTORIES false
		${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Assets/*
	)
	file(GLOB_RECURSE game_asset_files LIST_DIRECTORIES false
		${CMAKE_CURRENT_SOURCE_DIR}/Assets/*
	)

	set(stamp ${CMAKE_CURRENT_BINARY_DIR}/merged_assets.stamp)

	add_custom_command(
		OUTPUT ${stamp}
		COMMAND ${CMAKE_COMMAND}
			-DENGINE_DIR=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Assets
			-DGAME_DIR=${CMAKE_CURRENT_SOURCE_DIR}/Assets
			-DOUT_DIR=$<TARGET_FILE_DIR:${target}>/Assets
			-P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../CMake/MergeEngineProgramAssets.cmake
		COMMAND ${CMAKE_COMMAND} -E touch ${stamp}
		DEPENDS ${engine_asset_files} ${game_asset_files}
	)

	add_custom_target(${target}_assets DEPENDS ${stamp})
	add_dependencies(${target} ${target}_assets)
endfunction()

