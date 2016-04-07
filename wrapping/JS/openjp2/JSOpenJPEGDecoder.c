#include "openjpeg.h"
#define EMSCRIPTEN_API    __attribute__((used))

EMSCRIPTEN_API int jp2_decode(int a){
    opj_dparameters_t parameters;
    opj_codec_t* l_codec = NULL;
    
    l_codec = opj_create_decompress(OPJ_CODEC_JP2);
    
    return 0;
}


EMSCRIPTEN_API const char* jp2_version(){
  
    return opj_version();
}