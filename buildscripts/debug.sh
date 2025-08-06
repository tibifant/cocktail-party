#!

echo "Generating Project Files..."
premake/premake5 gmake2 --test-coverage
if [ $? -ne 0 ]; then exit 1; fi

echo "Building Debug..."
make clean
if [ $? -ne 0 ]; then exit 1; fi
config=debugclang_x64 make
if [ $? -ne 0 ]; then exit 1; fi
