/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "IO/FileStream.h"
#if defined(ALIMER_WINDOWS)
#include <windows.h>
#else
static const char* openMode[] =
{
	"rb",
	"wb",
	"r+b",
	"w+b"
};
#endif

namespace Alimer
{
	FileStream::FileStream(const String& fileName, FileAccess access)
		: _access(access)
	{
#if defined(ALIMER_WINDOWS)
		DWORD nativeAccess = 0;
		if (access == FileAccess::Read)
		{
			nativeAccess = GENERIC_READ;
		}

		if (access == FileAccess::Write)
		{
			nativeAccess = GENERIC_WRITE;
		}
		if (access == FileAccess::ReadWrite)
		{
			nativeAccess = GENERIC_READ | GENERIC_WRITE;
		}

		std::wstring wideString(fileName.begin(), fileName.end());
		_handle = CreateFileW(
			wideString.c_str(),
			nativeAccess,
			FILE_SHARE_READ,
			nullptr,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		if (_handle == INVALID_HANDLE_VALUE)
		{
			ALIMER_LOGERROR("Win32FileStream: Error during CreateFile call");
		}

		LARGE_INTEGER size;
		if (!GetFileSizeEx(_handle, &size))
		{
			ALIMER_LOGERROR("GetFileSizeEx: failed");
			return;
		}

		_length = (size_t)size.QuadPart;
#else
		_handle = fopen(GetNativePath(fileName).c_str(), openMode[(size_t)access - 1]);
#endif
	}

	FileStream::~FileStream()
	{
		Close();
	}

	bool FileStream::CanRead() const
	{
		return _access == FileAccess::Read || _access == FileAccess::ReadWrite;
	}

	bool FileStream::CanWrite() const
	{
		return _access == FileAccess::Write || _access == FileAccess::ReadWrite;
	}

	bool FileStream::CanSeek() const
	{
#if defined(ALIMER_WINDOWS)
		return _handle != INVALID_HANDLE_VALUE;
#else
		return _handle != nullptr;
#endif
	}

	void FileStream::Close()
	{
#if defined(ALIMER_WINDOWS)
		if (_handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(_handle);
			_handle = INVALID_HANDLE_VALUE;
		}
#else
		if (_handle != nullptr)
		{
			fclose((FILE*)_handle);
			_handle = nullptr;
		}
#endif
	}

	void FileStream::Flush()
	{
#if defined(ALIMER_WINDOWS)
		FlushFileBuffers(_handle);
#else
		fflush((FILE*)_handle);
#endif
	}

	uint64_t FileStream::GetPosition() const
	{
#if defined(ALIMER_WINDOWS)
		LARGE_INTEGER windowsoffset;
		windowsoffset.QuadPart = 0;
		if (!SetFilePointerEx(_handle, windowsoffset, &windowsoffset, FILE_CURRENT))
		{
			return false;
		}

		return (uint64_t)windowsoffset.QuadPart;
#else
		off_t pos = ftello((FILE*)_handle);
		return (int64_t)(pos < 0 ? 0 : pos);
#endif
	}

	void FileStream::SetPosition(uint64_t value)
	{
		if (value < 0)
		{
			ALIMER_LOGERROR("Position need to be greather or equat to zero");
			return;
		}

		Seek(value, SeekOrigin::Begin);
	}

	uint64_t FileStream::Read(void* buffer, uint64_t size) const
	{
		if (!CanRead())
		{
			ALIMER_LOGERROR("Cannot read for write only stream");
			return (size_t)-1;
		}

#if defined(ALIMER_WINDOWS)
		DWORD byteRead;
		if (!ReadFile(_handle, buffer, (DWORD)size, &byteRead, nullptr))
		{
			return 0;
		}

		return (size_t)byteRead;
#else
		return fread((FILE*)_handle, size, 1, (FILE*)_handle);
#endif
	}

	uint64_t FileStream::Write(const void* buffer, uint64_t size)
	{
		if (!size)
			return 0;

		if (!CanWrite())
		{
			ALIMER_LOGERROR("Cannot write for read only stream");
			return 0;
		}

#if defined(ALIMER_WINDOWS)
		DWORD bytesWritten;
		if (!WriteFile(_handle, buffer, (DWORD)size, &bytesWritten, nullptr))
		{
			return 0;
		}

		return (size_t)bytesWritten;
#else
		return fwrite(buffer, 1, size, (FILE*)_handle);
#endif
	}

	bool FileStream::Seek(uint64_t offset, SeekOrigin origin)
	{
		if (!CanSeek())
		{
			return false;
		}

#if defined(ALIMER_WINDOWS)
		DWORD windowswhence;
		switch (origin)
		{
		case SeekOrigin::Begin:
			windowswhence = FILE_BEGIN;
			break;
		case SeekOrigin::Current:
			windowswhence = FILE_CURRENT;
			break;
		case SeekOrigin::End:
			windowswhence = FILE_END;
			break;
		default:
			ALIMER_LOGERROR("Win32FileStream::Seek: Unknown value for 'origin'");
			return false;
		}

		LARGE_INTEGER windowsoffset;
		windowsoffset.QuadPart = offset;
		if (!SetFilePointerEx(_handle, windowsoffset, &windowsoffset, windowswhence)) {
			return false;
		}

		return windowsoffset.QuadPart != 0;
#else
		fseek((FILE*)_handle, offset, SEEK_SET);
#endif
	}

	bool FileStream::IsEof() const
	{
#if defined(ALIMER_WINDOWS)
		if (_handle == INVALID_HANDLE_VALUE)
			return true;

		return (size_t)GetPosition() >= Length();
#else
		return feof((FILE*)_handle);
#endif
	}
}