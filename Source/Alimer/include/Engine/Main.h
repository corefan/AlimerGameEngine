/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include "Utils/CommandLine.h"

#if ALIMER_WINDOWS_FAMILY
#	ifndef STRICT
#		define STRICT 1
#	endif

#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN 1
#	endif

#	ifndef NOMINMAX
#		define NOMINMAX 1
#	endif

#	include <Windows.h>
#elif ALIMER_ANDROID
#	include <android_native_app_glue.h>
#endif

#include "Engine/Application.h"

#if ALIMER_WINDOWS_FAMILY && !ALIMER_WINMODERN
#define ALIMER_MAIN(appClass) \
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {\
		UNREFERENCED_PARAMETER(hInstance); \
		UNREFERENCED_PARAMETER(hPrevInstance); \
		UNREFERENCED_PARAMETER(lpCmdLine); \
		UNREFERENCED_PARAMETER(nCmdShow); \
		std::unique_ptr<appClass> application(new appClass()); \
		application->GetArgs()->Parse(::GetCommandLineW()); \
		return application->Run(); \
}
#elif ALIMER_WINMODERN
#define ALIMER_MAIN(appClass) \
[Platform::MTAThread] \
int main(Platform::Array<Platform::String^>^) { \
	std::unique_ptr<appClass> application(new appClass()); \
	return application->Run(); \
}
#elif ALIMER_ANDROID
#define ALIMER_MAIN(appClass) \
android_app* AlimerAndroidAppState = nullptr; \
void android_main(struct android_app* app_) { \
	app_dummy(); \
	AlimerAndroidAppState = app_; \
	std::unique_ptr<appClass> application(new appClass()); \
	application->Run(); \
}
#else
#define ALIMER_MAIN(appClass) \
int main(int argc, const char** argv) { \
	std::unique_ptr<appClass> application(new appClass()); \
	application->GetArgs()->Parse(argc, argv); \
	return application->Run(); \
}
#endif