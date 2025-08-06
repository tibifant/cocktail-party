#!

echo "Generating Project Files..."
premake/premake5 gmake2
if [ $? -ne 0 ]; then exit 1; fi

echo "Building Release..."
make clean
if [ $? -ne 0 ]; then exit 1; fi
config=releaseclang_x64 make
if [ $? -ne 0 ]; then exit 1; fi
