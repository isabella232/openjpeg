/*
Copyright (c) 2016, Jean-Francois Pambrun
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "openjpeg.h"
#include <string.h>
#include <stdlib.h>
#define EMSCRIPTEN_API __attribute__((used))
#define J2K_MAGIC_NUMBER 0x51FF4FFF

//
//  Callbacks
//

static void error_callback(const char *msg, void *client_data) {
    (void)client_data;
    printf("[ERROR] %s", msg);
}
static void warning_callback(const char *msg, void *client_data) {
    (void)client_data;
    // printf("[WARNING] %s", msg);
}
static void info_callback(const char *msg, void *client_data) {
    (void)client_data;
    // printf("[INFO] %s", msg);
}

//
//  Stream I/O
//

static OPJ_SIZE_T read_from_buffer (void * p_buffer, OPJ_SIZE_T p_nb_bytes, void * data)
{
    memcpy(p_buffer, data, p_nb_bytes);
    return p_nb_bytes;
}

static OPJ_BOOL seek_from_buffer (OPJ_OFF_T p_nb_bytes, void * p_buffer_reader)
{
    //because of hardcoded check in OPJ..
    return OPJ_TRUE;
}

//
//  API
//

EMSCRIPTEN_API int jp2_decode(void* data, int data_size, void** p_image, int* p_image_size, int* size_x, int* size_y, int* size_comp){
    opj_dparameters_t parameters;
    opj_codec_t* l_codec = NULL;
    opj_image_t* image = NULL;
    opj_stream_t *l_stream = NULL;

    // detect stream type
    if( ((OPJ_INT32*)data)[0] == J2K_MAGIC_NUMBER ){
        l_codec = opj_create_decompress(OPJ_CODEC_J2K);
        // printf("OPJ_CODEC_J2K\n");
    }else{
        l_codec = opj_create_decompress(OPJ_CODEC_JP2);
        // printf("OPJ_CODEC_JP2\n");
    }

    opj_set_info_handler(l_codec, info_callback,00);
    opj_set_warning_handler(l_codec, warning_callback,00);
    opj_set_error_handler(l_codec, error_callback,00);

    opj_set_default_decoder_parameters(&parameters);

    l_stream = opj_stream_create(data_size, 1);
    opj_stream_set_user_data(l_stream, data, (opj_stream_free_user_data_fn) NULL);
    opj_stream_set_user_data_length(l_stream, data_size);
    opj_stream_set_read_function(l_stream, (opj_stream_read_fn) read_from_buffer);
    opj_stream_set_seek_function(l_stream, (opj_stream_seek_fn) seek_from_buffer);

    /* Setup the decoder decoding parameters using user parameters */
    if ( !opj_setup_decoder(l_codec, &parameters) ){
        printf("[ERROR] opj_decompress: failed to setup the decoder\n");
        opj_stream_destroy(l_stream);
        opj_destroy_codec(l_codec);
        return 1;
    }

    /* Read the main header of the codestream and if necessary the JP2 boxes*/
    if(! opj_read_header(l_stream, l_codec, &image)){
        printf("[ERROR] opj_decompress: failed to read the header\n");
        opj_stream_destroy(l_stream);
        opj_destroy_codec(l_codec);
        opj_image_destroy(image);
        return 1;
    }

    if (!opj_get_decoded_tile(l_codec, l_stream, image, parameters.tile_index)) {
        printf("[ERROR] opj_decompress: failed to decode tile!\n");
        opj_destroy_codec(l_codec);
        opj_stream_destroy(l_stream);
        opj_image_destroy(image);
        return 1;
    }

    // printf("tile %d is decoded!\n\n", parameters.tile_index);
    // printf("image X %d\n", image->x1);
    // printf("image Y %d\n", image->y1);
    // printf("image numcomps %d\n", image->numcomps);

    *size_x = image->x1;
    *size_y = image->y1;
    *size_comp = image->numcomps;

    *p_image_size = (*size_x) * (*size_y) * (*size_comp) * sizeof(OPJ_INT32);
    *p_image = malloc(*p_image_size);

    if(*size_comp == 1){
        memcpy(*p_image, image->comps[0].data, *p_image_size);
    }else if(*size_comp == 3){
        for(int i = 0; i < (*size_x) * (*size_y); i++){
            ((OPJ_INT32*)*p_image)[i*3+0] = image->comps[0].data[i];
            ((OPJ_INT32*)*p_image)[i*3+1] = image->comps[1].data[i];
            ((OPJ_INT32*)*p_image)[i*3+2] = image->comps[2].data[i];
        }
    }

    opj_stream_destroy(l_stream);
    opj_destroy_codec(l_codec);
    opj_image_destroy(image);
    return 0;
}

EMSCRIPTEN_API const char* jp2_version(){
    return opj_version();
}