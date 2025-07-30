cd build
cmake -DCMAKE_BUILD_TYPE=Debug
cmake -S . -B build
cd ..
cmake --build build
cp -a ./src/build/. ./build/bin/debug/
cd ./build/bin/debug/
./angelic
cd ..
cd ..