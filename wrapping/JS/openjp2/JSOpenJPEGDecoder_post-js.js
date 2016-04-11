Module["opj_decode"] = function(data) {
        var dataPtr = Module._malloc(data.length);
        writeArrayToMemory(data, dataPtr);

        // create param outpout
        var imagePtrPtr=Module._malloc(4);
        var imageSizePtr=Module._malloc(4);
        var imageSizeXPtr=Module._malloc(4);
        var imageSizeYPtr=Module._malloc(4);
        var imageSizeCompPtr=Module._malloc(4);

        var t0 = Date.now();
        var ret = Module.ccall('jp2_decode','number', ['number', 'number', 'number', 'number', 'number', 'number', 'number'],
                                                      [dataPtr, data.length, imagePtrPtr, imageSizePtr, imageSizeXPtr, imageSizeYPtr, imageSizeCompPtr]);
        // add num vomp..etc
        if(ret !== 0){
            console.log('[opj_decode] decoding failed!')
            Module._free(dataPtr);
            Module._free(getValue(imagePtrPtr, '*'));
            Module._free(imageSizeXPtr);
            Module._free(imageSizeYPtr);
            Module._free(imageSizePtr);
            Module._free(imageSizeCompPtr);
            return undefined;
        }

        var imagePtr = getValue(imagePtrPtr, '*')

        var image = {
            length : getValue(imageSizePtr,'i32'),
            sx :  getValue(imageSizeXPtr,'i32'),
            sy :  getValue(imageSizeYPtr,'i32'),
            nbChannels : getValue(imageSizeCompPtr,'i32'), // hard coded for now
            perf_timetodecode : undefined,
            pixelData : undefined
        };

        image.pixelData = new Int32Array(image.sx*image.sy*image.nbChannels);
        for (i = 0; i < image.pixelData.length; i++) {
            image.pixelData[i] = getValue(imagePtr + i*4, 'i32'); // because sizeof(i32) = 4
        }
        var t1 = Date.now();
        image.perf_timetodecode = t1-t0;

        // free
        Module._free(dataPtr)
        Module._free(imagePtrPtr);
        Module._free(imagePtr);
        Module._free(imageSizePtr);
        Module._free(imageSizeXPtr);
        Module._free(imageSizeYPtr);
        Module._free(imageSizeCompPtr);

        return image;
    }

