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
	//_commandList.Reset();
}

/*void HelloWorld::Setup()
{
	//_settings.deviceType = GpuDeviceType::OpenGL;
	//_settings.deviceType = GpuDeviceType::Vulkan;
	_settings.deviceType = GpuDeviceType::Direct3D11;
	//_settings.deviceType = GpuDeviceType::Direct3D12;

#ifdef _DEBUG
	_settings.deviceCreationType = GpuDeviceCreationType::Debug;
#endif
}

void HelloWorld::Initialize()
{
	Application::Initialize();

	SubscribeToEvent(_engine->GetWindow()->resizeEvent, &HelloWorld::HandleResize);

	// Create CommandBuffers for every swap chain image.
	auto gpuDevice = _engine->GetGpuDevice();
	_commandList.Reset(gpuDevice->CreateCommandList(GpuCommandListType::Direct));
}

void HelloWorld::HandleResize(WindowResizeEvent& event)
{

}

void HelloWorld::Draw(float elapsedTime, float totalElapsedTime)
{
	RHIColor4 clearColor = { 0.0f, 0.2f, 0.4f, 1.0f };
	RHIRect2D viewport = { 0, 0, _engine->GetWindow()->GetWidth(), _engine->GetWindow()->GetHeight() };
	GpuFramebuffer* backBuffer = _engine->GetSwapChain()->GetCurrentBackBuffer();

	_commandList->Reset();
	_commandList->SetViewport(viewport);
	_commandList->SetScissor(viewport);
	_commandList->BeginRenderPass(backBuffer, clearColor, 1.0f, 0);
	_commandList->EndRenderPass();
	_commandList->Close();
	_engine->GetGpuDevice()->GetDefaultCommandQueue()->Submit(_commandList.Get(), false);
}*/

ALIMER_MAIN(HelloWorld);