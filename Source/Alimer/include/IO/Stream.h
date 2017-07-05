/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE', which is part of this source code package.
*/

#pragma once

#include "Core/RefCounted.h"

namespace Alimer
{
	/// File access mode.
	enum class SeekOrigin : uint8_t
	{
		Begin = 0,
		Current = 1,
		End = 2
	};

	/**
	* Base IO Stream class.
	*/
	class ALIMER_API Stream : public RefCounted
	{
	public:
		/**
		* Destructor.
		* This closes the stream if it supports closing.
		*/
		virtual ~Stream();

		/**
		* Returns true if this stream can perform read operations.
		*
		* @return True if the stream can read, false otherwise.
		*/
		virtual bool CanRead() const = 0;

		/**
		* Returns true if this stream can perform write operations.
		*
		* @return True if the stream can write, false otherwise.
		*/
		virtual bool CanWrite() const = 0;

		/**
		* Returns true if this stream can seek.
		* CanSeek
		* @return True if the stream can seek, false otherwise.
		*/
		virtual bool CanSeek() const = 0;

		/**
		* Gets the current position of the stream.
		*/
		virtual uint64_t GetPosition() const = 0;

		/**
		* Sets the current position of the stream from begin.
		*/
		virtual void SetPosition(uint64_t value);

		/**
		* Gets the size/length of the stream.
		*/
		uint64_t Length() const { return _length; }

		/**
		* Read data from stream.
		* @param buffer Destination buffer.
		* @param size Number of bytes to read.
		* @return Number of bytes actually read.
		*/
		virtual uint64_t Read(void* buffer, uint64_t size) const = 0;

		/**
		* Write data to a stream.
		* @param buffer Source buffer.
		* @param size Number of bytes to write.
		* @return Number of bytes actually written.
		*/
		virtual uint64_t Write(const void* buffer, uint64_t size) = 0;

		/**
		* Seek with given offset and given origin.
		*/
		virtual bool Seek(uint64_t offset, SeekOrigin origin) = 0;

		virtual uint8_t ReadByte() const;
		virtual int8_t ReadSByte() const;
		virtual char ReadChar() const;
		virtual bool ReadBoolean() const;

		virtual uint16_t ReadUInt16() const;
		virtual uint32_t ReadUInt32() const;
		virtual uint64_t ReadUInt64() const;

		virtual int16_t ReadInt16() const;
		virtual int32_t ReadInt32() const;
		virtual int64_t ReadInt64() const;

		virtual float ReadSingle() const;
		virtual double ReadDouble() const;

		// Read string with optional binary mode, see WriteString.
		String ReadString(bool binaryMode = false);
		uint32_t Read7BitEncodedInt() const;

		virtual void WriteByte(const uint8_t value);
		virtual void Write(const bool value);
		virtual void WriteChar(const char value);

		virtual void Write(const int8_t value);
		virtual void Write(const uint16_t value);
		virtual void Write(const int16_t value);
		virtual void Write(const uint32_t value);
		virtual void Write(const int32_t value);
		virtual void Write(const uint64_t value);
		virtual void Write(const int64_t value);
		virtual void Write(const float value);
		virtual void Write(const double value);

		/// Write a string with optional binary mode in which first is writer 7bit encoded length then string like C#.
		void WriteString(const String& value, bool binaryMode = false);

		void Write7BitEncodedInt(uint32_t value);

		/**
		* Reads a text line from the stream.
		*/
		virtual String ReadLine();

		/**
		* Read a 4-character file ID.
		*/
		String ReadFileID();

		/**
		* Write a four-letter file ID. If the string is not long enough, spaces will be appended.
		*/
		void WriteFileID(const String& value);

		virtual void Close() { }
		virtual void Flush() { }
		virtual bool IsEof() const;

		/// Return the stream name.
		const String& GetName() const { return _name; }

		String ReadAllText();
		uint8_t* ReadAllBytes(uint64_t* fileSize = nullptr);

	protected:
		Stream();

		/// Stream name.
		String _name;
		uint64_t _length;

	private:
		void FillBuffer(uint32_t count) const;

		mutable std::vector<uint8_t> _buffer;

	private:
		DISALLOW_COPY_AND_ASSIGN(Stream);
	};
}