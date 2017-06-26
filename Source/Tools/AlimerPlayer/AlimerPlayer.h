/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Engine/Application.h"

using namespace Alimer;

class AlimerPlayer : public Application
{
public:
	AlimerPlayer();
	virtual ~AlimerPlayer();

protected:
	void Setup() override;

private:
};
