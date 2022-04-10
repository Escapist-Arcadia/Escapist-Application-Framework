#pragma once

#include"EspBuffer.h"

class EspCharacterTraitInString :public EspDefaultSimpleTypeTrait<char>
{
public:
	static inline void CopyValue(char* dest, const char& value)
	{
		*dest = value;
	}
	static inline void CopyValueN(char* dest, const char& value, unsigned int count)
	{
		::memset(dest, value, count);
	}
	static inline void CopyBuffer(char* dest, const char* source, unsigned int size)
	{
		::memcpy(
			(void*)(dest),
			(const void*)(source),
			size
		);
	}
	static inline void CopyBufferN(char* dest, const char* source, unsigned int size, unsigned int count)
	{
		for (; count > 0; --count, dest += size)
			CopyBuffer(dest, source, size);
	}
	static inline void FillEmptyValue(char* dest, unsigned int count)
	{
		::memset(dest, 0, count);
	}
	static inline void MoveBuffer(char* dest, const char* source, unsigned int size)
	{
		::memmove(
			(void*)(dest),
			(const void*)(source),
			size
		);
	}
	static inline bool CompareBuffer(const char*& left, const char*& right, unsigned int size)
	{
		return ::memcmp(
			(const void*)(left),
			(const void*)(right),
			size
		);
	}

	static inline const char* Find(const char* data, unsigned int data_size, const char& value)
	{
		return (const char*)(::memchr(data, value, data_size));
	}
	static inline const char* ReserveFind(const char* data, unsigned int data_size, const char& value)
	{
		return (const char*)(::strrchr(data, value));
	}

	static inline unsigned int GetSize(const char* data)
	{
		return ::strlen(data);
	}
};

class EspCharacterTraitInWideString :public EspDefaultSimpleTypeTrait<wchar_t>
{
public:
	static inline void CopyValue(wchar_t* dest, const wchar_t& value)
	{
		::wmemset(dest, value, 1);
	}
	static inline void CopyValueN(wchar_t* dest, const wchar_t& value, unsigned int count)
	{
		::wmemset(dest, value, count);
	}
	static inline void CopyBuffer(wchar_t* dest, const wchar_t* source, unsigned int size)
	{
		::wmemcpy(
			(dest),
			(source),
			size
		);
	}
	static inline void CopyBufferN(wchar_t* dest, const wchar_t* source, unsigned int size, unsigned int count)
	{
		for (; count > 0; --count, dest += size)
			CopyBuffer(dest, source, size);
	}
	static inline void FillEmptyValue(wchar_t* dest, unsigned int count)
	{
		::wmemset(dest, 0, count);
	}
	static inline void MoveBuffer(wchar_t* dest, const wchar_t* source, unsigned int size)
	{
		::wmemmove(
			(dest),
			(source),
			size
		);
	}
	static inline void CompareBuffer(const wchar_t*& left, const wchar_t*& right, unsigned int size)
	{
		::wmemcmp(
			(left),
			(right),
			size
		);
	}

	static inline const wchar_t* IndexOf(const wchar_t* data, unsigned int data_size, const wchar_t& value)
	{
		return ::wmemchr(data, value, data_size);
	}
	static inline const wchar_t* LastIndexOf(const wchar_t* data, unsigned int data_size, const wchar_t& value)
	{
		return ::wcsrchr(data, value);
	}

	static inline unsigned int GetSize(const wchar_t* data) { return ::wcslen(data); }
};

using EspStringA = EspBuffer<char, EspCharacterTraitInString>;
using EspStringW = EspBuffer<wchar_t, EspCharacterTraitInWideString>;
using EspByteArray = EspBuffer<unsigned char, EspDefaultSimpleTypeTrait<unsigned char>>;
using EspStringPieceA = EspBufferPiece<char, EspCharacterTraitInString>;
using EspStringPieceW = EspBufferPiece<wchar_t, EspCharacterTraitInWideString>;

#ifdef _UNICODE
using EspString = EspStringW;
using EspStringPiece = EspStringPieceW;
#else
using EspString = EspStringA;
using EspStringPiece = EspStringPieceA;
#endif
