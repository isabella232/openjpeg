
# OPENJPEG fork with JavaScript binding for emscripten

## What is OpenJPEG ?

Quick how to. You need emscripten, build tools and cmake.

```bash
git clone https://github.com/jpambrun/openjpeg
mkdir openjpeg-build
cd openjpeg-build
cmake -DCMAKE_TOOLCHAIN_FILE=/usr/lib/emscripten/cmake/Modules/Platform/Emscripten.cmake \
   -DCMAKE_BUILD_TYPE=Release \
   -DBUILD_CODEC=OFF \
   -DBUILD_JS=ON \
   -DBUILD_SHARED_LIBS=OFF \
   -G"Unix Makefiles" \
   ../openjpeg
cmake .
make
emcc -s TOTAL_MEMORY=20000000 -03 bin/libopenjpeg-js.bc -o bin/libopenjpeg.js
```

The output is bin/libopenjpeg.js and bin/libopenjpeg.js.mem.