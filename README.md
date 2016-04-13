## OPENJPEG fork with JavaScript binding for emscripten

### Changed from uclouvain/openjpeg

* Added JS bindings in wrapping/JS/.
* Merge Buffer-based streams from UltraLinq/openjpeg.
* Relaxed validation to enable decoding of truncated streams (may cause leaks)


### Usage

``` html
<head>
    <script src="libopenjpeg.js" type="text/javascript"></script>
</head>
...
<script>
Module['_main'] = function() {
   xhr = new XMLHttpRequest();
   xhr.open("GET", "img,jp2", true);
   xhr.responseType = 'arraybuffer';
   xhr.onload = function (oEvent) {
      data = new Uint8Array(xhr.response);
      
      image =  Module.opj_decode(data);
      
      var length        = image.length;
      var size_x        = image.sx;
      var size_y        = image.sy;
      var nbChannels    = image.nbChannels;
      var timeToDecode  = image.perf_timetodecode;
      var pixelData     = image.pixelData;
   }
}
</script>
```

pixelData is a Int32Array and the format is 
 * GGG... for grascale images
 * RGBRGBRGB... for color images

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
     -s TOTAL_MEMORY=50000000 \
     -s NO_FILESYSTEM=1 \
     -O3
```

The output is bin/libopenjpeg.js and bin/libopenjpeg.js.mem.
