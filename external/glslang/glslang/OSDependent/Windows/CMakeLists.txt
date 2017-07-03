set(SOURCES ossource.cpp ../osinclude.h)

add_library(OSDependent STATIC ${SOURCES})
set_target_properties (OSDependent PROPERTIES FOLDER glslang)

# MinGW GCC complains about function pointer casts to void*.
# Turn that off with -fpermissive.
if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    target_compile_options(OSDependent PRIVATE -fpermissive)
endif()

if(WIN32)
    source_group("Source" FILES ${SOURCES})
endif(WIN32)
