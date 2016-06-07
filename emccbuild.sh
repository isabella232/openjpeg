#!/bin/bash
mkdir build
emcc bin/libopenjpeg-js.bc -o \
     build/openJPEG-FixedMemory.js \
     --memory-init-file 0 \
     -s EXPORTED_FUNCTIONS="['_jp2_decode']" \
     -s TOTAL_MEMORY=400000000 \
     -s NO_FILESYSTEM=1 \
     -s EXPORT_NAME="'OpenJPEG'" \
     -s MODULARIZE=1 \
     -O3
emcc bin/libopenjpeg-js.bc -o \
     build/openJPEG-DynamicMemory.js \
     --memory-init-file 0 \
     -s EXPORTED_FUNCTIONS="['_jp2_decode']" \
     -s ALLOW_MEMORY_GROWTH=1 \
     -s NO_FILESYSTEM=1 \
     -s EXPORT_NAME="'OpenJPEG'" \
     -s MODULARIZE=1 \
     -O3
