message(STATUS "post")
message(STATUS ${CMAKE_CURRENT_BINARY_DIR})
execute_process(
        COMMAND pwd
        OUTPUT_VARIABLE PWD
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS ${PWD})
execute_process(
        COMMAND ls -al
        OUTPUT_VARIABLE LS
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS ${LS})

# architecture
EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )

# git hash
execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(REPLACE "." "_" FileVersion ${CMAKE_PROJECT_VERSION})

set(EXE_NAME "qube_puzzle_${CMAKE_SYSTEM_NAME}_${ARCHITECTURE}_${GIT_HASH}")

file(ARCHIVE_CREATE OUTPUT ${EXE_NAME}.zip PATHS ${CMAKE_CURRENT_BINARY_DIR}/bin/)