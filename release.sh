cd build
rm -rf ./build/release/
cmake -DCMAKE_BUILD_TYPE=Release ..
cd ..
cmake -S . -B build
cmake --build build
cp -a ./src/build/. ./build/bin/release/
cd ./build/bin/release/
./angelic
cd ..
cd ..