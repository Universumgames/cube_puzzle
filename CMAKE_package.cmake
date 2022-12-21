# architecture
if(WIN32)
    EXECUTE_PROCESS(COMMAND wmic os get osarchitecture OUTPUT_VARIABLE ARCHITECTURE_CMD_RAW)
    string(REPLACE "\r" "" ARCHITECTURE_CMD_CLEANR ${ARCHITECTURE_CMD_RAW})
    string(REPLACE " " "" ARCHITECTURE_CMD_NOSPACE ${ARCHITECTURE_CMD_CLEANR})
    string(REPLACE "\n" "_" ARCHITECTURE_CMD_NON ${ARCHITECTURE_CMD_NOSPACE})
    string(REPLACE "__" ";" ARCHITECTURE_LIST_1 ${ARCHITECTURE_CMD_NON})
    string(REPLACE "_" ";" ARCHITECTURE_LIST ${ARCHITECTURE_LIST_1})
    list(GET ARCHITECTURE_LIST 1 ARCHITECTURE)
else()
    EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
endif()

# get current git commit hash
execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# replace "." to "_" to fix executable name
string(REPLACE "." "_" FileVersion ${CMAKE_PROJECT_VERSION})

set(EXE_NAME "qube_puzzle_${CMAKE_SYSTEM_NAME}_${ARCHITECTURE}_${GIT_HASH}")

# create zip archive
file(MAKE_DIRECTORY packaged)
file(REMOVE_RECURSE prepared)
file(MAKE_DIRECTORY prepared/qube_game)
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/bin/ DESTINATION prepared/qube_game/)
file(COPY ../README.md ../LICENSE ../CONTRIBUTORS DESTINATION prepared/qube_game/)
if(WIN32)
    set(ARCHIVE_FILE_TYPE zip)
else()
    set(ARCHIVE_FILE_TYPE zip)
    set(ARCHIVE_ARGS FORMAT zip)
endif()

#file(ARCHIVE_CREATE OUTPUT packaged/${EXE_NAME}.${ARCHIVE_FILE_TYPE}
#        PATHS prepared/qube_game/
#        ${ARCHIVE_ARGS})
execute_process(COMMAND tar -acf ../packaged/${EXE_NAME}.${ARCHIVE_FILE_TYPE} qube_game
        WORKING_DIRECTORY prepared)