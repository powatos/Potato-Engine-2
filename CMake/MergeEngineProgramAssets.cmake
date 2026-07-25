
file(MAKE_DIRECTORY ${OUT_DIR})

set(engine_files "")
if (EXISTS ${ENGINE_DIR})
	file(GLOB_RECURSE engine_files LIST_DIRECTORIES false RELATIVE ${ENGINE_DIR} ${ENGINE_DIR}/*)
endif()

set(game_files "")
if (EXISTS ${GAME_DIR})
	file(GLOB_RECURSE game_files LIST_DIRECTORIES false RELATIVE ${GAME_DIR} ${GAME_DIR}/*)
endif()

foreach(item ${engine_files})
	if (item IN_LIST game_files)
		message(WARNING "Asset Override: '${item}' overrides engine default asset")
	else()
		get_filename_component(dest_dir ${OUT_DIR}/${item} DIRECTORY)
		file(MAKE_DIRECTORY ${dest_dir})
		file(COPY_FILE ${ENGINE_DIR}/${item} ${OUT_DIR}/${item} ONLY_IF_DIFFERENT)
	endif()
endforeach()

foreach(item ${game_files})
	get_filename_component(dest_dir ${OUT_DIR}/${item} DIRECTORY)
	file(MAKE_DIRECTORY ${dest_dir})
	file(COPY_FILE ${GAME_DIR}/${item} ${OUT_DIR}/${item} ONLY_IF_DIFFERENT)
endforeach()

message(STATUS "Program assets merge into engine assets complete")