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
* node
* grunt

> grunt

The output is dist/openJPEG-DynamicMemory.js and dist/openJPEG-FixedMemory.js

#### openJPEG-DynamicMemory.js

Slower but should handle any size image

#### openJPEG-FixedMemory.js

Faster but will crash if you run out of memory


