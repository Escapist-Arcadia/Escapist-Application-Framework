#pragma once
#include<tchar.h>
#include<atomic>
#include<memory>
#include<string>
#include<vector>

template<typename T>
void EspAllocBuffer(void*& buf)
{
	buf = ::malloc(sizeof(T));
	if (buf == nullptr)
		throw;
}
template<typename T>
void EspAllocBuffer(void*& buf, unsigned int size)
{
	buf = ::malloc(size * sizeof(T));
	if (buf == nullptr)
		throw;
}
template<typename T>
void EspAllocBuffer(T*& buf)
{
	buf = (T*)::malloc(sizeof(T));
	if (buf == nullptr)
		throw;
}
template<typename T>
void EspAllocBuffer(T*& buf, unsigned int size)
{
	buf = (T*)::malloc(size * sizeof(T));
	if (buf == nullptr)
		throw;
}

void EspDeAllocBuffer(void* buf)
{
	::free(buf);
}
template<typename T>
void EspDeAllocBuffer(T* buf)
{
	::free((void*)buf);
}

template<typename T> void EspDestroyInRef(T& ref) { ref.~T(); }
template<typename T> void EspDestroyInPtr(T* ptr) { ptr->~T(); }
template<typename T> void EspDestroyInPtr(void* ptr) { (reinterpret_cast<T*>(ptr))->~T(); }
template<typename T> void EspDestroyInCount(void* ptr, unsigned int count) { for (; count > 0; --count) (reinterpret_cast<T*>(ptr))->~T(); }
template<typename T> void EspDestroyInCount(T* ptr, unsigned int count) { for (; count > 0; --count) ptr->~T(); }

template<typename T>
class EspTypeTrait
{
public:
	static inline void CopyValue(T* ptr, const T& value);

	static inline void CopyValueN(T* ptr, const T& value, unsigned int count);

	static inline void CopyBuffer(T* ptr, const T* data, unsigned int size);

	static inline void CopyBufferN(T* ptr, const T* data, unsigned int size, unsigned int count);

	static inline void MoveBuffer(T* dest, const T* source, unsigned int size);

	static inline void FillEmptyValue(T* dest, unsigned int count);

	static inline void DestroyInRef(T& value);

	static inline void DestroyInPtr(T* ptr);

	static inline void DestroyInCount(T* data, unsigned int size);

	static inline bool Equals(const T& left, const T& right) { return (left == right); }

	static inline bool CompareBuffer(const T*& left, const T*& right, unsigned int size);

	static inline unsigned int ReckonSize(const T* data)
	{
		unsigned int length = 0;
		while (!EspTypeTrait<T>::Equals(*data, T()))
		{
			++length;
			++data;
		}
		return length;
	}

	static inline unsigned int GetSize(const T* data) { return ReckonSize(data); }

	static inline const T* Find(const T* data, unsigned int data_size, const T& value);

	static inline const T* Find(const T* data, unsigned int data_size, const T* target, unsigned int target_data_size);

	static inline const T* ReserveFind(const T* data, unsigned int data_size, const T& value);

	static inline const T* ReverseFind(const T* data, unsigned int data_size, const T* target, unsigned int target_data_size);
};

template<typename T>
class EspDefaultSimpleTypeTrait :public EspTypeTrait<T>
{
public:
	static inline void CopyValue(T* ptr, const T& value)
	{
		::memcpy(
			reinterpret_cast<void*>(ptr),
			reinterpret_cast<const void*>(&value),
			sizeof(T)
		);
	}

	static inline void CopyValueN(T* ptr, const T& value, unsigned int count)
	{
		::memcpy(
			reinterpret_cast<void*>(ptr),
			reinterpret_cast<const void*>(&value),
			count * sizeof(T)
		);
	}

	static inline void CopyBuffer(T* ptr, const T* data, unsigned int size)
	{
		::memcpy(
			reinterpret_cast<void*>(ptr),
			reinterpret_cast<const void*>(data),
			size * sizeof(T)
		);
	}

	static inline void CopyBufferN(T* ptr, const T* data, unsigned int size, unsigned int count)
	{
		for (; count > 0; ptr += size, --count)
			::memcpy(
				reinterpret_cast<void*>(ptr),
				reinterpret_cast<const void*>(data),
				size * sizeof(T)
			);
	}

	static inline void MoveBuffer(T* dest, const T* source, unsigned int size)
	{
		::memmove(
			reinterpret_cast<void*>(dest),
			reinterpret_cast<const void*>(source),
			size * sizeof(T)
		);
	}

	static inline void FillEmptyValue(T* dest, unsigned int count)
	{
		::memset(
			reinterpret_cast<void*>(dest),
			0,
			count * sizeof(T)
		);
	}

	static inline void DestroyInRef(T& value) {}

	static inline void DestroyInPtr(T* ptr) {}

	static inline void DestroyInCount(T* data, unsigned int size) {}

	static inline bool CompareBuffer(const T*& left, const T*& right, unsigned int size)
	{
		return ::memcmp(
			reinterpret_cast<const void*>(left),
			reinterpret_cast<const void*>(right),
			size * sizeof(T)
		);
	}

	static inline const T* Find(const T* data, unsigned int data_size, const T& value)
	{
		for (; data_size > 0; --data_size, ++data)
			if (EspDefaultSimpleTypeTrait<T>::Equals(*data, value))
				return data;
		return nullptr;
	}

	static inline const T* Find(const T* data, unsigned int data_size, const T* target, unsigned int target_data_size)
	{
		const T* data_pos = data;
		const T* target_pos = target;
		unsigned int target_checked_size = target_data_size;

		while (data_size != 0)
		{
			while (data_size && target_checked_size && EspDefaultSimpleTypeTrait<T>::Equals(*data_pos, *target_pos))
				++data_pos, ++target_pos, --data_size, --target_checked_size;
			if (target_checked_size == 0)
				return data;
			else if (target_checked_size != target_data_size)
			{
				data = data_pos;
				target_pos = target;
				target_checked_size = target_data_size;
			}
			else
				++data, ++data_pos, --data_size;
		}
		return nullptr;
	}

	static inline const T* ReserveFind(const T* data, unsigned int data_size, const T& value)
	{

	}

	static inline const T* ReverseFind(const T* data, unsigned int data_size, const T* target, unsigned int target_data_size)
	{

	}

	static inline unsigned int GetSize(const T* data) { return EspTypeTrait<T>::ReckonSize(data); }
};

template<typename T>
class EspDefaultGenericTypeTrait :public EspTypeTrait<T>
{
public:
	static inline void CopyValue(T* ptr, const T& value)
	{
		new(ptr)T(value);
	}

	static inline void CopyValueN(T* ptr, const T& value, unsigned int count)
	{
		for (; count > 0; --count, ++ptr)
			new(ptr)T(value);
	}

	static inline void CopyBuffer(T* ptr, const T* data, unsigned int size)
	{
		for (; size > 0; --size, ++ptr, ++data)
			new(ptr)T(*data);
	}

	static inline void CopyBufferN(T* ptr, const T* data, unsigned int size, unsigned int count)
	{
		for (; count > 0; ptr += size, --count)
			CopyBuffer(ptr, data, size);
	}

	static inline void MoveBuffer(T* dest, const T* source, unsigned int size)
	{
		if (dest == source || size == 0)
			return;
		if (dest > source || (dest - source) < (int)size)
		{
			T* dest_pos = dest + size;
			const T* source_pos = source + size;
			for (; source_pos >= source; --source_pos, --dest_pos)
				new(dest_pos)T(source_pos);
			return;
		}
		else
			CopyBuffer(dest, source, size);
	}

	static inline void FillEmptyValue(T* dest, unsigned int count)
	{
		::memset(
			reinterpret_cast<void*>(dest),
			0,
			count * sizeof(T)
		);
	}

	static inline void DestroyInRef(T& ref) { ref.~T(); }

	static inline void DestroyInPtr(T* ptr) { ptr->~T(); }

	static inline void DestroyInCount(T* data, unsigned int size)
	{
		for (; size > 0; --size, ++data)
			data->~T();
	}

	static inline bool CompareBuffer(const T*& left, const T*& right, unsigned int size)
	{
		return ::memcmp(
			reinterpret_cast<const void*>(left),
			reinterpret_cast<const void*>(right),
			size * sizeof(T)
		);
	}

	static inline const T* Find(const T* data, unsigned int data_size, const T& value)
	{
		for (; data_size > 0; --data_size, ++data)
			if (EspDefaultSimpleTypeTrait<T>::Equals(*data, value))
				return data;
		return nullptr;
	}

	static inline const T* Find(const T* data, unsigned int data_size, const T* target, unsigned int target_data_size)
	{
		const T* data_pos = data;
		const T* target_pos = target;
		unsigned int target_checked_size = target_data_size;

		while (data_size != 0)
		{
			while (data_size && target_checked_size && EspDefaultSimpleTypeTrait<T>::Equals(*data_pos, *target_pos))
				++data_pos, ++target_pos, --data_size, --target_checked_size;
			if (target_checked_size == 0)
				return data;
			else if (target_checked_size != target_data_size)
			{
				data = data_pos;
				target_pos = target;
				target_checked_size = target_data_size;
			}
			else
				++data, ++data_pos, --data_size;
		}
		return nullptr;
	}

	static inline const T* ReserveFind(const T* data, unsigned int data_size, const T& value)
	{

	}

	static inline const T* ReverseFind(const T* data, unsigned int data_size, const T* target, unsigned int target_data_size)
	{

	}

	static inline unsigned int GetSize(const T* data) { return EspTypeTrait::ReckonSize(data); }
};

enum class EspTypeTraitPattern
{
	Default_Simple_Pattern,
	Default_Generic_Pattern,
	Non_Default_Pattern
};

template<typename T>
constexpr bool IsComplexPattern = !std::_Is_any_of_v<std::remove_cv_t<T>,
	bool, char, signed char, unsigned char, wchar_t, char16_t, char32_t,
	short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long,
	float, double, long double,
	void, std::nullptr_t>;

template<typename T>
struct EspTypeTraitPatternDefiner
{
	static const EspTypeTraitPattern Pattern = IsComplexPattern<T> ? EspTypeTraitPattern::Default_Generic_Pattern : EspTypeTraitPattern::Default_Simple_Pattern;
};

#define DefineTypeTraitPattern(type, type_trait)\
template<>\
struct EspTypeTraitPatternDefiner<type>\
{\
	static const EspTypeTraitPattern Pattern = type_trait;\
};\

#define DefineSimpleTypeTraitPattern(type) DefineTypeTraitPattern(type, EspTypeTraitPattern::Default_Simple_Pattern)
#define DefineGenericTypeTraitPattern(type) DefineTypeTraitPattern(type, EspTypeTraitPattern::Default_Generic_Pattern)
#define DefineSpecificTypeTraitPattern(type) DefineTypeTraitPattern(type, EspTypeTraitPattern::Non_Default_Pattern)

DefineSimpleTypeTraitPattern(bool);
DefineSimpleTypeTraitPattern(char);
DefineSimpleTypeTraitPattern(signed char);
DefineSimpleTypeTraitPattern(unsigned char);
DefineSimpleTypeTraitPattern(wchar_t);
DefineSimpleTypeTraitPattern(char16_t);
DefineSimpleTypeTraitPattern(char32_t);
DefineSimpleTypeTraitPattern(short);
DefineSimpleTypeTraitPattern(unsigned short);
DefineSimpleTypeTraitPattern(int);
DefineSimpleTypeTraitPattern(unsigned int);
DefineSimpleTypeTraitPattern(long);
DefineSimpleTypeTraitPattern(unsigned long);
DefineSimpleTypeTraitPattern(long long);
DefineSimpleTypeTraitPattern(unsigned long long);
DefineSimpleTypeTraitPattern(float);
DefineSimpleTypeTraitPattern(double);
DefineSimpleTypeTraitPattern(long double);
DefineSimpleTypeTraitPattern(void);
DefineSimpleTypeTraitPattern(std::nullptr_t);


template<typename T, typename = void>
struct EspTypeTraitPatternSelector
{
	using TypeTrait = EspDefaultSimpleTypeTrait<T>;
};

template<typename T>
struct EspTypeTraitPatternSelector<T,
	typename std::enable_if<(EspTypeTraitPatternDefiner<T>::Pattern == EspTypeTraitPattern::Default_Simple_Pattern)>::type>
{
	using TypeTrait = EspDefaultSimpleTypeTrait<T>;
};

template<typename T>
struct EspTypeTraitPatternSelector<T,
	typename std::enable_if<(EspTypeTraitPatternDefiner<T>::Pattern == EspTypeTraitPattern::Default_Generic_Pattern)>::type>
{
	using TypeTrait = EspDefaultGenericTypeTrait<T>;
};

template<typename T>
struct EspTypeTraitPatternSelector<T,
	typename std::enable_if<(EspTypeTraitPatternDefiner<T>::Pattern == EspTypeTraitPattern::Non_Default_Pattern)>::type>
{
	using TypeTrait = EspTypeTrait<T>;
};

template<class T>
class EspAtomic
{
private:
	std::atomic<T> atom;

public:
	inline EspAtomic() {}
	//inline EspAtomic(const T nVal) { atom.store(nVal); }
	inline EspAtomic(const T& nVal) { atom.store(nVal); }

	inline T GetValue() { return atom.load(); }
	inline const T GetConstValue()const { return atom.load(); }

	//inline EspAtomic& SetValue(const T nVal) { atom.store(nVal); return *this; }
	inline EspAtomic& SetValue(const T& nVal) { atom.store(nVal); return *this; }

	inline EspAtomic& Add() { atom.fetch_add(1); return *this; }
	inline EspAtomic& Add(const T& nVal) { atom.fetch_add(nVal); return *this; }

	inline EspAtomic& Subtract() { atom.fetch_sub(1); return *this; }
	inline EspAtomic& Subtract(const T& nVal) { atom.fetch_sub(nVal); return *this; }

	inline bool IsLockFree()const { return atom.is_lock_free(); }
};

template<typename T>
class EspBufferInfo
{
public:
	EspAtomic<int> ref;
	unsigned int size;
	unsigned int capacity;
	T* data;

	inline EspBufferInfo()
		:ref(1), size(0), capacity(0), data(nullptr) {}
	inline EspBufferInfo(unsigned int new_size)
		: ref(1), size(new_size), capacity(new_size * 2) {
		EspAllocBuffer<T>(data, capacity);
	}
	inline EspBufferInfo(unsigned int new_size, unsigned int new_capacity)
		: ref(1), size(new_size), capacity(new_capacity) {
		EspAllocBuffer<T>(data, capacity);
	}

	inline bool IsEmpty()const { return (data == nullptr) || (size == 0); }
	inline bool IsNull()const { return (data == nullptr) || (capacity == 0); }
	inline bool IsEmptyOrNull()const { return (data == nullptr) || (capacity == 0) || (size == 0); }
	inline bool IsLocked()const { return (ref.GetConstValue() == -1); }
	inline bool IsShared()const { return (ref.GetConstValue() > 1); }

	inline unsigned int GetSize()const { return ((data == nullptr) || (capacity == 0)) ? 0 : size; }
	inline unsigned int GetCount()const { return ((data == nullptr) || (capacity == 0)) ? 0 : size; }
	inline unsigned int GetLength()const { return ((data == nullptr) || (capacity == 0)) ? 0 : size; }
	inline unsigned int GetCapacity()const { return data == nullptr ? 0 : capacity; }
};

template<typename T, typename TypeTrait = EspTypeTraitPatternSelector<T>::TypeTrait>
class EspBuffer;

template<typename T, typename TypeTrait = EspTypeTraitPatternSelector<T>::TypeTrait>
class EspBufferPiece;

template<typename T, typename TypeTrait>
class EspBuffer
{
private:
	EspBufferInfo<T>* info;

public:
	EspBuffer() { info = nullptr; }
	EspBuffer(const T& value)
	{
		::EspAllocBuffer<EspBufferInfo<T>>(info);
		new(info)EspBufferInfo<T>(1, 4);
		TypeTrait::CopyValue(info->data, value);
		TypeTrait::FillEmptyValue(info->data + 1, 3);
	}
	EspBuffer(const T& value, unsigned int count)
	{
		::EspAllocBuffer<EspBufferInfo<T>>(info);
		new(info)EspBufferInfo<T>(count);
		TypeTrait::CopyValueN(info->data, value, count);
		TypeTrait::FillEmptyValue(info->data + info->size, info->size);
	}
	EspBuffer(const T* data, unsigned int size)
	{
		::EspAllocBuffer<EspBufferInfo<T>>(info);
		new(info)EspBufferInfo<T>(size);
		TypeTrait::CopyBuffer(info->data, data, size);
		TypeTrait::FillEmptyValue(info->data + info->size, info->size);
	}
	EspBuffer(const T* data, unsigned int size, unsigned int count)
	{
		::EspAllocBuffer<EspBufferInfo<T>>(info);
		new(info)EspBufferInfo<T>(size * count);
		TypeTrait::CopyBufferN(info->data, data, size, count);
		TypeTrait::FillEmptyValue(info->data + info->size, info->size);
	}
	EspBuffer(const T* data) { new(this)EspBuffer<T>(data, TypeTrait::GetSize(data)); }
	EspBuffer(const EspBuffer& buffer)
	{
		if (buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
		{
			info = nullptr;
			return;
		}
		if (buffer.IsLocked())
			new(this)EspBuffer<T>(buffer.info->data, buffer.info->size);
		else
		{
			info = buffer.info;
			info->ref.Add();
		}
	}
	EspBuffer(const EspBuffer& buffer, unsigned int size)
	{
		if (size == 0 || buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
		{
			info = nullptr;
			return;
		}
		if (size == buffer.info->size)
		{
			if (buffer.info->IsLocked())
				new(this)EspBuffer<T>(buffer.info->data, buffer.info->size);
			else
			{
				info = buffer.info;
				info->ref.Add();
			}
		}
		else
			new(this)EspBuffer(buffer.info->data, size);
	}
	EspBuffer(const EspBuffer& buffer, unsigned int size, unsigned int count)
	{
		if (size == 0 || count == 0 || buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
			info = nullptr;
		else
			new(this)EspBuffer(buffer.info->data, size, count);
	}

	inline bool IsEmpty()const { return info->IsEmpty(); }
	inline bool IsNull()const { return info->IsNull(); }
	inline bool IsEmptyOrNull()const { return info->IsEmptyOrNull(); }
	inline bool IsLocked()const { return info->IsLocked(); }
	inline bool IsShared()const { return info->IsShared(); }

	inline unsigned int GetSize()const { return info->GetSize(); }
	inline unsigned int GetCount()const { return info->GetCount(); }
	inline unsigned int GetLength()const { return info->GetLength(); }
	EspBuffer& RefreshSize() { info->size = TypeTrait::GetSize(info->data); return *this; }
	EspBuffer& RefreshSize(unsigned int size) { (info->size = size); return *this; }
	inline unsigned int GetCapacity()const { return info->GetCapacity(); }

	inline T* GetData()
	{
		if (info->IsShared())
		{
			EspBufferInfo<T>* new_info(nullptr);
			::EspAllocBuffer<EspBufferInfo<T>>(new_info);
			new(new_info)EspBufferInfo<T>(info->size);
			TypeTrait::CopyBuffer(new_info->data, info->data, info->size);
			TypeTrait::FillEmptyValue(new_info->data + new_info->size, new_info->size);
			info->ref.Subtract();
			info = new_info;
		}
		return info->data;
	}
	inline const T* GetConstData()const { return info->data; }
	inline operator EspBufferPiece<T, TypeTrait>()const
	{
		if (info == nullptr || info->data == nullptr)
			return EspBufferPiece<T, TypeTrait>();
		return EspBufferPiece<T, TypeTrait>(info->data, info->size);
	}

	inline T& GetAt(unsigned int index)
	{
		if (info->IsShared())
		{
			EspBufferInfo<T>* new_info(nullptr);
			::EspAllocBuffer<EspBufferInfo<T>>(new_info);
			new(new_info)EspBufferInfo<T>(info->size);
			TypeTrait::CopyBuffer(new_info->data, info->data, info->size);
			TypeTrait::FillEmptyValue(new_info->data + new_info->size, new_info->size);
			info->ref.Subtract();
			info = new_info;
		}
		return info->data[index];
	}
	inline const T& GetConstAt(unsigned int index)const { return info->data[index]; }
	inline EspBuffer& SetAt(unsigned int index, const T& value)
	{
		if (index < info->size)
		{
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info(nullptr);
				::EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(info->size);
				TypeTrait::CopyBuffer(new_info->data, info->data, index);
				TypeTrait::CopyValue(new_info->data + index, value);
				TypeTrait::CopyBuffer(new_info->data + index + 1, info->data + index + 1, info->size - index - 1);
				TypeTrait::FillEmptyValue(new_info->data + new_info->size, new_info->size);
				info->ref.Subtract();
				info = new_info;
			}
			else
				TypeTrait::CopyValue(info->data + index, value);
		}
		return *this;
	}

	EspBuffer& Empty()
	{
		if (info == nullptr || info->data == nullptr)
			return *this;

		if (info->IsShared())
		{
			info->ref.Subtract();
			info = nullptr;
		}
		else
		{
			EspDestroyInCount<T>(info->data, info->size);
			TypeTrait::FillEmptyValue(info->data, info->size);
		}
		return *this;
	}
	inline bool Compare(const T*& data)const { return TypeTrait::CompareBuffer(info->data, data, info->size); }
	inline bool Compare(const EspBuffer& buffer)const
	{
		if (info == buffer.info)
			return true;
		else
			return TypeTrait::CompareBuffer(info->data, buffer.info->data, info->size);
	}

	/*
	inline const T* Find(const T& target)const { return TypeTrait::Find(info->data, info->size, target); }
	inline const T* Find(const T* target, unsigned int size) { return TypeTrait::Find(info->data,info->size,); }
	inline const T* ReserveFind(const T& target)const { return TypeTrait::ReserveFind(info->data, info->size, target); }
	inline const T* ReverseFind(const T* target, unsigned int size) { return TypeTrait::Find(info->data, info->size, target, size); }

	inline const unsigned int IndexOf(const T& target)
	{
		const T* pos = TypeTrait::Find(info->data, info->size, target);
		return pos != nullptr ? pos - info->data : nullptr;
	}
	inline const unsigned int IndexOf(const T* target, unsigned int size)
	{
		const T* pos = TypeTrait::Find(info->data, info->size, target, size);
		return pos != nullptr ? pos - info->data : nullptr;
	}
	inline const unsigned int LastIndexOf(const T& target)
	{
		const T* pos = TypeTrait::ReverseFind(info->data, info->size, target);
		return pos != nullptr ? pos - info->data : nullptr;
	}
	inline const unsigned int LastIndexOf(const T* target, unsigned int size)
	{
		const T* pos = TypeTrait::ReverseFind(info->data, info->size, target, size);
		return pos != nullptr ? pos - info->data : nullptr;
	}
	*/

	EspBuffer& SetCapacity(unsigned int new_capacity)
	{
		if (new_capacity == 0)
		{
			if (info == nullptr || info->data == nullptr)
				return *this;
			if (info == nullptr || info->data == nullptr || info->size == 0 || info->capacity == 0)
				return *this;
			if (info->IsShared())
			{
				info->ref.Subtract();
				info = nullptr;
			}
			else
			{
				EspDestroyInCount<T>(info->data, info->size);
				EspDeAllocBuffer<T>(info->data);
				info->data = nullptr;
			}
		}
		else
		{
			if (info == nullptr)
			{
				EspAllocBuffer<EspBufferInfo<T>>(info);
				new(info)EspBufferInfo<T>(0, new_capacity);

				TypeTrait::FillEmptyValue(info->data, new_capacity);
			}
			else if (info->data == nullptr)
			{
				EspAllocBuffer<T>(info->data, new_capacity);
				TypeTrait::FillEmptyValue(info->data, new_capacity);

				info->size = 0;
				info->capacity = new_capacity;
			}
			else
			{
				if (new_capacity != info->capacity)
				{
					if (info->IsShared())
					{
						EspBufferInfo<T>* new_info(nullptr);
						EspAllocBuffer<EspBufferInfo<T>>(new_info);
						new(new_info)EspBufferInfo<T>(info->size, new_capacity);

						TypeTrait::CopyBuffer(new_info->data, info->data, info->size);
						TypeTrait::FillEmptyValue(new_info->data + info->size, new_capacity - info->size);

						info->ref.Subtract();
						info = new_info;
					}
					else
					{
						T* new_data(nullptr);
						EspAllocBuffer<T>(new_data, new_capacity);

						EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, info->size);
						TypeTrait::FillEmptyValue(new_data + info->size, new_capacity - info->size);

						EspDeAllocBuffer<T>(info->data);
						info->capacity = new_capacity;
						info->data = new_data;
					}
				}
			}
		}
		return *this;

	}
	EspBuffer& SetSize(unsigned int new_size)
	{
		if (new_size == 0)
		{
			if (info == nullptr || info->data == nullptr || info->size == 0 || info->capacity == 0)
				return *this;
			if (info->IsShared())
			{
				info->ref.Subtract();
				info = nullptr;
			}
			else
			{
				TypeTrait::FillEmptyValue(info->data, info->size);
				info->size = 0;
			}
		}
		else
		{
			if (info == nullptr)
			{
				EspAllocBuffer<EspBufferInfo<T>>(info);
				new(info)EspBufferInfo<T>(new_size);

				TypeTrait::FillEmptyValue(info->data + new_size, new_size);
			}
			else if (info->data == nullptr)
			{
				unsigned int new_capacity = new_size * 2;
				EspAllocBuffer<T>(info->data, new_capacity);
				TypeTrait::FillEmptyValue(info->data + new_size, new_size);

				info->size = new_size;
				info->capacity = new_capacity;
			}
			else
			{
				if (info->size != new_size)
				{
					if (info->IsShared())
					{
						EspBufferInfo<T>* new_info(nullptr);
						EspAllocBuffer<EspBufferInfo<T>>(new_info);
						new(new_info)EspBufferInfo<T>(new_size);

						TypeTrait::CopyBuffer(new_info->data, info->data, new_size > info->size ? info->size : new_size);
						TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

						info->ref.Subtract();
						info = new_info;
					}
					else
					{
						if (new_size < info->capacity)
						{
							if (new_size < info->size)
								TypeTrait::FillEmptyValue(info->data + new_size, info->size - new_size);
							info->size = new_size;
						}
						else
						{
							unsigned int new_capacity = new_size * 2;
							T* new_data(nullptr);
							EspAllocBuffer<T>(new_data, new_capacity);

							TypeTrait::CopyBuffer(new_data, info->data, info->size);
							TypeTrait::FillEmptyValue(new_data + new_size, new_size);

							EspDeAllocBuffer<T>(info->data);
							info->size = new_size;
							info->capacity = new_capacity;
							info->data = new_data;
						}
					}
				}
			}
		}
		return *this;
	}
	EspBuffer& SetSize(unsigned int new_size, unsigned int new_capacity)
	{

	}

	EspBuffer& Append(const T& value)
	{
		if (info == nullptr)
		{
			EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(1, 4);

			TypeTrait::CopyValue(info->data, value);
			TypeTrait::FillEmptyValue(info->data + 1, 3);
		}
		else if (info->data == nullptr)
		{
			EspAllocBuffer<T>(info->data, 4);
			TypeTrait::CopyValue(info->data, value);
			TypeTrait::FillEmptyValue(info->data + 1, 3);

			info->size = 1;
			info->capacity = 4;
		}
		else
		{
			unsigned int new_size = info->size + 1;
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(new_size);

				TypeTrait::CopyBuffer(new_info->data, info->data, info->size);
				TypeTrait::CopyValue(new_info->data + info->size, value);
				TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (new_size < info->capacity)
				{
					TypeTrait::CopyValue(info->data + info->size, value);

					info->size = new_size;
				}
				else
				{
					unsigned int new_capacity = new_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, info->size);
					TypeTrait::CopyValue(new_data + info->size, value);
					TypeTrait::FillEmptyValue(new_data + new_size, new_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = new_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Append(const T& value, unsigned int count)
	{
		if (count == 0)
			return *this;

		if (info == nullptr)
		{
			EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(count);

			TypeTrait::CopyValueN(info->data, value, count);
			TypeTrait::FillEmptyValue(info->data + count, count);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = count * 2;
			EspAllocBuffer<T>(info->data, new_capacity);
			TypeTrait::CopyValueN(info->data, value, count);
			TypeTrait::FillEmptyValue(info->data + count, count);

			info->size = count;
			info->capacity = new_capacity;
		}
		else
		{
			unsigned int new_size = info->size + count;
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(new_size);

				TypeTrait::CopyBuffer(new_info->data, info->data, info->size);
				TypeTrait::CopyValueN(new_info->data + info->size, value, count);
				TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (new_size < info->capacity)
				{
					TypeTrait::CopyValueN(info->data + info->size, value, count);

					info->size = new_size;
				}
				else
				{
					unsigned int new_capacity = new_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, info->size);
					TypeTrait::CopyValueN(new_data + info->size, value, count);
					TypeTrait::FillEmptyValue(new_data + new_size, new_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = new_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Append(const T* data, unsigned int size)
	{
		if (data == nullptr || size == 0)
			return *this;

		if (info == nullptr)
		{
			EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(size);

			TypeTrait::CopyBuffer(info->data, data, size);
			TypeTrait::FillEmptyValue(info->data + size, size);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = size * 2;
			EspAllocBuffer<T>(info->data, new_capacity);
			TypeTrait::CopyBuffer(info->data, data, size);
			TypeTrait::FillEmptyValue(info->data + size, size);

			info->size = size;
			info->capacity = new_capacity;
		}
		else
		{
			unsigned int new_size = info->size + size;
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(new_size);

				TypeTrait::CopyBuffer(new_info->data, info->data, info->size);
				TypeTrait::CopyBuffer(new_info->data + info->size, data, size);
				TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (new_size < info->capacity)
				{
					TypeTrait::CopyBuffer(info->data + info->size, data, size);

					info->size = new_size;
				}
				else
				{
					unsigned int new_capacity = new_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, info->size);
					TypeTrait::CopyBuffer(new_data + info->size, data, size);
					TypeTrait::FillEmptyValue(new_data + new_size, new_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = new_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Append(const T* data, unsigned int size, unsigned int count)
	{
		if (data == nullptr || size == 0 || count == 0)
			return *this;

		unsigned int add_size = size * count;
		if (info == nullptr)
		{
			EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(add_size);

			TypeTrait::CopyBufferN(info->data, data, size, count);
			TypeTrait::FillEmptyValue(info->data + add_size, add_size);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = add_size * 2;
			EspAllocBuffer<T>(info->data, new_capacity);
			TypeTrait::CopyBufferN(info->data, data, size, count);
			TypeTrait::FillEmptyValue(info->data + add_size, add_size);

			info->size = add_size;
			info->capacity = new_capacity;
		}
		else
		{
			unsigned int new_size = info->size + add_size;
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(new_size);

				TypeTrait::CopyBuffer(new_info->data, info->data, info->size);
				TypeTrait::CopyBufferN(new_info->data + info->size, data, size, count);
				TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (new_size < info->capacity)
				{
					TypeTrait::CopyBufferN(info->data + info->size, data, size, count);

					info->size = new_size;
				}
				else
				{
					unsigned int new_capacity = new_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, info->size);
					TypeTrait::CopyBufferN(new_data + info->size, data, size, count);
					TypeTrait::FillEmptyValue(new_data + new_size, new_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = new_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Append(const T* data)
	{
		return Append(data, TypeTrait::GetSize(data));
	}
	EspBuffer& Append(const EspBuffer& buffer)
	{
		if (buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
			return *this;
		if (info == nullptr)
		{
			if (buffer.info->IsLocked())
			{
				::EspAllocBuffer<EspBufferInfo<T>>(info);
				new(info)EspBufferInfo<T>(buffer.info->size);

				TypeTrait::CopyBuffer(info->data, buffer.info->data, buffer.info->size);
				TypeTrait::FillEmptyValue(info->data + info->size, info->size);
			}
			else
			{
				info = buffer.info;
				info->ref.Add();
			}
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = buffer.info->size * 2;
			::EspAllocBuffer<T>(info->data, new_capacity);

			TypeTrait::CopyBuffer(info->data, buffer.info->data, buffer.info->size);
			TypeTrait::FillEmptyValue(info->data + buffer.info->size, buffer.info->size);

			info->size = buffer.info->size;
			info->capacity = new_capacity;
		}
		else
		{
			unsigned int new_size = info->size + buffer.info->size;
			if (new_size < info->capacity)
			{
				TypeTrait::CopyBuffer(info->data + info->size, buffer.info->data, buffer.info->size);

				info->size = new_size;
			}
			else
			{
				unsigned int new_capacity = new_size * 2;
				T* new_data;
				EspAllocBuffer<T>(new_data, new_capacity);

				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, info->size);
				TypeTrait::CopyBuffer(new_data + info->size, buffer.info->data, buffer.info->size);
				TypeTrait::FillEmptyValue(new_data + new_size, new_size);

				EspDeAllocBuffer<T>(info->data);
				info->size = new_size;
				info->capacity = new_capacity;
				info->data = new_data;
			}
		}
		return *this;
	}
	EspBuffer& Append(const EspBuffer& buffer, unsigned int size)
	{
		if (size == 0 || buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
			return *this;
		if (size >= buffer.info->size)
			Append(buffer);

		if (info == nullptr)
		{
			::EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(size);

			TypeTrait::CopyBuffer(info->data, buffer.info->data, size);
			TypeTrait::FillEmptyValue(info->data + size, size);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = size * 2;
			::EspAllocBuffer<T>(info->data, new_capacity);

			TypeTrait::CopyBuffer(info->data, buffer.info->data, size);
			TypeTrait::FillEmptyValue(info->data + size, size);

			info->size = size;
			info->capacity = new_capacity;
		}
		else
		{
			unsigned int new_size = info->size + size;
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(new_size);

				TypeTrait::CopyBuffer(new_info->data, info->data, info->size);
				TypeTrait::CopyBuffer(new_info->data + info->size, buffer.info->data, size);
				TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (new_size < info->capacity)
				{
					TypeTrait::CopyBuffer(info->data + info->size, buffer.info->data, size);

					info->size = new_size;
				}
				else
				{
					unsigned int new_capacity = new_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, info->size);
					TypeTrait::CopyBuffer(new_data + info->size, buffer.info->data, size);
					TypeTrait::FillEmptyValue(new_data + new_size, new_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = new_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Append(const EspBuffer& buffer, unsigned int size, unsigned int count)
	{
		if (size == 0 || buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
			return *this;
		if (size > buffer.info->size)
			size = buffer.info->size;

		unsigned int add_size = size * count;
		if (info == nullptr)
		{
			::EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(add_size);

			TypeTrait::CopyBufferN(info->data, buffer.info->data, size, count);
			TypeTrait::FillEmptyValue(info->data + add_size, add_size);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = add_size * 2;
			::EspAllocBuffer<T>(info->data, new_capacity);

			TypeTrait::CopyBufferN(info->data, buffer.info->data, size, count);
			TypeTrait::FillEmptyValue(info->data + add_size, add_size);

			info->size = add_size;
			info->capacity = new_capacity;
		}
		else
		{
			unsigned int new_size = info->size + add_size;
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(new_size);

				TypeTrait::CopyBuffer(new_info->data, info->data, info->size);
				TypeTrait::CopyBufferN(new_info->data + info->size, buffer.info->data, size, count);
				TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (new_size < info->capacity)
				{
					TypeTrait::CopyBufferN(info->data + info->size, buffer.info->data, size, count);

					info->size = new_size;
				}
				else
				{
					unsigned int new_capacity = new_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, info->size);
					TypeTrait::CopyBufferN(new_data + info->size, buffer.info->data, size, count);
					TypeTrait::FillEmptyValue(new_data + new_size, new_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = new_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}

	EspBuffer& Assign(const T& value)
	{
		if (info == nullptr)
		{
			EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(1, 4);

			TypeTrait::CopyValue(info->data, value);
			TypeTrait::FillEmptyValue(info->data + 1, 3);
		}
		else if (info->data == nullptr)
		{
			EspAllocBuffer<T>(info->data, 4);
			TypeTrait::CopyValue(info->data, value);
			TypeTrait::FillEmptyValue(info->data + 1, 3);

			info->size = 1;
			info->capacity = 4;
		}
		else
		{
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(1, 4);

				TypeTrait::CopyValue(new_info->data, value);
				TypeTrait::FillEmptyValue(new_info->data + 1, 3);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (info->capacity > 1)
				{
					EspDestroyInCount<T>(info->data, info->size);
					TypeTrait::CopyValue(info->data, value);
					if (info->size > 1)
						TypeTrait::FillEmptyValue(info->data + 1, info->size - 1);
					info->size = 1;
				}
				else
				{
					T* new_data;
					EspAllocBuffer<T>(new_data, 4);

					TypeTrait::CopyValue(new_data, value);
					TypeTrait::FillEmptyValue(new_data + 1, 3);

					EspDeAllocBuffer<T>(info->data);
					info->size = 1;
					info->capacity = 4;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Assign(const T& value, unsigned int count)
	{
		if (count == 0)
			return *this;

		if (info == nullptr)
		{
			EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(count);

			TypeTrait::CopyValueN(info->data, value, count);
			TypeTrait::FillEmptyValue(info->data + count, count);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = count * 2;
			EspAllocBuffer<T>(info->data, new_capacity);
			TypeTrait::CopyValueN(info->data, value, count);
			TypeTrait::FillEmptyValue(info->data + count, count);

			info->size = count;
			info->capacity = new_capacity;
		}
		else
		{
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(count);

				TypeTrait::CopyValueN(new_info->data, value, count);
				TypeTrait::FillEmptyValue(new_info->data + count, count);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (count < info->capacity)
				{
					EspDestroyInCount<T>(info->data, info->size);
					TypeTrait::CopyValueN(info->data, value, count);
					if (count < info->size)
						TypeTrait::FillEmptyValue(info->data + count, info->size - count);
					info->size = count;
				}
				else
				{
					unsigned int new_capacity = count * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					TypeTrait::CopyValueN(new_data, value, count);
					TypeTrait::FillEmptyValue(new_data + count, count);

					EspDeAllocBuffer<T>(info->data);
					info->size = count;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Assign(const T* data, unsigned int size)
	{
		if (data == nullptr || size == 0)
			return *this;

		if (info == nullptr)
		{
			EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(size);

			TypeTrait::CopyBuffer(info->data, data, size);
			TypeTrait::FillEmptyValue(info->data + size, size);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = size * 2;
			EspAllocBuffer<T>(info->data, new_capacity);
			TypeTrait::CopyBuffer(info->data, data, size);
			TypeTrait::FillEmptyValue(info->data + size, size);

			info->size = size;
			info->capacity = new_capacity;
		}
		else
		{
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(size);

				TypeTrait::CopyBuffer(new_info->data, data, size);
				TypeTrait::FillEmptyValue(new_info->data + size, size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (size < info->capacity)
				{
					EspDestroyInCount<T>(info->data, info->size);
					TypeTrait::CopyBuffer(info->data, data, size);
					if (size < info->size)
						TypeTrait::FillEmptyValue(info->data + size, info->size - size);
					info->size = size;
				}
				else
				{
					unsigned int new_capacity = size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					TypeTrait::CopyBuffer(new_data, data, size);
					TypeTrait::FillEmptyValue(new_data + size, size);

					EspDeAllocBuffer<T>(info->data);
					info->size = size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Assign(const T* data)
	{
		Assign(data, TypeTrait::GetSize(data));
	}
	EspBuffer& Assign(const EspBuffer& buffer)
	{
		if (buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
			return *this;

		if (info == nullptr)
		{
			if (buffer.info->IsLocked())
			{
				::EspAllocBuffer<EspBufferInfo<T>>(info);
				new(info)EspBufferInfo<T>(buffer.info->size);

				TypeTrait::CopyBuffer(info->data, buffer.info->data, buffer.info->size);
				TypeTrait::FillEmptyValue(info->data + info->size, info->size);
			}
			else
			{
				info = buffer.info;
				info->ref.Add();
			}
		}
		if (info == nullptr)
		{
			EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(buffer.info->size);

			TypeTrait::CopyBuffer(info->data, buffer.info->data, buffer.info->size);
			TypeTrait::FillEmptyValue(info->data + buffer.info->size, buffer.info->size);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = buffer.info->size * 2;
			EspAllocBuffer<T>(info->data, new_capacity);
			TypeTrait::CopyBuffer(info->data, buffer.info->data, buffer.info->size);
			TypeTrait::FillEmptyValue(info->data + buffer.info->size, buffer.info->size);

			info->size = buffer.info->size;
			info->capacity = new_capacity;
		}
		else
		{
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(buffer.info->size);

				TypeTrait::CopyBuffer(new_info->data, buffer.info->data, buffer.info->size);
				TypeTrait::FillEmptyValue(new_info->data + buffer.info->size, buffer.info->size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (buffer.info->size < info->capacity)
				{
					EspDestroyInCount<T>(info->data, info->size);
					TypeTrait::CopyBuffer(info->data, buffer.info->data, buffer.info->size);
					if (buffer.info->size < info->size)
						TypeTrait::FillEmptyValue(info->data + buffer.info->size, info->size - buffer.info->size);
					info->size = buffer.info->size;
				}
				else
				{
					unsigned int new_capacity = buffer.info->size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					TypeTrait::CopyBuffer(new_data, buffer.info->data, buffer.info->size);
					TypeTrait::FillEmptyValue(new_data + buffer.info->size, buffer.info->size);

					EspDeAllocBuffer<T>(info->data);
					info->size = buffer.info->size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Assign(const EspBuffer& buffer, unsigned int size)
	{
		if (size == 0 || buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
			return *this;
		if (size >= buffer.info->size)
			Append(buffer);

		if (info == nullptr)
		{
			EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(size);

			TypeTrait::CopyBuffer(info->data, buffer.info->data, size);
			TypeTrait::FillEmptyValue(info->data + size, size);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = size * 2;
			EspAllocBuffer<T>(info->data, new_capacity);
			TypeTrait::CopyBuffer(info->data, buffer.info->data, size);
			TypeTrait::FillEmptyValue(info->data + size, size);

			info->size = size;
			info->capacity = new_capacity;
		}
		else
		{
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(buffer.info->size);

				TypeTrait::CopyBuffer(new_info->data, buffer.info->data, size);
				TypeTrait::FillEmptyValue(new_info->data + size, size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (size < info->capacity)
				{
					EspDestroyInCount<T>(info->data, info->size);
					TypeTrait::CopyBuffer(info->data, buffer.info->data, size);
					if (size < info->size)
						TypeTrait::FillEmptyValue(info->data + size, info->size - size);
					info->size = size;
				}
				else
				{
					unsigned int new_capacity = size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					TypeTrait::CopyBuffer(new_data, buffer.info->data, size);
					TypeTrait::FillEmptyValue(new_data + size, size);

					EspDeAllocBuffer<T>(info->data);
					info->size = size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Assign(const EspBuffer& buffer, unsigned int size, unsigned int count)
	{
		if (size == 0 || count == 0 || buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
			return *this;

		unsigned int add_size = size * count;
		if (info == nullptr)
		{
			::EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(add_size);

			TypeTrait::CopyBufferN(info->data, buffer.info->data, size, count);
			TypeTrait::FillEmptyValue(info->data + add_size, add_size);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = add_size * 2;
			::EspAllocBuffer<T>(info->data, new_capacity);

			TypeTrait::CopyBufferN(info->data, buffer.info->data, size, count);
			TypeTrait::FillEmptyValue(info->data + add_size, add_size);

			info->size = add_size;
			info->capacity = new_capacity;
		}
		else
		{
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(add_size);

				TypeTrait::CopyBufferN(new_info->data, buffer.info->data, size, count);
				TypeTrait::FillEmptyValue(new_info->data + add_size, add_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (size < info->capacity)
				{
					EspDestroyInCount<T>(info->data, info->size);
					TypeTrait::CopyBufferN(info->data, buffer.info->data, size, count);
					if (add_size < info->size)
						TypeTrait::FillEmptyValue(info->data + add_size, info->size - add_size);
					info->size = add_size;
				}
				else
				{
					unsigned int new_capacity = add_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					TypeTrait::CopyBufferN(new_data, buffer.info->data, size, count);
					TypeTrait::FillEmptyValue(new_data + add_size, add_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = add_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}

	EspBuffer& Prepend(const T& value)
	{
		if (info == nullptr)
		{
			EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(1, 4);

			TypeTrait::CopyValue(info->data, value);
			TypeTrait::FillEmptyValue(info->data + 1, 3);
		}
		else if (info->data == nullptr)
		{
			EspAllocBuffer<T>(info->data, 4);
			TypeTrait::CopyValue(info->data, value);
			TypeTrait::FillEmptyValue(info->data + 1, 3);

			info->size = 1;
			info->capacity = 4;
		}
		else
		{
			unsigned int new_size = info->size + 1;
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(new_size);

				TypeTrait::CopyValue(new_info->data, value);
				TypeTrait::CopyBuffer(new_info->data + 1, info->data, info->size);
				TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (new_size < info->capacity)
				{
					EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + 1, info->data, info->size);
					TypeTrait::CopyValue(info->data, value);

					info->size = new_size;
				}
				else
				{
					unsigned int new_capacity = new_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					TypeTrait::CopyValue(new_data, value);
					EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + 1, info->data, info->size);
					TypeTrait::FillEmptyValue(new_data + new_size, new_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = new_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
			return *this;
		}
	}
	EspBuffer& Prepend(const T& value, unsigned int count)
	{
		if (count == 0)
			return *this;

		if (info == nullptr)
		{
			EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(count);

			TypeTrait::CopyValueN(info->data, value, count);
			TypeTrait::FillEmptyValue(info->data + count, count);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = count * 2;
			EspAllocBuffer<T>(info->data, new_capacity);
			TypeTrait::CopyValueN(info->data, value, count);
			TypeTrait::FillEmptyValue(info->data + count, count);

			info->size = count;
			info->capacity = new_capacity;
		}
		else {
			unsigned int new_size = info->size + count;
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(new_size);

				TypeTrait::CopyValueN(new_info->data, value, count);
				TypeTrait::CopyBuffer(new_info->data + count, info->data, info->size);
				TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (new_size < info->capacity)
				{
					EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + count, info->data, info->size);
					TypeTrait::CopyValueN(info->data, value, count);

					info->size = new_size;
				}
				else
				{
					unsigned int new_capacity = new_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					TypeTrait::CopyValueN(new_data, value, count);
					EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + count, info->data, info->size);
					TypeTrait::FillEmptyValue(new_data + new_size, new_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = new_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Prepend(const T* data, unsigned int size)
	{
		if (data == nullptr || size == 0)
			return *this;

		if (info == nullptr)
		{
			EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(size);

			TypeTrait::CopyBuffer(info->data, data, size);
			TypeTrait::FillEmptyValue(info->data + size, size);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = size * 2;
			EspAllocBuffer<T>(info->data, new_capacity);
			TypeTrait::CopyBuffer(info->data, data, size);
			TypeTrait::FillEmptyValue(info->data + size, size);

			info->size = size;
			info->capacity = new_capacity;
		}
		else
		{
			unsigned int new_size = info->size + size;
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(new_size);

				TypeTrait::CopyBuffer(new_info->data, info->data, info->size);
				TypeTrait::CopyBuffer(new_info->data, data, size);
				TypeTrait::CopyBuffer(new_info->data + size, info->data, info->size);
				TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (new_size < info->capacity)
				{
					EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + size, info->data, info->size);
					TypeTrait::CopyBuffer(info->data, data, size);

					info->size = new_size;
				}
				else
				{
					unsigned int new_capacity = new_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					TypeTrait::CopyBuffer(new_data, data, size);
					EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + size, info->data, info->size);
					TypeTrait::FillEmptyValue(new_data + new_size, new_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = new_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Prepend(const T* data, unsigned int size, unsigned int count)
	{
		if (data == nullptr || size == 0 || count == 0)
			return *this;

		unsigned int add_size = size * count;
		if (info == nullptr)
		{
			EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(add_size);

			TypeTrait::CopyBufferN(info->data, data, size, count);
			TypeTrait::FillEmptyValue(info->data + add_size, add_size);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = add_size * 2;
			EspAllocBuffer<T>(info->data, new_capacity);
			TypeTrait::CopyBufferN(info->data, data, size, count);
			TypeTrait::FillEmptyValue(info->data + add_size, add_size);

			info->size = add_size;
			info->capacity = new_capacity;
		}
		else
		{
			unsigned int new_size = info->size + add_size;
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(new_size);

				TypeTrait::CopyBufferN(new_info->data, data, size, count);
				TypeTrait::CopyBuffer(new_info->data + add_size, info->data, info->size);
				TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (new_size < info->capacity)
				{
					EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + add_size, info->data, info->size);
					TypeTrait::CopyBufferN(info->data, data, size, count);

					info->size = new_size;
				}
				else
				{
					unsigned int new_capacity = new_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					TypeTrait::CopyBufferN(new_data, data, size, count);
					EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + add_size, info->data, info->size);
					TypeTrait::FillEmptyValue(new_data + new_size, new_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = new_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Prepend(const T* data)
	{
		return Prepend(data, TypeTrait::GetSize(data));
	}
	EspBuffer& Prepend(const EspBuffer& buffer)
	{
		if (buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
			return *this;
		if (info == nullptr)
		{
			if (buffer.info->IsLocked())
			{
				::EspAllocBuffer<EspBufferInfo<T>>(info);
				new(info)EspBufferInfo<T>(buffer.info->size);

				TypeTrait::CopyBuffer(info->data, buffer.info->data, buffer.info->size);
				TypeTrait::FillEmptyValue(info->data + info->size, info->size);
			}
			else
			{
				info = buffer.info;
				info->ref.Add();
			}
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = buffer.info->size * 2;
			::EspAllocBuffer<T>(info->data, new_capacity);

			TypeTrait::CopyBuffer(info->data, buffer.info->data, buffer.info->size);
			TypeTrait::FillEmptyValue(info->data + buffer.info->size, buffer.info->size);

			info->size = buffer.info->size;
			info->capacity = new_capacity;
		}
		else
		{
			unsigned int new_size = info->size + buffer.info->size;
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(new_size);

				TypeTrait::CopyBuffer(new_info->data, buffer.info->data, buffer.info->size);
				TypeTrait::CopyBuffer(new_info->data + buffer.info->size, info->data, info->size);
				TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (new_size < info->capacity)
				{
					EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + buffer.info->size, info->data, info->size);
					TypeTrait::CopyBuffer(info->data, buffer.info->data, buffer.info->size);

					info->size = new_size;
				}
				else
				{
					unsigned int new_capacity = new_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					TypeTrait::CopyBuffer(new_data, buffer.info->data, buffer.info->size);
					EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + buffer.info->size, info->data, info->size);
					TypeTrait::FillEmptyValue(new_data + new_size, new_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = new_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Prepend(const EspBuffer& buffer, unsigned int size)
	{
		if (size == 0 || buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
			return *this;
		if (size >= buffer.info->size)
			Prepend(buffer);

		if (info == nullptr)
		{
			::EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(size);

			TypeTrait::CopyBuffer(info->data, buffer.info->data, size);
			TypeTrait::FillEmptyValue(info->data + size, size);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = size * 2;
			::EspAllocBuffer<T>(info->data, new_capacity);

			TypeTrait::CopyBuffer(info->data, buffer.info->data, size);
			TypeTrait::FillEmptyValue(info->data + size, size);

			info->size = size;
			info->capacity = new_capacity;
		}
		else
		{
			unsigned int new_size = info->size + size;
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(new_size);

				TypeTrait::CopyBuffer(new_info->data, buffer.info->data, size);
				TypeTrait::CopyBuffer(new_info->data + size, info->data, info->size);
				TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (new_size < info->capacity)
				{
					EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + size, info->data, info->size);
					TypeTrait::CopyBuffer(info->data, buffer.info->data, size);

					info->size = new_size;
				}
				else
				{
					unsigned int new_capacity = new_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					TypeTrait::CopyBuffer(new_data, buffer.info->data, size);
					EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + size, info->data, info->size);
					TypeTrait::FillEmptyValue(new_data + new_size, new_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = new_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}
	EspBuffer& Prepend(const EspBuffer& buffer, unsigned int size, unsigned int count)
	{
		if (size == 0 || buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
			return *this;
		if (size > buffer.info->size)
			size = buffer.info->size;

		unsigned int add_size = size * count;
		if (info == nullptr)
		{
			::EspAllocBuffer<EspBufferInfo<T>>(info);
			new(info)EspBufferInfo<T>(add_size);

			TypeTrait::CopyBufferN(info->data, buffer.info->data, size, count);
			TypeTrait::FillEmptyValue(info->data + add_size, add_size);
		}
		else if (info->data == nullptr)
		{
			unsigned int new_capacity = size * 2;
			::EspAllocBuffer<T>(info->data, new_capacity);

			TypeTrait::CopyBufferN(info->data, buffer.info->data, size, count);
			TypeTrait::FillEmptyValue(info->data + add_size, add_size);

			info->size = add_size;
			info->capacity = new_capacity;
		}
		else {
			unsigned int new_size = info->size + add_size;
			if (info->IsShared())
			{
				EspBufferInfo<T>* new_info;
				EspAllocBuffer<EspBufferInfo<T>>(new_info);
				new(new_info)EspBufferInfo<T>(new_size);

				TypeTrait::CopyBufferN(new_info->data, buffer.info->data, size, count);
				TypeTrait::CopyBuffer(new_info->data + add_size, info->data, info->size);
				TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

				info->ref.Subtract();
				info = new_info;
			}
			else
			{
				if (new_size < info->capacity)
				{
					EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + add_size, info->data, info->size);
					TypeTrait::CopyBufferN(info->data, buffer.info->data, size, count);

					info->size = new_size;
				}
				else
				{
					unsigned int new_capacity = new_size * 2;
					T* new_data;
					EspAllocBuffer<T>(new_data, new_capacity);

					TypeTrait::CopyBufferN(new_data, buffer.info->data, size, count);
					EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + add_size, info->data, info->size);
					TypeTrait::FillEmptyValue(new_data + new_size, new_size);

					EspDeAllocBuffer<T>(info->data);
					info->size = new_size;
					info->capacity = new_capacity;
					info->data = new_data;
				}
			}
		}
		return *this;
	}

	EspBuffer& Insert(unsigned int index, const T& value)
	{
		if (info == nullptr || info->data == nullptr)
			return *this;
		if (index == 0)
			return Prepend(value);
		else if (index == info->size)
			return Append(value);

		unsigned int new_size = info->size + 1;
		if (info->IsShared())
		{
			EspBufferInfo<T>* new_info;
			EspAllocBuffer<EspBufferInfo<T>>(new_info);
			new(new_info)EspBufferInfo<T>(new_size);

			TypeTrait::CopyBuffer(new_info->data, info->data, index);
			TypeTrait::CopyValue(new_info->data + index, value);
			TypeTrait::CopyBuffer(new_info->data + index + 1, info->data + index, info->size - index);
			TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

			info->ref.Subtract();
			info = new_info;
		}
		else
		{
			if (new_size < info->capacity)
			{
				EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + index + 1, info->data + index, info->size - index);
				TypeTrait::CopyValue(info->data + index, value);

				info->size = new_size;
			}
			else
			{
				unsigned int new_capacity = new_size * 2;
				T* new_data;
				EspAllocBuffer<T>(new_data, new_capacity);

				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, index);
				TypeTrait::CopyValue(new_data + index, value);
				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + index + 1, info->data + index, info->size - index);
				TypeTrait::FillEmptyValue(new_data + new_size, new_size);

				EspDeAllocBuffer<T>(info->data);
				info->size = new_size;
				info->capacity = new_capacity;
				info->data = new_data;
			}
		}
		return *this;
	}
	EspBuffer& Insert(unsigned int index, const T& value, unsigned int count)
	{
		if (count == 0 || info == nullptr || info->data == nullptr)
			return *this;
		if (index == 0)
			return Prepend(value, count);
		else if (index == info->size)
			return Append(value, count);

		unsigned int new_size = info->size + count;
		if (info->IsShared())
		{
			EspBufferInfo<T>* new_info;
			EspAllocBuffer<EspBufferInfo<T>>(new_info);
			new(new_info)EspBufferInfo<T>(new_size);

			TypeTrait::CopyBuffer(new_info->data, info->data, index);
			TypeTrait::CopyValueN(new_info->data + index, value, count);
			TypeTrait::CopyBuffer(new_info->data + index + count, info->data + index, info->size - index);
			TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

			info->ref.Subtract();
			info = new_info;
		}
		else
		{
			if (new_size < info->capacity)
			{
				EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + index + count, info->data + index, info->size - index);
				TypeTrait::CopyValueN(info->data + index, value, count);

				info->size = new_size;
			}
			else
			{
				unsigned int new_capacity = new_size * 2;
				T* new_data;
				EspAllocBuffer<T>(new_data, new_capacity);

				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, index);
				TypeTrait::CopyValueN(new_data + index, value, count);
				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + index + count, info->data + index, info->size - index);
				TypeTrait::FillEmptyValue(new_data + new_size, new_size);

				EspDeAllocBuffer<T>(info->data);
				info->size = new_size;
				info->capacity = new_capacity;
				info->data = new_data;
			}
		}
		return *this;
	}
	EspBuffer& Insert(unsigned int index, const T* data, unsigned int size)
	{
		if (data == nullptr || size == 0 || info == nullptr || info->data == nullptr)
			return *this;
		if (index == 0)
			return Prepend(data, size);
		else if (index == info->size)
			return Append(data, size);

		unsigned int new_size = info->size + size;
		if (info->IsShared())
		{
			EspBufferInfo<T>* new_info;
			EspAllocBuffer<EspBufferInfo<T>>(new_info);
			new(new_info)EspBufferInfo<T>(new_size);

			TypeTrait::CopyBuffer(new_info->data, info->data, index);
			TypeTrait::CopyBuffer(new_info->data + index, data, size);
			TypeTrait::CopyBuffer(new_info->data + index + size, info->data + index, info->size - index);
			TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

			info->ref.Subtract();
			info = new_info;
		}
		else
		{
			if (new_size < info->capacity)
			{
				EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + index + size, info->data + index, info->size - index);
				TypeTrait::CopyBuffer(info->data + index, data, size);

				info->size = new_size;
			}
			else
			{
				unsigned int new_capacity = new_size * 2;
				T* new_data;
				EspAllocBuffer<T>(new_data, new_capacity);

				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, index);
				TypeTrait::CopyBuffer(new_data + index, data, size);
				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + index + size, info->data + index, info->size - index);
				TypeTrait::FillEmptyValue(new_data + new_size, new_size);

				EspDeAllocBuffer<T>(info->data);
				info->size = new_size;
				info->capacity = new_capacity;
				info->data = new_data;
			}
		}
		return *this;
	}
	EspBuffer& Insert(unsigned int index, const T* data, unsigned int size, unsigned int count)
	{
		if (data == nullptr || size == 0 || count == 0 || info == nullptr || info->data == nullptr)
			return *this;
		if (index == 0)
			return Prepend(data, size);
		else if (index == info->size)
			return Append(data, size);

		unsigned int add_size = size * count;
		unsigned int new_size = info->size + add_size;
		if (info->IsShared())
		{
			EspBufferInfo<T>* new_info;
			EspAllocBuffer<EspBufferInfo<T>>(new_info);
			new(new_info)EspBufferInfo<T>(new_size);

			TypeTrait::CopyBuffer(new_info->data, info->data, index);
			TypeTrait::CopyBufferN(new_info->data + index, data, size, count);
			TypeTrait::CopyBuffer(new_info->data + index + add_size, info->data + index, info->size - index);
			TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

			info->ref.Subtract();
			info = new_info;
		}
		else
		{
			if (new_size < info->capacity)
			{
				EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + index + add_size, info->data + index, info->size - index);
				TypeTrait::CopyBufferN(info->data + index, data, size, count);

				info->size = new_size;
			}
			else
			{
				unsigned int new_capacity = new_size * 2;
				T* new_data;
				EspAllocBuffer<T>(new_data, new_capacity);

				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, index);
				TypeTrait::CopyBufferN(new_data + index, data, size, count);
				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + index + add_size, info->data + index, info->size - index);
				TypeTrait::FillEmptyValue(new_data + new_size, new_size);

				EspDeAllocBuffer<T>(info->data);
				info->size = new_size;
				info->capacity = new_capacity;
				info->data = new_data;
			}
		}
		return *this;
	}
	EspBuffer& Insert(unsigned int index, const T* data)
	{
		return Insert(index, data, TypeTrait::GetSize(data));
	}
	EspBuffer& Insert(unsigned int index, const EspBuffer& buffer)
	{
		if (info == nullptr || info->data == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
			return *this;
		if (index == 0)
			Prepend(buffer);
		if (index == info->size)
			Append(buffer);

		unsigned int new_size = info->size + buffer.info->size;
		if (info->IsShared())
		{
			EspBufferInfo<T>* new_info;
			EspAllocBuffer<EspBufferInfo<T>>(new_info);
			new(new_info)EspBufferInfo<T>(new_size);

			TypeTrait::CopyBuffer(new_info->data, info->data, index);
			TypeTrait::CopyBuffer(new_info->data + index, buffer.info->data, buffer.info->size);
			TypeTrait::CopyBuffer(new_info->data + index + buffer.info->size, info->data + index, info->size - index);
			TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

			info->ref.Subtract();
			info = new_info;
		}
		else
		{
			if (new_size < info->capacity)
			{
				EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + index + buffer.info->size, info->data + index, info->size - index);
				TypeTrait::CopyBuffer(info->data + index, buffer.info->data, buffer.info->size);

				info->size = new_size;
			}
			else
			{
				unsigned int new_capacity = new_size * 2;
				T* new_data;
				EspAllocBuffer<T>(new_data, new_capacity);

				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, index);
				TypeTrait::CopyBuffer(new_data + index, buffer.info->data, buffer.info->size);
				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + index + buffer.info->size, info->data + index, info->size - index);
				TypeTrait::FillEmptyValue(new_data + new_size, new_size);

				EspDeAllocBuffer<T>(info->data);
				info->size = new_size;
				info->capacity = new_capacity;
				info->data = new_data;
			}
		}
		return *this;
	}
	EspBuffer& Insert(unsigned int index, const EspBuffer& buffer, unsigned int size)
	{
		if (size == 0 || info == nullptr || info->data == nullptr || buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
			return *this;
		if (index == 0)
			Prepend(buffer, size);
		if (index == info->size)
			Append(buffer, size);
		if (size > buffer.info->size)
			size = buffer.info->size;

		unsigned int new_size = info->size + size;
		if (info->IsShared())
		{
			EspBufferInfo<T>* new_info;
			EspAllocBuffer<EspBufferInfo<T>>(new_info);
			new(new_info)EspBufferInfo<T>(new_size);

			TypeTrait::CopyBuffer(new_info->data, info->data, index);
			TypeTrait::CopyBuffer(new_info->data + index, buffer.info->data, size);
			TypeTrait::CopyBuffer(new_info->data + index + size, info->data + index, info->size - index);
			TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

			info->ref.Subtract();
			info = new_info;
		}
		else
		{
			if (new_size < info->capacity)
			{
				EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + index + size, info->data + index, info->size - index);
				TypeTrait::CopyBuffer(info->data + index, buffer.info->data, size);

				info->size = new_size;
			}
			else
			{
				unsigned int new_capacity = new_size * 2;
				T* new_data;
				EspAllocBuffer<T>(new_data, new_capacity);

				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, index);
				TypeTrait::CopyBuffer(new_data + index, buffer.info->data, size);
				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + index + size, info->data + index, info->size - index);
				TypeTrait::FillEmptyValue(new_data + new_size, new_size);

				EspDeAllocBuffer<T>(info->data);
				info->size = new_size;
				info->capacity = new_capacity;
				info->data = new_data;
			}
		}
		return *this;
	}
	EspBuffer& Insert(unsigned int index, const EspBuffer& buffer, unsigned int size, unsigned int count)
	{
		if (size == 0 || info == nullptr || info->data == nullptr || buffer.info == nullptr || buffer.info->data == nullptr || buffer.info->capacity == 0 || buffer.info->size == 0)
			return *this;
		if (index == 0)
			Prepend(buffer, size, count);
		if (index == info->size)
			Append(buffer, size, count);
		if (size > buffer.info->size)
			size = buffer.info->size;

		unsigned int add_size = size * count;
		unsigned int new_size = info->size + add_size;
		if (info->IsShared())
		{
			EspBufferInfo<T>* new_info;
			EspAllocBuffer<EspBufferInfo<T>>(new_info);
			new(new_info)EspBufferInfo<T>(new_size);

			TypeTrait::CopyBuffer(new_info->data, info->data, index);
			TypeTrait::CopyBufferN(new_info->data + index, buffer.info->data, size, count);
			TypeTrait::CopyBuffer(new_info->data + index + add_size, info->data + index, info->size - index);
			TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

			info->ref.Subtract();
			info = new_info;
		}
		else
		{
			if (new_size < info->capacity)
			{
				EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + index + add_size, info->data + index, info->size - index);
				TypeTrait::CopyBufferN(info->data + index, buffer.info->data, size, count);

				info->size = new_size;
			}
			else
			{
				unsigned int new_capacity = new_size * 2;
				T* new_data;
				EspAllocBuffer<T>(new_data, new_capacity);

				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data, info->data, index);
				TypeTrait::CopyBufferN(new_data + index, buffer.info->data, size, count);
				EspDefaultSimpleTypeTrait<T>::CopyBuffer(new_data + index + add_size, info->data + index, info->size - index);
				TypeTrait::FillEmptyValue(new_data + new_size, new_size);

				EspDeAllocBuffer<T>(info->data);
				info->size = new_size;
				info->capacity = new_capacity;
				info->data = new_data;
			}
		}
		return *this;
	}

	EspBuffer& Delete(unsigned int index, unsigned int count)
	{
		if (info == nullptr || info->data == nullptr || count == 0)
			return *this;

		unsigned int new_size = info->size - count;
		if (info->IsShared())
		{
			EspBufferInfo<T>* new_info(nullptr);
			EspAllocBuffer<EspBufferInfo<T>>(new_info);
			new(new_info)EspBufferInfo<T>(new_size);

			TypeTrait::CopyBuffer(new_info->data, info->data, index);
			TypeTrait::CopyBuffer(new_info->data + index, info->data + index + count, new_size - index);
			TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

			info->ref.Subtract();
			info = new_info;
		}
		else
		{
			EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data + index, info->data + index + count, new_size - index);
			TypeTrait::FillEmptyValue(info->data + new_size, count);

			info->size = new_size;
		}
		return *this;
	}
	EspBuffer& Delete(unsigned int index)
	{
		Delete(index, 1);
	}

	EspBuffer& TrimLeft(unsigned int count)
	{
		if (info == nullptr || info->data == nullptr || count == 0)
			return *this;

		unsigned int new_size = info->size - count;
		if (info->IsShared())
		{
			EspBufferInfo<T>* new_info(nullptr);
			EspAllocBuffer<EspBufferInfo<T>>(new_info);
			new(new_info)EspBufferInfo<T>(new_size);

			TypeTrait::CopyBuffer(new_info->data, info->data + count, new_size);
			TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

			info->ref.Subtract();
			info = new_info;
		}
		else
		{
			EspDefaultSimpleTypeTrait<T>::MoveBuffer(info->data, info->data + count, new_size);
			TypeTrait::FillEmptyValue(info->data + new_size, count);

			info->size = new_size;
		}
		return *this;
	}
	EspBuffer& TrimRight(unsigned int count)
	{
		if (info == nullptr || info->data == nullptr || count == 0)
			return *this;

		unsigned int new_size = info->size - count;
		if (info->IsShared())
		{
			EspBufferInfo<T>* new_info(nullptr);
			EspAllocBuffer<EspBufferInfo<T>>(new_info);
			new(new_info)EspBufferInfo<T>(new_size);

			TypeTrait::CopyBuffer(new_info->data, info->data, new_size);
			TypeTrait::FillEmptyValue(new_info->data + new_size, new_size);

			info->ref.Subtract();
			info = new_info;
		}
		else
		{
			TypeTrait::FillEmptyValue(info->data + new_size, count);

			info->size = new_size;
		}
		return *this;
	}

	EspBuffer GetLeft(unsigned int count)
	{
		if (count > info->size)
			return EspBuffer(*this);
		return EspBuffer(info->data, count);
	}
	EspBuffer GetRight(unsigned int count)
	{
		if (count > info->size)
			return EspBuffer(*this);
		return EspBuffer(info->data, info->size - count);
	}
	EspBuffer GetMiddle(unsigned int index, unsigned int count)
	{
		if ((index > info->size) || (count > info->size - index))
			return EspBuffer(*this);
		return EspBuffer(info->data + index, count);
	}
};

template<typename T, typename TypeTrait>
class EspBufferPiece
{
private:
	const T* data;
	unsigned int size;

public:
	EspBufferPiece() :data(nullptr), size(0) {}
	EspBufferPiece(T* new_data, unsigned int new_size) :data(new_data), size(new_size) {}
	EspBufferPiece(EspBuffer<T, TypeTrait> new_data) :data(new_data.GetConstData()), size(new_data.GetSize()) {}
	EspBufferPiece(EspBuffer<T, TypeTrait> new_data, unsigned int new_size) :data(new_data.GetConstData()), size(new_size) {}
	//EspBufferPiece(std::string new_data) :data(new_data.c_str()), size(new_data.size()) {}
	//EspBufferPiece(std::string new_data, unsigned int new_size) :data(new_data.c_str()), size(new_size) {}
	//EspBufferPiece(std::vector<T> new_data) :data(new_data.data()), size(new_data.size()) {}
	//EspBufferPiece(std::vector<T> new_data, unsigned int new_size) :data(new_data.data()), size(new_size) {}

	const T* GetData()const { return data; }
	unsigned int GetSize()const { return size; }
	unsigned int GetCount()const { return size; }
	unsigned int GetLength()const { return size; }
};
