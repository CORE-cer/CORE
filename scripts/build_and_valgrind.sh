# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"
cd ..

# Include common
source scripts/common.sh
# _setArgs "$@"

# Compile the project under "Debug"
./scripts/build.sh --build_type=Debug "$@"
build_result=$?

if [ $build_result -ne 0 ]; then
    echo -e "${RED}Build failed!${NORMAL_OUTPUT}"
    exit 1
fi

echo -e "${PURPLE}--------------${NORMAL_OUTPUT}"
echo -e "${PURPLE}Internal Tests with Valgrind${NORMAL_OUTPUT}"
echo -e "${PURPLE}--------------${NORMAL_OUTPUT}"
valgrind --leak-check=full --suppressions=./valgrind_suppressions.txt \
    --exit-on-first-error=yes --error-exitcode=1  ./build/Debug/tests
valgrind_test_result=$?

if [ $valgrind_test_result -eq 0 ]; then
    echo -e "${GREEN}Valgrind passed!${NORMAL_OUTPUT}"
else
    echo -e "${RED}Some tests failed!${NORMAL_OUTPUT}"
    exit 1
fi
