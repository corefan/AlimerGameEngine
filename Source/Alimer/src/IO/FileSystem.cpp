/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "IO/FileSystem.h"
#include "Utils/StringUtils.h"

#if ALIMER_MICROSOFT_FAMILY
#	include <windows.h>
#else
#endif

namespace Alimer
{
	String GetInternalPath(const String& pathName)
	{
		return StringUtils::Replace(pathName, "\\", "/");
	}

	String GetNativePath(const String& pathName)
	{
#ifdef _WIN32
		return StringUtils::Replace(pathName, "/", "\\");
#else
		return pathName;
#endif
	}

	WString GetWideNativePath(const String& pathName)
	{
#ifdef _WIN32
		return StringUtils::ToWString(StringUtils::Replace(pathName, "/", "\\"));
#else
		return StringUtils::ToWString(pathName);
#endif
	}


	String AddTrailingSlash(const String& pathName)
	{
		String ret = StringUtils::Trim(pathName);
		ret = StringUtils::Replace(ret, "\\", "/");
		if (!ret.empty() && ret.back() != '/')
			ret += '/';
		return ret;
	}

	void SplitPath(const String& fullPath, String& pathName, String& fileName, String& extension, bool lowercaseExtension = true)
	{
		String fullPathCopy = GetInternalPath(fullPath);

		String::size_type extPos = fullPathCopy.find_last_of('.');
		String::size_type pathPos = fullPathCopy.find_last_of('/');

		if (extPos != String::npos && (pathPos == String::npos || extPos > pathPos))
		{
			extension = fullPathCopy.substr(extPos);
			if (lowercaseExtension)
				extension = StringUtils::ToLower(extension);
			fullPathCopy = fullPathCopy.substr(0, extPos);
		}
		else
			extension.clear();

		pathPos = fullPathCopy.find_last_of('/');
		if (pathPos != String::npos)
		{
			fileName = fullPathCopy.substr(pathPos + 1);
			pathName = fullPathCopy.substr(0, pathPos + 1);
		}
		else
		{
			fileName = fullPathCopy;
			pathName.clear();
		}
	}

	String GetPath(const String& fullPath)
	{
		String path, file, extension;
		SplitPath(fullPath, path, file, extension);
		return path;
	}

	String FileSystem::GetProgramDir()
	{
#if ALIMER_ANDROID
		// This is an internal directory specifier pointing to the assets in the .apk
		// Files from this directory will be opened using special handling
		return APK;
#elif defined(IOS) || defined(TVOS)
		return AddTrailingSlash(SDL_IOS_GetResourceDir());
#elif ALIMER_MICROSOFT_FAMILY
		wchar_t exeName[MAX_PATH];
		exeName[0] = 0;
		GetModuleFileNameW(0, exeName, MAX_PATH);
		return GetPath(StringUtils::ToString(exeName));
#elif ALIMER_APPLE
		char exeName[MAX_PATH];
		memset(exeName, 0, MAX_PATH);
		unsigned size = MAX_PATH;
		_NSGetExecutablePath(exeName, &size);
		return GetPath(String(exeName));
#elif ALIMER_LINUX
		char exeName[MAX_PATH];
		memset(exeName, 0, MAX_PATH);
		pid_t pid = getpid();
		String link = "/proc/" + String(pid) + "/exe";
		readlink(link.CString(), exeName, MAX_PATH);
		return GetPath(String(exeName));
#else
		return GetCurrentDir();
#endif
	}

	String FileSystem::GetCurrentDir()
	{
#if ALIMER_MICROSOFT_FAMILY
		wchar_t path[MAX_PATH];
		path[0] = 0;
		GetCurrentDirectoryW(MAX_PATH, path);
		return AddTrailingSlash(StringUtils::ToString(path));
#else
		char path[MAX_PATH];
		path[0] = 0;
		getcwd(path, MAX_PATH);
		return AddTrailingSlash(String(path));
#endif
	}

	bool FileSystem::SetCurrentDir(const String& pathName)
	{
		/*if (!CheckAccess(pathName))
		{
			ALIMER_LOGERRORF("Access denied to %s", pathName.c_str());
			return false;
		}*/

#if ALIMER_MICROSOFT_FAMILY
		if (SetCurrentDirectoryW(GetWideNativePath(pathName).c_str()) == FALSE)
		{
			ALIMER_LOGERRORF("Failed to change directory to %s", pathName.c_str());
			return false;
		}
#else
		if (chdir(GetNativePath(pathName).c_str()) != 0)
		{
			ALIMER_LOGERRORF("Failed to change directory to %s", pathName.c_str());
			return false;
		}
#endif

		return true;
	}
}