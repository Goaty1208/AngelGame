mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug
cd ..
cmake -S . -B build
cmake --build build
cp -a ./src/build/. ./build/bin/debug/
cd ./build/bin/debug/
gdb angelic
cd ..
cd ..