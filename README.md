## OPENJPEG fork with JavaScript binding for emscripten

### Changed from uclouvain/openjpeg

* Added JS bindings in wrapping/JS/.
* Merge Buffer-based streams from UltraLinq/openjpeg.
* Relaxed validation to enable decoding of truncated streams (may cause leaks)


### Usage

See https://github.com/chafey/cornerstoneWADOImageLoader/blob/master/src/decoders/decodeJPEG2000.js

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
     --memory-init-file 0 \
     -s EXPORTED_FUNCTIONS="['_jp2_decode']" \
     -s ALLOW_MEMORY_GROWTH=1 \
     -s NO_FILESYSTEM=1 \
     -s EXPORT_NAME="'OpenJPEG'" \
     -s MODULARIZE=1 \
     -O3
```

The output is bin/libopenjpeg.js
