#!

echo "Generating Project Files..."
premake/premake5 gmake2 --test-coverage

echo "Building Release..."
make clean
config=releaseclang_x64 make

echo "Building Debug..."
make clean
config=debugclang_x64 make

echo "Running Tests, Generating Coverage..."
LLVM_PROFILE_FILE="./backend/builds/bin/coverage.profraw" backend/builds/bin/cocktail-party --run-tests

echo "Preparing..."
llvm-profdata merge -sparse ./backend/builds/bin/coverage.profraw -o ./backend/builds/bin/coverage_merged.profdata
llvm-cov report ./backend/builds/bin/cocktail-party -instr-profile=./backend/builds/bin/coverage_merged.profdata -show-region-summary=false -show-branch-summary=false backend/src/cocktail_party.cpp backend/src/cocktail_party.h
#TOTAL.*?(\d+\.\d+%)$
