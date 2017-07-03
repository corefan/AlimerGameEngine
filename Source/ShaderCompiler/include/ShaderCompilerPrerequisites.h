/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"

#if ALIMER_SHADER_COMPILER_EXPORTS
#       define SHADER_COMPILER_API ALIMER_INTERFACE_EXPORT
#else
#       define SHADER_COMPILER_API ALIMER_INTERFACE_IMPORT
#endif

