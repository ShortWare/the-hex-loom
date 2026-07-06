Install emsdk
```
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

CMake options
```
-DPLATFORM=Web -DCMAKE_TOOLCHAIN_FILE=/home/Pd/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
```
