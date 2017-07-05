/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE', which is part of this source code package.
*/

#include "IO/Stream.h"

namespace Alimer
{
	Stream::Stream()
		: _name("")
		, _length(0)
	{
		_buffer.resize(16);
	}

	Stream::~Stream()
	{
		Close();
	}

	void Stream::SetPosition(uint64_t value)
	{
		if (value < 0)
		{
			ALIMER_LOGERROR("Position need to be greather or equat to zero");
			return;
		}

		Seek(value, SeekOrigin::Begin);
	}

	void Stream::FillBuffer(uint32_t count) const
	{
		if (count == 1)
		{
			_buffer[0] = ReadByte();
		}
		else
		{
			Read(_buffer.data(), count);
		}
	}

	uint8_t Stream::ReadByte() const
	{
		uint8_t* readBuffer = new uint8_t[1];
		if (Read(readBuffer, sizeof(uint8_t)) == 0)
		{
			return (uint8_t)-1;
		}

		return readBuffer[0];
	}

	int8_t Stream::ReadSByte() const
	{
		FillBuffer(1);
		return (int8_t)_buffer[0];
	}

	uint32_t Stream::Read7BitEncodedInt() const
	{
		uint32_t count = 0;
		uint32_t shift = 0;
		uint8_t b;

		do
		{
			// Check for a corrupted stream.  Read a max of 5 bytes.
			// In a future version, add a DataFormatException.
			if (shift == 5 * 7)  // 5 bytes max per Int32, shift += 7
			{
				ALIMER_LOGERROR("Bad Read7BitEncodedInt");
				return 0;
			}

			// ReadByte handles end of stream cases for us.
			b = ReadByte();
			count |= (b & 0x7F) << shift;
			shift += 7;
		} while ((b & 0x80) != 0);

		return count;
	}

	char Stream::ReadChar() const
	{
		char result = ReadByte();

		// Decode UTF-8.
		if (result & 0x80)
		{
			int byteCount = 1;

			while (result & (0x80 >> byteCount))
			{
				byteCount++;
			}

			result &= (1 << (8 - byteCount)) - 1;

			while (--byteCount)
			{
				result <<= 6;
				result |= ReadByte() & 0x3F;
			}
		}

		return result;
	}

	bool Stream::ReadBoolean() const
	{
		FillBuffer(1);
		return (_buffer[0] != 0);
	}

	uint16_t Stream::ReadUInt16() const
	{
		FillBuffer(2);
		return (uint16_t)(_buffer[0] | _buffer[1] << 8);
	}

	uint32_t Stream::ReadUInt32() const
	{
		FillBuffer(4);
		return (uint32_t)(_buffer[0] | _buffer[1] << 8 | _buffer[2] << 16 | _buffer[3] << 24);
	}

	uint64_t Stream::ReadUInt64() const
	{
		FillBuffer(8);
		uint32_t lo = (uint32_t)(_buffer[0] | _buffer[1] << 8 |
			_buffer[2] << 16 | _buffer[3] << 24);
		uint32_t hi = (uint32_t)(_buffer[4] | _buffer[5] << 8 |
			_buffer[6] << 16 | _buffer[7] << 24);
		return ((uint64_t)hi) << 32 | lo;
	}

	int16_t Stream::ReadInt16() const
	{
		FillBuffer(2);
		return (int16_t)(_buffer[0] | _buffer[1] << 8);
	}

	int32_t Stream::ReadInt32() const
	{
		FillBuffer(4);
		return (int32_t)(_buffer[0] | _buffer[1] << 8 | _buffer[2] << 16 | _buffer[3] << 24);
	}

	int64_t Stream::ReadInt64() const
	{
		FillBuffer(8);
		uint32_t lo = (uint32_t)(_buffer[0] | _buffer[1] << 8 |
			_buffer[2] << 16 | _buffer[3] << 24);
		uint32_t hi = (uint32_t)(_buffer[4] | _buffer[5] << 8 |
			_buffer[6] << 16 | _buffer[7] << 24);
		return (int64_t)((uint64_t)hi) << 32 | lo;
	}

	float Stream::ReadSingle() const
	{
		FillBuffer(4);
		uint32_t tmpBuffer = (uint32_t)(_buffer[0] | _buffer[1] << 8 | _buffer[2] << 16 | _buffer[3] << 24);
		return *((float*)&tmpBuffer);
	}

	double Stream::ReadDouble() const
	{
		FillBuffer(8);
		uint32_t lo = (uint32_t)(_buffer[0] | _buffer[1] << 8 |
			_buffer[2] << 16 | _buffer[3] << 24);
		uint32_t hi = (uint32_t)(_buffer[4] | _buffer[5] << 8 |
			_buffer[6] << 16 | _buffer[7] << 24);

		uint64_t tmpBuffer = ((uint64_t)hi) << 32 | lo;
		return *((double*)&tmpBuffer);
	}

	String Stream::ReadString(bool binaryMode)
	{
		String result;

		if (binaryMode)
		{
			uint64_t stringLength = (uint64_t)Read7BitEncodedInt();
			uint64_t endOfString = GetPosition() + stringLength;
			while (GetPosition() < endOfString)
			{
				result += ReadChar();
			}
		}
		else
		{
			while (!IsEof())
			{
				char c = ReadByte();
				if (!c)
					break;
				else
					result += c;
			}
		}

		return result;
	}

	void Stream::WriteByte(const uint8_t value)
	{
		_buffer[0] = value;
		Write(_buffer.data(), 1);
	}

	void Stream::Write(const bool value)
	{
		WriteByte(value ? 1 : 0);
	}

	void Stream::WriteChar(const char value)
	{
		Write(&value, sizeof(uint8_t));
	}

	void Stream::Write(const int8_t value)
	{
		WriteByte(value);
	}

	void Stream::Write(const uint16_t value)
	{
		_buffer[0] = (uint8_t)value;
		_buffer[1] = (uint8_t)(value >> 8);
		Write(_buffer.data(), 2);
	}

	void Stream::Write(const int16_t value)
	{
		_buffer[0] = (uint8_t)value;
		_buffer[1] = (uint8_t)(value >> 8);
		Write(_buffer.data(), 2);
	}

	void Stream::Write(const uint32_t value)
	{
		_buffer[0] = (uint8_t)value;
		_buffer[1] = (uint8_t)(value >> 8);
		_buffer[2] = (uint8_t)(value >> 16);
		_buffer[3] = (uint8_t)(value >> 24);
		Write(_buffer.data(), 4);
	}

	void Stream::Write(const int32_t value)
	{
		_buffer[0] = (uint8_t)value;
		_buffer[1] = (uint8_t)(value >> 8);
		_buffer[2] = (uint8_t)(value >> 16);
		_buffer[3] = (uint8_t)(value >> 24);
		Write(_buffer.data(), 4);
	}

	void Stream::Write(const uint64_t value)
	{
		_buffer[0] = (uint8_t)value;
		_buffer[1] = (uint8_t)(value >> 8);
		_buffer[2] = (uint8_t)(value >> 16);
		_buffer[3] = (uint8_t)(value >> 24);
		_buffer[4] = (uint8_t)(value >> 32);
		_buffer[5] = (uint8_t)(value >> 40);
		_buffer[6] = (uint8_t)(value >> 48);
		_buffer[7] = (uint8_t)(value >> 56);
		Write(_buffer.data(), 8);
	}

	void Stream::Write(const int64_t value)
	{
		_buffer[0] = (uint8_t)value;
		_buffer[1] = (uint8_t)(value >> 8);
		_buffer[2] = (uint8_t)(value >> 16);
		_buffer[3] = (uint8_t)(value >> 24);
		_buffer[4] = (uint8_t)(value >> 32);
		_buffer[5] = (uint8_t)(value >> 40);
		_buffer[6] = (uint8_t)(value >> 48);
		_buffer[7] = (uint8_t)(value >> 56);
		Write(_buffer.data(), 8);
	}

	void Stream::Write(const float value)
	{
		uint32_t uintValue = *(uint32_t*)(&value);
		_buffer[0] = (uint8_t)uintValue;
		_buffer[1] = (uint8_t)(uintValue >> 8);
		_buffer[2] = (uint8_t)(uintValue >> 16);
		_buffer[3] = (uint8_t)(uintValue >> 24);
		Write(_buffer.data(), 4);
	}

	void Stream::Write(const double value)
	{
		uint64_t uint64Value = *(uint64_t*)(&value);
		_buffer[0] = (uint8_t)uint64Value;
		_buffer[1] = (uint8_t)(uint64Value >> 8);
		_buffer[2] = (uint8_t)(uint64Value >> 16);
		_buffer[3] = (uint8_t)(uint64Value >> 24);
		_buffer[4] = (uint8_t)(uint64Value >> 32);
		_buffer[5] = (uint8_t)(uint64Value >> 40);
		_buffer[6] = (uint8_t)(uint64Value >> 48);
		_buffer[7] = (uint8_t)(uint64Value >> 56);
		Write(_buffer.data(), 8);
	}

	void Stream::Write7BitEncodedInt(uint32_t value)
	{
		// Write out an int 7 bits at a time.  The high bit of the byte,
		// when on, tells reader to continue reading more bytes. 
		while (value >= 0x80)
		{
			Write((uint8_t)(value | 0x80));
			value >>= 7;
		}
		Write((uint8_t)value);
	}

	void Stream::WriteString(const String& value, bool binaryMode)
	{
		if (binaryMode)
		{
			Write7BitEncodedInt((uint32_t)value.length());
			Write(value.c_str(), value.length());
		}
		else
		{
			// Null terminate string
			Write(value.c_str(), value.length() + 1);
		}
	}

	String Stream::ReadLine()
	{
		String ret;

		while (!IsEof())
		{
			char c = ReadByte();
			if (c == 10)
				break;
			if (c == 13)
			{
				// Peek next char to see if it's 10, and skip it too
				if (!IsEof())
				{
					char next = ReadByte();
					if (next != 10)
					{
						Seek(GetPosition() - 1, SeekOrigin::Begin);
					}
				}

				break;
			}

			ret += c;
		}

		return ret;
	}

	String Stream::ReadFileID()
	{
		String ret;
		ret.resize(4);
		Read(&ret[0], 4);
		return ret;
	}

	void Stream::WriteFileID(const String& value)
	{
		Write(value.c_str(), std::min<size_t>(value.length(), 4));
		for (size_t i = value.length(); i < 4; ++i)
		{
			Write(' ');
		}
	}

	bool Stream::IsEof() const
	{
		return GetPosition() >= Length();
	}

	String Stream::ReadAllText()
	{
		String content = "";
		uint64_t length = Length();
		if (length)
		{
			content.resize(length);
			Read(&content[0], length);
		}

		return content;
	}

	uint8_t* Stream::ReadAllBytes(uint64_t* fileSize)
	{
		uint64_t size = Length();

		uint8_t* buffer = new uint8_t[size + 1];
		uint64_t read = Read(buffer, size);
		if (read != size)
		{
			ALIMER_LOGERRORF("Failed to read complete contents of stream (amount read vs. file size: %u < %u).", read, size);
			SafeDeleteArray(buffer);
			return nullptr;
		}

		// Force the character buffer to be NULL-terminated.
		buffer[size] = '\0';

		if (fileSize) {
			*fileSize = size;
		}

		return buffer;
	}
}