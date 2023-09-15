_setArgs(){
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
    esac
    shift
  done
}

BUILD_TYPE="Debug"
CONAN_PROFILE="conan_profiles/x86_64-linux"

RED='\033[0;31m'
GREEN='\033[0;32m'
PURPLE='\033[0;35m'
NORMAL_OUTPUT='\033[0m'
