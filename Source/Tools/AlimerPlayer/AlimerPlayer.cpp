/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "AlimerPlayer.h"
#include "Engine/Main.h"
#include "Core/TimeSpan.h"

AlimerPlayer::AlimerPlayer()
	: Application()
{
#if defined(_MSC_VER) && defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

AlimerPlayer::~AlimerPlayer()
{
}

void AlimerPlayer::Setup()
{
	Application::Setup();

#if defined(ALIMER_WINDOWS) && !defined(ALIMER_WINMODERN)
	/*if (GetArguments().size() < 1 || _args.HasArg('h', "help"))
	{
		Help();
		//ErrorExit();
		return;
	}*/

#endif
}

int main(int argc, const char** argv)
{
	std::unique_ptr<AlimerPlayer> application(new AlimerPlayer());
	application->GetArgs()->Parse(argc, argv);
	return application->Run();
}