<div align="center">

# [The Hex Loom]

**[Releases](https://github.com/ShortWare/raylib-gamejam/releases)**

[![License: Beerware](https://img.shields.io/badge/license-Beerware-yellow.svg)](https://github.com/Pdrewicz/RimForge)

<br><br><br>

</div>



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

Server
```
python -m http.server 8000
```
