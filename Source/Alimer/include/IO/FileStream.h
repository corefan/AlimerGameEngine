/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.txt', which is part of this source code package.
*/

#pragma once

#include "IO/Stream.h"

namespace Alimer
{
	/// File access mode.
	enum class FileAccess : uint32_t
	{
		Read = 1,
		Write = 2, 
		ReadWrite = 3
	};

	/**
	* File System Stream.
	*/
	class ALIMER_API FileStream final : public Stream
	{
	public:
		/**
		* Constructor.
		*/
		FileStream(const String& fileName, FileAccess access = FileAccess::Read);

		/**
		* Destructor.
		* This closes the stream if it supports closing.
		*/
		virtual ~FileStream();

		void Close() override;
		void Flush() override;

		bool CanRead() const override;
		bool CanWrite() const override;
		bool CanSeek() const override;

		uint64_t GetPosition() const override;
		void SetPosition(uint64_t value) override;

		uint64_t Read(void* buffer, uint64_t size) const override;
		uint64_t Write(const void* buffer, uint64_t size) override;
		bool Seek(uint64_t offset, SeekOrigin origin) override;

		bool IsEof() const override;

	private:
		FileAccess _access;

		/// File handle.
		void* _handle;
	};
}