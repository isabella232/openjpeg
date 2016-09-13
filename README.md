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

The output is in the dist folder:

#### openJPEG-DynamicMemory-browser.js

Slower but should handle any size image.  Uses a global object so can work in a web browser

#### openJPEG-DynamicMemory-commonJS.js

Slower but should handle any size image.  CommonJS so loads into Node.js

#### openJPEG-FixedMemory-browser.js

Faster but will crash if you run out of memory.  Uses a global object so can work in a web browser

#### openJPEG-FixedMemory-commonJS.js

Faster but will crash if you run out of memory.  CommonJS so loads into Node.js


