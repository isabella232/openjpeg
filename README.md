## OPENJPEG fork with JavaScript binding for emscripten

### Changed from uclouvain/openjpeg

* Added JS bindings in wrapping/JS/.
* Merge Buffer-based streams from UltraLinq/openjpeg.
* relaxed validation to enable decoding of truncated streams (may cause leaks)

### Building
Requires 
* emscripten
* cmake

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
mkdir dist
emcc bin/libopenjpeg-js.bc -o dist/libopenjpeg.js \
     --post-js bin/JSOpenJPEGDecoder_post-js.js \
     --memory-init-file 0 \
     -s TOTAL_MEMORY=500000000 \
     -s NO_FILESYSTEM=1 \
     -O3
```

The output is bin/libopenjpeg.js and bin/libopenjpeg.js.mem.
