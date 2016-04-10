#JSOpenJPEG/CMakeLists.txt

# First thing define the common source:
set(openjpeg-js_SRCS
  JSOpenJPEGDecoder.c
  )

# required header file:
include_directories(
  ${OPENJPEG_BINARY_DIR}/src/lib/openjp2 # opj_config.h
  ${OPENJPEG_SOURCE_DIR}/src/lib/openjp2
  ${OPENJPEG_SOURCE_DIR}/src/bin/common
  ${OPENJPEG_SOURCE_DIR}/src/bin/jp2
  )

add_library(openjpeg-js MODULE
  ${openjpeg-js_SRCS}
  )

set_target_properties(openjpeg-js PROPERTIES SUFFIX ".bc")

configure_file("JSOpenJPEGDecoder_post-js.js" ${EXECUTABLE_OUTPUT_PATH}/JSOpenJPEGDecoder_post-js.js COPYONLY)

target_link_libraries(openjpeg-js openjp2)