add_library(OSDependent STATIC ossource.cpp ../osinclude.h)
set_target_properties (OSDependent PROPERTIES FOLDER glslang)