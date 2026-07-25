
if (NOT EXISTS ${SOURCE_DIR})
	return()
endif()

file(GLOB_RECURSE all_items LIST_DIRECTORIES true RELATIVE ${SOURCE_DIR} ${SOURCE_DIR}/*)

foreach(item ${all_items})
	if(IS_DIRECTORY ${SOURCE_DIR}/${item})
		file(MAKE_DIRECTORY ${DEST_DIR}/${item})
	endif()
endforeach()

message(STATUS "Assets directory mirrored from ${SOURCE_DIR} to ${DEST_DIR}")