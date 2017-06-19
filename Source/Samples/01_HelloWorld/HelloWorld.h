/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.txt', which is part of this source code package.
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
	/*void Setup() override;
	void Initialize() override;
	void Draw(float elapsedTime, float totalElapsedTime) override;

	void HandleResize(WindowResizeEvent& event);

	UniquePtr<GpuCommandList> _commandList;*/
};
