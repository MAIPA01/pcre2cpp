if(EXISTS ${CLANG_TIDY_CONFIG})
	file(SHA1 "${CLANG_TIDY_CONFIG}" CLANG_TIDY_HASH)
else()
	set(CLANG_TIDY_HASH "")
endif()

function(compute_tidy_hash SRC OUT_HASH)
    file(SHA1 "${SRC}" FILE_HASH)
	string(SHA1 FINAL_HASH "${FILE_HASH}${CLANG_TIDY_HASH}")
    set(${OUT_HASH} ${FINAL_HASH} PARENT_SCOPE)
endfunction()

if(NOT EXISTS ${CACHE_FILE})
	file(WRITE ${CACHE_FILE} "")
endif()

file(READ ${CACHE_FILE} CACHE_CONTENT)
string(REPLACE "\n" ";" CACHE_LINES "${CACHE_CONTENT}")

separate_arguments(SOURCES NATIVE_COMMAND "${SOURCES}")
separate_arguments(CLANG_TIDY_ARGS NATIVE_COMMAND "${CLANG_TIDY_ARGS}")

set(FILES_TO_TIDY)
set(CACHE_MAP)

message(STATUS "CLANG TIDY EXE: ${CLANG_TIDY_EXE}")

execute_process(
		COMMAND ${CLANG_TIDY_EXE}
				--version
		OUTPUT_VARIABLE CLANG_TIDY_VERSION_OUTPUT
)

string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" CLANG_TIDY_VERSION "${CLANG_TIDY_VERSION_OUTPUT}")

message(STATUS "CLANG TIDY VERSION: ${CLANG_TIDY_VERSION}")

# wczytaj cache do mapy
foreach(LINE ${CACHE_LINES})
	if(LINE MATCHES "(.+)\\|(.+)")
		set(FILE ${CMAKE_MATCH_1})
		set(HASH ${CMAKE_MATCH_2})
		set(CACHE_MAP_${FILE} ${HASH})
	endif()
endforeach()

if(FORCE_TIDY)
    message(STATUS "clang-tidy: forced run on all files")
    set(FILES_TO_TIDY ${SOURCES})
else()
	foreach(SRC ${SOURCES})
		if(NOT EXISTS ${SRC})
			continue()
		endif()
	
		compute_tidy_hash(${SRC} FILE_HASH)
		set(CACHED_HASH ${CACHE_MAP_${SRC}})

		if(NOT CACHED_HASH OR NOT CACHED_HASH STREQUAL FILE_HASH)
			list(APPEND FILES_TO_TIDY ${SRC})
		endif()
	endforeach()
endif()

if(FILES_TO_TIDY)
	list(LENGTH FILES_TO_TIDY FILES_LEN)

	message(STATUS "clang-tidy checking ${FILES_LEN} files:")
	foreach(F ${FILES_TO_TIDY})
		message(STATUS " ${F}")
	endforeach()
	
	set(YAML_FILE "${CMAKE_BINARY_DIR}/clang_tidy_fixes.yaml")

	# Execute clang-tidy
	execute_process(
        COMMAND ${CLANG_TIDY_EXE} 
				--config-file=${CLANG_TIDY_CONFIG}
				-p=${CLANG_COMPILE_COMMANDS_DIR}
				${CLANG_TIDY_ARGS}
				--export-fixes=${YAML_FILE}
				${FILES_TO_TIDY}
        RESULT_VARIABLE RES
    )

	if(NOT RES EQUAL 0)
		message(STATUS "clang-tidy found errors.")
	endif()

	# Check which files have warnings
	set(FILES_WITH_WARNINGS)

	if(EXISTS ${YAML_FILE})
		file(READ ${YAML_FILE} YAML_CONTENT)
		string(REGEX MATCHALL "FilePath: ([^\n]+)" MATCHES "${YAML_CONTENT}")

		foreach(M ${MATCHES})
			string(REGEX REPLACE "FilePath: " "" FILE "${M}")
			string(STRIP "${FILE}" FILE)

			# Remove YAML string apostrophes
			string(REPLACE "'" "" FILE "${FILE}")

			file(TO_CMAKE_PATH "${FILE}" FILE)
	 		list(APPEND FILES_WITH_WARNINGS ${FILE})
		endforeach()

		list(REMOVE_DUPLICATES FILES_WITH_WARNINGS)
	endif()

	# Set new hashes
	foreach(SRC ${FILES_TO_TIDY})
		get_filename_component(SRC_ABS ${SRC} ABSOLUTE)
		file(TO_CMAKE_PATH "${SRC_ABS}" SRC_ABS)
		list(FIND FILES_WITH_WARNINGS ${SRC_ABS} IDX)

		if(IDX EQUAL -1)
			compute_tidy_hash(${SRC} NEW_HASH)
			set(CACHE_MAP_${SRC} ${NEW_HASH})
		else()
			message(STATUS "clang-tidy warnings in ${SRC}")
		endif()
	endforeach()

	# Delete yaml file
	if(EXISTS ${YAML_FILE})
		file(REMOVE ${YAML_FILE})
	endif()

	# Clear cache file
	file(WRITE ${CACHE_FILE} "")

	# Update hashes
	foreach(SRC ${SOURCES})
		if(DEFINED CACHE_MAP_${SRC})
			file(APPEND ${CACHE_FILE} "${SRC}|${CACHE_MAP_${SRC}}\n")
		endif()
	endforeach()
else()
	message(STATUS "No files to tidy.")
endif() # zapisz cache