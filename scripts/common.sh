# Global array for excluded queries
declare -a EXCLUDED_QUERIES

function _setArgs() {
  while [ "${1:-}" != "" ]; do
    case "$1" in
      "-p" | "--profile")
        shift
        CONAN_PROFILE=$1
        ;;
      "-b" | "--buildType")
        shift
        BUILD_TYPE=$1
        ;;
      "-s" | "--sanitizer")
        shift
        SANITIZER=$1
        ;;
      "--exclude")
        shift
        EXCLUDED_QUERIES+=("$1") # Add the excluded query to the array
        ;;
    esac
    shift
  done
}

function build() {
  if [ ! -z "$SANITIZER" ]; then
      conan build . --profile:host ${CONAN_PROFILE} --profile:build ${CONAN_PROFILE}\
          -s build_type=${BUILD_TYPE}\
          --build missing -o sanitizer=${SANITIZER}
  else
      conan build . --profile:host ${CONAN_PROFILE} --profile:build ${CONAN_PROFILE}\
          -s build_type=${BUILD_TYPE}\
          --build missing
  fi 

  build_result=$?
  if [ $build_result -ne 0 ]; then
      echo -e "${RED}Build failed!${NORMAL_OUTPUT}"
      exit 1
  fi
}

# Default values
BUILD_TYPE="Debug"
CONAN_PROFILE="conan_profiles/x86_64-linux-gcc"

RED='\033[0;31m'
GREEN='\033[0;32m'
PURPLE='\033[0;35m'
NORMAL_OUTPUT='\033[0m'
