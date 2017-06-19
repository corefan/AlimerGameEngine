/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.txt', which is part of this source code package.
*/

#include "HelloWorld.h"
#include "Engine/Main.h"
#include "Core/TimeSpan.h"

HelloWorld::HelloWorld()
	: Application()
{
	TimeSpan span(0, 0, 30);
}

HelloWorld::~HelloWorld()
{
}

ALIMER_MAIN(HelloWorld);