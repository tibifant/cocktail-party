#!

echo "Running Tests, Generating Coverage..."
LLVM_PROFILE_FILE="./backend/builds/bin/coverage.profraw" backend/builds/bin/cocktail-party --run-tests
if [ $? -ne 0 ]; then exit 1; fi

echo "Preparing..."
llvm-profdata merge -sparse ./backend/builds/bin/coverage.profraw -o ./backend/builds/bin/coverage_merged.profdata
if [ $? -ne 0 ]; then exit 1; fi
llvm-cov report ./backend/builds/bin/cocktail-party -instr-profile=./backend/builds/bin/coverage_merged.profdata -show-region-summary=false -show-branch-summary=false backend/src/cocktail_party.cpp backend/src/cocktail_party.h
if [ $? -ne 0 ]; then exit 1; fi
