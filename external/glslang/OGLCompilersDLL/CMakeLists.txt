set(SOURCES InitializeDll.cpp InitializeDll.h)

add_library(OGLCompiler STATIC ${SOURCES})
set_target_properties (OGLCompiler PROPERTIES FOLDER glslang)

if(WIN32)
    source_group("Source" FILES ${SOURCES})
endif(WIN32)
