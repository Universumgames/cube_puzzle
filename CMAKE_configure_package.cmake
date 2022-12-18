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

#configure config header
configure_file (
        "${REAL_SRC_DIR}/src/config.hpp.in"
        "${CMAKE_CURRENT_BINARY_DIR}/generated/config.hpp" @ONLY)

string(REPLACE "." "_" FileVersion ${CMAKE_PROJECT_VERSION})

set(EXE_NAME "qube_puzzle_${CMAKE_SYSTEM_NAME}_${ARCHITECTURE}_${GIT_HASH}")

# compile info
file(WRITE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/appInfo
"Version: ${CMAKE_PROJECT_VERSION}
Commit: ${GIT_HASH}
Architecture: ${ARCHITECTURE}
System: ${CMAKE_SYSTEM_NAME}
")
#Executable Base Name: ${EXE_NAME}