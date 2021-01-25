# ==============================================================================
# Output Directories and CMake Variables
# ==============================================================================
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")
set(LIBRARY_OUTPUT_PATH    "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

# ==============================================================================
# Setup the repository
# ==============================================================================
# This will perfom task such as setting up the git hooks for anyone who uses
# this project
execute_process(
	COMMAND           ./setup.sh
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# ==============================================================================
# Calculate correct project version number
# ==============================================================================
# Read in the current version
file(READ version.txt PROJECT_VERSION)

# create array of version strings
string(REPLACE "." ";" VERSION_ARRAY ${PROJECT_VERSION})
# ------------------------------------------------------------------------------
# Major, Minor, Patch numbers
# ------------------------------------------------------------------------------
# set corresponding cmake version variables
list(POP_FRONT VERSION_ARRAY
	PROJECT_VERSION_MAJOR
	PROJECT_VERSION_MINOR
	PROJECT_VERSION_PATCH
)
# Remove the trailing newline from the patch number
string(REPLACE "\n" "" PROJECT_VERSION_PATCH ${PROJECT_VERSION_PATCH})
# ------------------------------------------------------------------------------
# Tweak number
# ------------------------------------------------------------------------------
# If the current commit has a special tag, put that as the tweak number. Else,
# use the commit hash as the tweak number
execute_process(
	COMMAND         git tag --points-at HEAD
	OUTPUT_VARIABLE PROJECT_VERSION_TWEAK
)
# The command returns a newline-seprated list of tags at the current commit. It
# ends in a new line
#
# Replace all the newlines but the last with a dash
string(REGEX REPLACE
	"\n([^$])"                 # match string
	"-\\1"                     # replace string
	PROJECT_VERSION_TWEAK      # output var
	"${PROJECT_VERSION_TWEAK}" # input string
)
if(PROJECT_VERSION_TWEAK STREQUAL "")
	# This command leavs a trailing newline
	execute_process(
		COMMAND         git rev-parse HEAD
		OUTPUT_VARIABLE PROJECT_VERSION_TWEAK
	)
endif()
# Remove the trailing newline from the tweak number
string(REPLACE "\n" "" PROJECT_VERSION_TWEAK ${PROJECT_VERSION_TWEAK})

# ------------------------------------------------------------------------------
# Full version number
# ------------------------------------------------------------------------------
# Rebuild the project version including the current commit hash
string(JOIN "." PROJECT_VERSION
	${PROJECT_VERSION_MAJOR}
	${PROJECT_VERSION_MINOR}
	${PROJECT_VERSION_PATCH}
	${PROJECT_VERSION_TWEAK}
)
unset(VERSION_ARRAY) # delete aray of version strings
message(STATUS "PROJECT_VERSION ${PROJECT_VERSION}")
message(STATUS "PROJECT_VERSION_MAJOR ${PROJECT_VERSION_MAJOR}")
message(STATUS "PROJECT_VERSION_MINOR ${PROJECT_VERSION_MINOR}")
message(STATUS "PROJECT_VERSION_PATCH ${PROJECT_VERSION_PATCH}")
message(STATUS "PROJECT_VERSION_TWEAK ${PROJECT_VERSION_TWEAK}")
