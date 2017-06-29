/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "AlimerPlayer.h"
#include "Engine/Main.h"
#include "Core/TimeSpan.h"

void Help(const char* errorMessage = NULL)
{
	if (errorMessage != NULL)
	{
		fprintf(stderr, "Error:\n%s\n\n", errorMessage);
	}

	fprintf(stderr,
		"Alimer Game Engine runtime player\n"
		"Copyright 2016-2017 Amer Koleci. All rights reserved.\n"
		"License: https://github.com/AlimerGameEngine/AlimerGameEngine/blob/master/LICENSE.md\n\n"
	);

	fprintf(stderr,
		"Usage: AlimerPlayer <path> [options]\n"
		"\n"
		"<path>						Directory <path> to run from.\n"
		"Options:\n"
		"  -h --help                Display this help.\n"
		"  -v --version             Display engine version.\n"
		"  --gfx <type>             Target platform.\n"
		"           default\n"
		"           d3d11\n"
		"           d3d12\n"
		"           opengl|gl\n"
		"           vulkan\n"
	);
}

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

bool AlimerPlayer::Setup()
{
	Application::Setup();

	if (_commandLine->GetArgs().size() < 1 || _commandLine->HasArg('h', "help"))
	{
		Help();
		_exitCode = EXIT_FAILURE;
		return false;
	}

	if (_commandLine->HasArg('v', "version"))
	{
		fprintf(stderr
			, "AlimerPlayer, version %d.%d.%d.\n"
			, ALIMER_VERSION_MAJOR
			, ALIMER_VERSION_MINOR
			, ALIMER_VERSION_PATCH
		);

		return false;
	}

	return true;
}

void AlimerPlayer::Initialize()
{

}

int main(int argc, const char** argv)
{
	std::unique_ptr<AlimerPlayer> application(new AlimerPlayer());
	application->GetArgs()->Parse(argc, argv);
	return application->Run();
}