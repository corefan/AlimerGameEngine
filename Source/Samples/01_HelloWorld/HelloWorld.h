/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Engine/Application.h"

using namespace Alimer;

class HelloWorld : public Application
{
public:
	HelloWorld();
	virtual ~HelloWorld();

private:
	bool Setup() override;
};
