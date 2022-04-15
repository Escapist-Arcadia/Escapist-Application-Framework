#pragma once

#include<cassert>
#include"../Buffer/EspBuffer.h"
#include"../Buffer/EspString.h"

#ifdef GetObject
#pragma push_macro("GetObject")
#define HAS_UNDEFINED_GET_OBJECT_API
#undef GetObject
#endif


enum class EspJsonValueType
{
	// The value is [null]
	Null,

	// The value is [true] or [false]
	Boolean,

	// The value is a number and can be assigned to a [int] type
	Integer,

	// The value is a number and can be assigned to a [long long] type but are too long to be assigned to a [int] type
	LongInteger,

	// The value is a number but has decimal part, and store in [double] type
	FloatingPoint,

	// The value is a string, and are encircled by quote["]
	String,

	// an json object {}
	Object,

	// an json array []
	Array
};

class EspJsonValue;
class EspJsonMember;
class EspJsonObject;
class EspJsonArray;


class EspJsonValue
{
private:
	using Type = EspJsonValueType;

	EspJsonValueType type;
	union
	{
		bool boolean;
		int integer;
		long long int long_integer;
		struct { double value; unsigned int decimal_count; }floating_point;
		EspString* string;
		EspJsonObject* obj;
		EspJsonArray* arr;
		struct { EspJsonValue* data; unsigned int size; unsigned int capacity; }jarray;
		struct { EspJsonMember* data; unsigned int size; unsigned int capacity; }jobject;
	}value;

public:
	inline EspJsonValue() :type(Type::Null) {}
	inline EspJsonValue(const EspJsonValueType& new_type) : type(new_type) {}
	inline EspJsonValue(const bool& new_value) : type(Type::Boolean) { value.boolean = new_value; }
	inline EspJsonValue(const int& new_value) : type(Type::Integer) { value.integer = new_value; }
	inline EspJsonValue(const long long int& new_value) : type(Type::LongInteger) { value.long_integer = new_value; }
	inline EspJsonValue(const double& new_value) : type(Type::FloatingPoint)
	{
		new(this)EspJsonValue(new_value, 0);
	}
	inline EspJsonValue(const double& new_value, unsigned int decimal_count) : type(Type::FloatingPoint)
	{
		value.floating_point.value = new_value;
		value.floating_point.decimal_count = decimal_count;
	}
	inline EspJsonValue(const TCHAR* new_value) : type(Type::String)
	{
		::EspAllocBuffer<EspString>(value.string);
		new(value.string)EspString(new_value);
	}
	inline EspJsonValue(const TCHAR* new_value, const unsigned int size) : type(Type::String)
	{
		::EspAllocBuffer<EspString>(value.string);
		new(value.string)EspString(new_value, size);
	}
	inline EspJsonValue(const EspString& new_value) : type(Type::String)
	{
		::EspAllocBuffer<EspString>(value.string);
		new(value.string)EspString(new_value);
	}
	inline EspJsonValue(const EspString& new_value, const unsigned int size) : type(Type::String)
	{
		::EspAllocBuffer<EspString>(value.string);
		new(value.string)EspString(new_value, size);
	}
	inline EspJsonValue(EspString* new_value) : type(Type::String) { value.string = new_value; }
	inline EspJsonValue(const EspJsonObject& new_value);
	inline EspJsonValue(EspJsonObject* new_value);
	inline EspJsonValue(const EspJsonArray& new_value);
	inline EspJsonValue(EspJsonArray* new_value);
	EspJsonValue(const EspJsonValue& new_value);
	void FreeValue();
	inline ~EspJsonValue()
	{
		FreeValue();
	}

	inline EspJsonValueType GetValueType()const { return type; }
	inline bool IsNull()const { return type == Type::Null; }
	inline bool IsBoolean()const { return type == Type::Boolean; }
	inline bool IsInteger()const { return type == Type::Integer; }
	inline bool IsLongInteger()const { return type == Type::LongInteger; }
	inline bool IsInt()const { return IsInteger() || IsLongInteger(); }
	inline bool IsFloatingPoint()const { return type == Type::LongInteger; }
	inline bool IsNumber()const { return IsInt() || IsFloatingPoint(); }
	inline bool IsString()const { return type == Type::String; }
	inline bool IsObject()const { return type == Type::Object; }
	inline bool IsArray()const { return type == Type::Array; }

	inline bool GetBoolean() { assert(IsBoolean()); return value.boolean; }
	inline const bool& GetConstBoolean()const { assert(IsBoolean()); return value.boolean; }
	inline int GetInteger() { assert(IsInteger()); return value.integer; }
	inline const int& GetConstInteger()const { assert(IsInteger()); return value.integer; }
	inline long long int GetLongInteger()
	{
		if (IsLongInteger())
			return value.long_integer;
		else if (IsInteger())
			return (long long int)value.integer;
		else
			assert(false);
	}
	inline const long long int& GetConstLongInteger()const
	{
		if (IsLongInteger())
			return value.long_integer;
		else if (IsInteger())
			return (long long int)value.integer;
		else
			assert(false);
	}
	inline double GetFloatingPoint()
	{
		if (IsFloatingPoint())
			return value.floating_point.value;
		else if (IsInteger())
			return (double)value.integer;
		else if (IsLongInteger())
			return (double)value.long_integer;
		else
			assert(false);
	}
	inline const double& GetConstFloatingPoint()const
	{
		if (IsFloatingPoint())
			return value.floating_point.value;
		else if (IsInteger())
			return (double)value.integer;
		else if (IsLongInteger())
			return (double)value.long_integer;
		else
			assert(false);
	}
	inline EspString GetString() { assert(IsString()); return *value.string; }
	inline const EspString& GetConstString()const { assert(IsString()); return *value.string; }
	inline EspString* GetStringPtr() { assert(IsString()); return value.string; }
	inline EspJsonObject GetObject();
	inline const EspJsonObject& GetConstObject()const;
	inline EspJsonObject* GetObjectPtr()const;
	inline EspJsonArray GetArray();
	inline const EspJsonArray& GetConstArray()const;
	inline EspJsonArray* GetArrayPtr()const;

	inline void SetNull()
	{
		if (!IsNull())
		{
			FreeValue();
			type = Type::Null;
		}
	}
	inline void SetBoolean(const bool& new_value)
	{
		if (!IsBoolean())
		{
			FreeValue();
			type = Type::Boolean;
		}
		value.boolean = new_value;
	}
	inline void SetInteger(const int& new_value)
	{
		if (!IsInteger())
		{
			FreeValue();
			type = Type::Integer;
		}
		value.integer = new_value;
	}
	inline void SetLongInteger(const long long int& new_value)
	{
		if (!IsLongInteger())
		{
			FreeValue();
			type = Type::LongInteger;
		}
		value.long_integer = new_value;
	}
	inline void SetFloatingPoint(const double& new_value, const unsigned int decimal_count)
	{
		if (!IsFloatingPoint())
		{
			FreeValue();
			type = Type::FloatingPoint;
		}
		value.floating_point.value = new_value;
		value.floating_point.decimal_count = decimal_count;
	}
	inline void SetFloatingPoint(const double& new_value)
	{
		SetFloatingPoint(new_value, 0);
	}
	inline void SetString(const EspString& new_value)
	{
		if (!IsString())
		{
			FreeValue();
			type = Type::String;
		}
		EspAllocBuffer<EspString>(value.string);
		new(value.string)EspString(new_value);
	}
	inline void SetString(const EspString& new_value, const unsigned int size)
	{
		if (!IsString())
		{
			FreeValue();
			type = Type::String;
		}
		EspAllocBuffer<EspString>(value.string);
		new(value.string)EspString(new_value, size);
	}
	inline void SetStringPtr(EspString* new_value)
	{
		if (!IsString())
		{
			FreeValue();
			type = Type::String;
		}
		value.string = new_value;
	}
	inline void SetArray(const EspJsonArray& new_value);
	inline void SetArrayPtr(EspJsonArray* new_value);
	inline void SetObject(const EspJsonObject& new_value);
	inline void SetObjectPtr(EspJsonObject* new_value);
};
class EspJsonMember
{
private:
	EspString key;
	EspJsonValue value;

public:
	inline EspJsonMember() {}
	inline EspJsonMember(const EspJsonValueType& type) :value(type) {}
	inline EspJsonMember(const EspString& new_key, const EspJsonValue& new_value) : key(new_key), value(new_value) {}
	inline EspJsonMember(const EspString& new_key) : key(new_key), value(EspJsonValueType::Null) {}
	inline EspJsonMember(const EspString& new_key, const bool& new_value) : key(new_key), value(new_value) {}
	inline EspJsonMember(const EspString& new_key, const int& new_value) : key(new_key), value(new_value) {}
	inline EspJsonMember(const EspString& new_key, const long long int& new_value) : key(new_key), value(new_value) {}
	inline EspJsonMember(const EspString& new_key, const double& new_value) : key(new_key), value(new_value) {}
	inline EspJsonMember(const EspString& new_key, const EspString& new_value) : key(new_key), value(new_value) {}
	inline EspJsonMember(const EspString& new_key, const EspString& new_value, const unsigned int size) : key(new_key), value(new_value, size) {}
	inline EspJsonMember(const EspString& new_key, EspString* new_value) : key(new_key), value(new_value) {}
	inline EspJsonMember(const EspString& new_key, const EspJsonObject& new_value) : key(new_key), value(new_value) {}
	inline EspJsonMember(const EspString& new_key, EspJsonObject* new_value) : key(new_key), value(new_value) {}
	inline EspJsonMember(const EspString& new_key, const EspJsonArray& new_value) : key(new_key), value(new_value) {}
	inline EspJsonMember(const EspString& new_key, EspJsonArray* new_value) : key(new_key), value(new_value) {}

	inline EspString GetKey() { return key; }
	inline EspString& GetKeyRef() { return key; }
	inline const EspString& GetConstKey()const { return key; }
	inline void SetKey(const EspString& new_key) { key.Assign(new_key); }

	inline EspJsonValue GetValue() { return value; }
	inline EspJsonValue& GetValueRef() { return value; }
	inline const EspJsonValue& GetConstValue()const { return value; }
	inline void SetValue(const EspJsonValue& new_value)
	{
		switch (new_value.GetValueType())
		{
		case EspJsonValueType::Null:value.SetNull(); break;
		case EspJsonValueType::Boolean:value.SetBoolean(new_value.GetConstBoolean()); break;
		case EspJsonValueType::Integer:value.SetInteger(new_value.GetConstInteger()); break;
		case EspJsonValueType::LongInteger:value.SetLongInteger(new_value.GetConstLongInteger()); break;
		case EspJsonValueType::FloatingPoint:value.SetFloatingPoint(new_value.GetConstFloatingPoint()); break;
		case EspJsonValueType::String:value.SetString(new_value.GetConstString()); break;
		case EspJsonValueType::Object:value.SetObject(new_value.GetConstObject()); break;
		case EspJsonValueType::Array:value.SetArray(new_value.GetConstArray()); break;
		default:
			break;
		}

	}
};

class EspJsonObject
{
public:
	using EspJsonObjectData = EspBuffer<EspJsonMember>;
	EspJsonObjectData data;

public:
	EspJsonObject() {}
	EspJsonObject(const EspJsonObjectData& new_data) :data(new_data) {}
	EspJsonObject(const EspJsonObject& new_object) :data(new_object.data) {}

	inline unsigned int GetCount()const { return data.GetCount(); }
	inline EspJsonMember& GetAt(unsigned int index) { return data.GetAt(index); }
	inline const EspJsonMember& GetConstAt(unsigned int index)const { return data.GetConstAt(index); }
	inline unsigned int IndexOfKey(const EspString& key)const
	{
		for (unsigned int index = 0; index < data.GetCount(); ++index)
			if (data.GetConstAt(index).GetConstKey().Compare(key))
				return index;
		return -1;
	}

	inline void AppendMember(const EspJsonMember& member) { data.Append(member); }
	inline void AppendMember(const EspString& new_key, const EspJsonValue& new_value)
	{
		data.Append(new_value.GetValueType());
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(new_key);
		ref.SetValue(new_value);
	}
	inline void AppendNull(const EspString& key)
	{
		data.Append(EspJsonValueType::Null);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValue().SetNull();
	}
	inline void AppendBoolean(const EspString& key, const bool& value)
	{
		data.Append(EspJsonValueType::Boolean);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetBoolean(value);
	}
	inline void AppendInteger(const EspString& key, const int& value)
	{
		data.Append(EspJsonValueType::Integer);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetInteger(value);
	}
	inline void AppendLongInteger(const EspString& key, const long long& value)
	{
		data.Append(EspJsonValueType::LongInteger);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetLongInteger(value);
	}
	inline void AppendFloatingPoint(const EspString& key, const double& value)
	{
		data.Append(EspJsonValueType::LongInteger);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetFloatingPoint(value);
	}
	inline void AppendString(const EspString& key, const TCHAR* value)
	{
		data.Append(EspJsonValueType::String);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetString(value);
	}
	inline void AppendString(const EspString& key, const EspString& value)
	{
		data.Append(EspJsonValueType::String);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetString(value);
	}
	inline void AppendStringPtr(const EspString& key, const EspString* value)
	{
		data.Append(EspJsonValueType::String);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetStringPtr((EspString*)value);
	}
	inline void AppendObject(const EspString& key, const EspJsonObject& value)
	{
		data.Append(EspJsonValueType::Object);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetObject(value);
	}
	inline void AppendObjectPtr(const EspString& key, const EspJsonObject* value)
	{
		data.Append(EspJsonValueType::Object);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetObjectPtr((EspJsonObject*)value);
	}
	inline void AppendArray(const EspString& key, const EspJsonArray& value)
	{
		data.Append(EspJsonValueType::Array);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetArray(value);
	}
	inline void AppendArrayPtr(const EspString& key, const EspJsonArray* value)
	{
		data.Append(EspJsonValueType::Array);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetArrayPtr((EspJsonArray*)value);
	}

	inline void PrependMember(const EspJsonMember& member) { data.Prepend(member); }
	inline void PrependMember(const EspString& new_key, const EspJsonValue& new_value)
	{
		data.Prepend(new_value.GetValueType());
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(new_key);
		ref.SetValue(new_value);
	}
	inline void PrependNull(const EspString& key)
	{
		data.Prepend(EspJsonValueType::Null);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValue().SetNull();
	}
	inline void PrependBoolean(const EspString& key, const bool& value)
	{
		data.Prepend(EspJsonValueType::Boolean);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetBoolean(value);
	}
	inline void PrependInteger(const EspString& key, const int& value)
	{
		data.Prepend(EspJsonValueType::Integer);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetInteger(value);
	}
	inline void PrependLongInteger(const EspString& key, const long long& value)
	{
		data.Prepend(EspJsonValueType::LongInteger);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetLongInteger(value);
	}
	inline void PrependFloatingPoint(const EspString& key, const double& value)
	{
		data.Prepend(EspJsonValueType::LongInteger);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetFloatingPoint(value);
	}
	inline void PrependString(const EspString& key, const TCHAR* value)
	{
		data.Prepend(EspJsonValueType::String);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetString(value);
	}
	inline void PrependString(const EspString& key, const EspString& value)
	{
		data.Prepend(EspJsonValueType::String);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetString(value);
	}
	inline void PrependStringPtr(const EspString& key, const EspString* value)
	{
		data.Prepend(EspJsonValueType::String);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetStringPtr((EspString*)value);
	}
	inline void PrependObject(const EspString& key, const EspJsonObject& value)
	{
		data.Prepend(EspJsonValueType::Object);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetObject(value);
	}
	inline void PrependObjectPtr(const EspString& key, const EspJsonObject* value)
	{
		data.Prepend(EspJsonValueType::Object);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetObjectPtr((EspJsonObject*)value);
	}
	inline void PrependArray(const EspString& key, const EspJsonArray& value)
	{
		data.Prepend(EspJsonValueType::Array);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetArray(value);
	}
	inline void PrependArrayPtr(const EspString& key, const EspJsonArray* value)
	{
		data.Prepend(EspJsonValueType::Array);
		EspJsonMember& ref = data.GetAt(data.GetCount() - 1);
		ref.SetKey(key);
		ref.GetValueRef().SetArrayPtr((EspJsonArray*)value);
	}

	inline void InsertMember(const unsigned int index, const EspJsonMember& member) { data.Insert(index, member); }
	inline void InsertMember(const unsigned int index, const EspString& new_key, const EspJsonValue& new_value)
	{
		data.Insert(index, new_value.GetValueType());
		EspJsonMember& ref = data.GetAt(index);
		ref.SetKey(new_key);
		ref.SetValue(new_value);
	}
	inline void InsertNull(const unsigned int index, const EspString& key)
	{
		data.Insert(index, EspJsonValueType::Null);
		EspJsonMember& ref = data.GetAt(index);
		ref.SetKey(key);
		ref.GetValue().SetNull();
	}
	inline void InsertBoolean(const unsigned int index, const EspString& key, const bool& value)
	{
		data.Insert(index, EspJsonValueType::Boolean);
		EspJsonMember& ref = data.GetAt(index);
		ref.SetKey(key);
		ref.GetValueRef().SetBoolean(value);
	}
	inline void InsertInteger(const unsigned int index, const EspString& key, const int& value)
	{
		data.Insert(index, EspJsonValueType::Integer);
		EspJsonMember& ref = data.GetAt(index);
		ref.SetKey(key);
		ref.GetValueRef().SetInteger(value);
	}
	inline void InsertLongInteger(const unsigned int index, const EspString& key, const long long& value)
	{
		data.Insert(index, EspJsonValueType::LongInteger);
		EspJsonMember& ref = data.GetAt(index);
		ref.SetKey(key);
		ref.GetValueRef().SetLongInteger(value);
	}
	inline void InsertFloatingPoint(const unsigned int index, const EspString& key, const double& value)
	{
		data.Insert(index, EspJsonValueType::LongInteger);
		EspJsonMember& ref = data.GetAt(index);
		ref.SetKey(key);
		ref.GetValueRef().SetFloatingPoint(value);
	}
	inline void InsertString(const unsigned int index, const EspString& key, const TCHAR* value)
	{
		data.Insert(index, EspJsonValueType::String);
		EspJsonMember& ref = data.GetAt(index);
		ref.SetKey(key);
		ref.GetValueRef().SetString(value);
	}
	inline void InsertString(const unsigned int index, const EspString& key, const EspString& value)
	{
		data.Insert(index, EspJsonValueType::String);
		EspJsonMember& ref = data.GetAt(index);
		ref.SetKey(key);
		ref.GetValueRef().SetString(value);
	}
	inline void InsertStringPtr(const unsigned int index, const EspString& key, const EspString* value)
	{
		data.Insert(index, EspJsonValueType::String);
		EspJsonMember& ref = data.GetAt(index);
		ref.SetKey(key);
		ref.GetValueRef().SetStringPtr((EspString*)value);
	}
	inline void InsertObject(const unsigned int index, const EspString& key, const EspJsonObject& value)
	{
		data.Insert(index, EspJsonValueType::Object);
		EspJsonMember& ref = data.GetAt(index);
		ref.SetKey(key);
		ref.GetValueRef().SetObject(value);
	}
	inline void InsertObjectPtr(const unsigned int index, const EspString& key, const EspJsonObject* value)
	{
		data.Insert(index, EspJsonValueType::Object);
		EspJsonMember& ref = data.GetAt(index);
		ref.SetKey(key);
		ref.GetValueRef().SetObjectPtr((EspJsonObject*)value);
	}
	inline void InsertArray(const unsigned int index, const EspString& key, const EspJsonArray& value)
	{
		data.Insert(index, EspJsonValueType::Array);
		EspJsonMember& ref = data.GetAt(index);
		ref.SetKey(key);
		ref.GetValueRef().SetArray(value);
	}
	inline void InsertArrayPtr(const unsigned int index, const EspString& key, const EspJsonArray* value)
	{
		data.Insert(index, EspJsonValueType::Array);
		EspJsonMember& ref = data.GetAt(index);
		ref.SetKey(key);
		ref.GetValueRef().SetArrayPtr((EspJsonArray*)value);
	}

	inline void DeleteMember(unsigned int index) { data.Delete(index); }
	inline void DeleteMember(unsigned int index, unsigned int count) { data.Delete(index, count); }

	EspString Synthesize()const;
};
class EspJsonArray
{
public:
	using EspJsonArrayData = EspBuffer<EspJsonValue>;
	EspJsonArrayData data;

public:
	EspJsonArray() {}
	EspJsonArray(const EspJsonArrayData& new_data) :data(new_data) {}
	EspJsonArray(const EspJsonArray& new_array) :data(new_array.data) {}

	inline unsigned int GetCount()const { return data.GetCount(); }
	inline EspJsonValue& GetAt(unsigned int index) { return data.GetAt(index); }
	inline const EspJsonValue& GetConstAt(unsigned int index)const { return data.GetConstAt(index); }

	inline void AppendValue(const EspJsonValue& value) { data.Append(value); }
	inline void AppendBoolean(const bool& value) { data.Append(value); }
	inline void AppendInteger(const int& value) { data.Append(value); }
	inline void AppendLongInteger(const long long& value) { data.Append(value); }
	inline void AppendFloatingPoint(const double& value) { data.Append(value); }
	inline void AppendString(const TCHAR* value) { data.Append(value); }
	inline void AppendString(const EspString& value) { data.Append(value); }
	inline void AppendStringPtr(const EspString* value) { data.Append(value); }
	inline void AppendJsonObject(const EspJsonObject& value) { data.Append(value); }
	inline void AppendJsonObjectPtr(const EspJsonObject* value) { data.Append(value); }
	inline void AppendJsonArray(const EspJsonArray& value) { data.Append(value); }
	inline void AppendJsonArrayPtr(const EspJsonArray* value) { data.Append(value); }

	inline void PrependValue(const EspJsonValue& value) { data.Prepend(value); }
	inline void PrependBoolean(const bool& value) { data.Prepend(value); }
	inline void PrependInteger(const int& value) { data.Prepend(value); }
	inline void PrependLongInteger(const long long& value) { data.Prepend(value); }
	inline void PrependFloatingPoint(const double& value) { data.Prepend(value); }
	inline void PrependString(const TCHAR* value) { data.Prepend(value); }
	inline void PrependString(const EspString& value) { data.Prepend(value); }
	inline void PrependStringPtr(const EspString* value) { data.Prepend(value); }
	inline void PrependJsonObject(const EspJsonObject& value) { data.Prepend(value); }
	inline void PrependJsonObjectPtr(const EspJsonObject* value) { data.Prepend(value); }
	inline void PrependJsonArray(const EspJsonArray& value) { data.Prepend(value); }
	inline void PrependJsonArrayPtr(const EspJsonArray* value) { data.Prepend(value); }

	inline void InsertValue(unsigned int index, const EspJsonValue& value) { data.Insert(index, value); }
	inline void InsertBoolean(unsigned int index, const bool& value) { data.Insert(index, value); }
	inline void InsertInteger(unsigned int index, const int& value) { data.Insert(index, value); }
	inline void InsertLongInteger(unsigned int index, const long long& value) { data.Insert(index, value); }
	inline void InsertFloatingPoint(unsigned int index, const double& value) { data.Insert(index, value); }
	inline void InsertString(unsigned int index, const TCHAR* value) { data.Insert(index, value); }
	inline void InsertString(unsigned int index, const EspString& value) { data.Insert(index, value); }
	inline void InsertStringPtr(unsigned int index, const EspString* value) { data.Insert(index, value); }
	inline void InsertJsonObject(unsigned int index, const EspJsonObject& value) { data.Insert(index, value); }
	inline void InsertJsonObjectPtr(unsigned int index, const EspJsonObject* value) { data.Insert(index, value); }
	inline void InsertJsonArray(unsigned int index, const EspJsonArray& value) { data.Insert(index, value); }
	inline void InsertJsonArrayPtr(unsigned int index, const EspJsonArray* value) { data.Insert(index, value); }

	inline void DeleteValue(unsigned int index) { data.Delete(index); }
	inline void DeleteValue(unsigned int index, unsigned int count) { data.Delete(index, count); }

	EspString Synthesize()const;
};

inline EspJsonValue::EspJsonValue(const EspJsonObject& new_value) : type(Type::Object)
{
	::EspAllocBuffer<EspJsonObject>(value.obj);
	new(value.obj)EspJsonObject(new_value);
}
inline EspJsonValue::EspJsonValue(EspJsonObject* new_value) : type(Type::Object) { value.obj = new_value; }
inline EspJsonValue::EspJsonValue(const EspJsonArray& new_value) : type(Type::Array)
{
	::EspAllocBuffer<EspJsonArray>(value.arr);
	new(value.arr)EspJsonArray(new_value);
}
inline EspJsonValue::EspJsonValue(EspJsonArray* new_value) : type(Type::Array) { value.arr = new_value; }
EspJsonValue::EspJsonValue(const EspJsonValue& new_value) : type(new_value.type)
{
	switch (type)
	{
	case EspJsonValueType::Null:
		break;
	case EspJsonValueType::Boolean:
		value.boolean = new_value.value.boolean;
		break;
	case EspJsonValueType::Integer:
		value.integer = new_value.value.integer;
		break;
	case EspJsonValueType::LongInteger:
		value.long_integer = new_value.value.long_integer;
		break;
	case EspJsonValueType::FloatingPoint:
		value.floating_point = new_value.value.floating_point;
		break;
	case EspJsonValueType::String:
		::EspAllocBuffer<EspString>(value.string);
		new(value.string)EspString(*new_value.value.string);
		break;
	case EspJsonValueType::Object:
		::EspAllocBuffer<EspJsonObject>(value.obj);
		new(value.obj)EspJsonObject(*new_value.value.obj);
		break;
	case EspJsonValueType::Array:
		::EspAllocBuffer<EspJsonArray>(value.arr);
		new(value.arr)EspJsonArray(*new_value.value.arr);
		break;
	default:
		break;
	}
}
void EspJsonValue::FreeValue()
{
	switch (type)
	{
	case EspJsonValueType::String:
		value.string->~EspString();
		::EspDeAllocBuffer<EspString>(value.string);
		break;
	case EspJsonValueType::Object:
		value.obj->~EspJsonObject();
		::EspDeAllocBuffer<EspJsonObject>(value.obj);
		break;
	case EspJsonValueType::Array:
		value.arr->~EspJsonArray();
		::EspDeAllocBuffer<EspJsonArray>(value.arr);
		break;
	default:
		break;
	}
}
inline EspJsonObject EspJsonValue::GetObject() { assert(IsObject()); return *value.obj; }
inline const EspJsonObject& EspJsonValue::GetConstObject()const { assert(IsObject()); return *value.obj; }
inline EspJsonObject* EspJsonValue::GetObjectPtr()const { assert(IsObject()); return value.obj; }
inline EspJsonArray EspJsonValue::GetArray() { assert(IsArray()); return *value.arr; }
inline const EspJsonArray& EspJsonValue::GetConstArray()const { assert(IsArray()); return *value.arr; }
inline EspJsonArray* EspJsonValue::GetArrayPtr()const { assert(IsArray()); return value.arr; }
inline void EspJsonValue::SetArray(const EspJsonArray& new_value)
{
	if (!IsArray())
	{
		FreeValue();
		type = Type::Array;
	}
	EspAllocBuffer<EspJsonArray>(value.arr);
	new(value.arr)EspJsonArray(new_value);
}
inline void EspJsonValue::SetArrayPtr(EspJsonArray* new_value)
{
	if (!IsArray())
	{
		FreeValue();
		type = Type::Array;
	}
	value.arr = new_value;
}
inline void EspJsonValue::SetObject(const EspJsonObject& new_value)
{
	if (!IsObject())
	{
		FreeValue();
		type = Type::Object;
	}
	EspAllocBuffer<EspJsonObject>(value.obj);
	new(value.obj)EspJsonObject(new_value);
}
inline void EspJsonValue::SetObjectPtr(EspJsonObject* new_value)
{
	if (!IsObject())
	{
		FreeValue();
		type = Type::Object;
	}
	value.obj = new_value;
}

enum class EspJsonErrorCode
{
	Error_NoError,
	Error_Invalid_Character,
	Error_Invalid_Escape_Character,
	Error_Invalid_Number,
	Error_Miss_Quote,
	Error_Miss_Comma,
	Error_Miss_Colon,
	Error_Miss_Brace,
	Error_Miss_Bracket,
	Error_Length_Out_Of_Range
};
class EspJsonParseResult
{
private:
	const EspJsonErrorCode error;
	const unsigned int pos;

public:
	inline EspJsonParseResult(const EspJsonErrorCode& code, const unsigned int& pos) :error(code), pos(pos) {}
	inline const EspJsonErrorCode& GetErrorCode()const { return error; }
	inline const unsigned int GetErrorPos()const { return pos; }
	inline bool HasError()const { return error != EspJsonErrorCode::Error_NoError; }
};
class EspJsonParser
{
public:
private:
	struct EspJsonInfo
	{
		const TCHAR* const json;
		const TCHAR* pos;
		const unsigned int size;
		EspJsonErrorCode code;
	};
	using Code = EspJsonErrorCode;

	static void SkipSpace(EspJsonInfo& info)
	{
		while ((*info.pos) == ' ' ||
			(*info.pos) == '\n' ||
			(*info.pos) == '\r' ||
			(*info.pos) == '\t')
			++info.pos;
	}

	static void ParseKey(EspJsonInfo& info, EspString& key)
	{
		while (true) //Infinite loop, the loop continues until any quote without escape characters are detected
		{
			switch (*info.pos)
			{
			case '\\'://escape character, json format support 8 escape character.
				++info.pos;
				switch (*info.pos)
				{
				case '"':key.Append(*info.pos); ++info.pos; break;
				case '\\':key.Append(*info.pos); ++info.pos; break;
				case '/':key.Append(*info.pos); ++info.pos; break;
				case 'b':key.Append(*info.pos); ++info.pos; break;
				case 'f':key.Append(*info.pos); ++info.pos; break;
				case 'n':key.Append(*info.pos); ++info.pos; break;
				case 'r':key.Append(*info.pos); ++info.pos; break;
				case 't':key.Append(*info.pos); ++info.pos; break;
				case 'u':
					break;
				default:
					info.code = Code::Error_Invalid_Escape_Character;
					return;
				}
				break;

			case '"':
				++info.pos; //Skip the quote
				return;

			case '\n':
			case '\r': //Json string do not permit this two characters.
				info.code = Code::Error_Invalid_Character;
				return;


			default: // All other situation is ok and be seemed as normal characters.
				key.Append(*info.pos);
				++info.pos;
				break;
			}
		}
	}

	static void ParseNull(EspJsonInfo& info, EspJsonValue& value)
	{
		if (*(++info.pos) == __T('u') && *(++info.pos) == __T('l') && *(++info.pos) == __T('l')) // must match true
		{
			value.SetNull();
			++info.pos;
		}
		else
			info.code = Code::Error_Invalid_Character; // or, set the error.
	}
	static void ParseTrue(EspJsonInfo& info, EspJsonValue& value)
	{
		if (*(++info.pos) == __T('r') && *(++info.pos) == __T('u') && *(++info.pos) == __T('e')) // must match true
		{
			value.SetBoolean(true);
			++info.pos;
		}
		else
			info.code = Code::Error_Invalid_Character; // or, set the error.
	}
	static void ParseFalse(EspJsonInfo& info, EspJsonValue& value)
	{
		if (*(++info.pos) == __T('a') && *(++info.pos) == __T('l') && *(++info.pos) == __T('s') && *(++info.pos) == __T('e')) // must match true
		{
			value.SetBoolean(false);
			++info.pos;
		}
		else
			info.code = Code::Error_Invalid_Character; // or, set the error.
	}
	static void ParseNumber(EspJsonInfo& info, EspJsonValue& value)
	{
		EspString str;
		if (*info.pos == '-')
		{
			str.Append('-');
			++info.pos;
		}
		if (*info.pos == '0')
		{
			str.Append('0');
			++info.pos;
		}
		else
		{
			if (!(*info.pos >= '1' && *info.pos <= '9'))
			{
				info.code = EspJsonErrorCode::Error_Invalid_Number;
				return;
			}
			while (*info.pos >= '0' && *info.pos <= '9')
			{
				str.Append(*info.pos);
				++info.pos;
			}
		}
		unsigned int demicial_count = 0;
		if (*info.pos == '.')
		{
			str.Append('.');
			++info.pos;
			if (!(*info.pos >= '0' && *info.pos <= '9'))
			{
				info.code = EspJsonErrorCode::Error_Invalid_Number;
				return;
			}
			while (*info.pos >= '0' && *info.pos <= '9')
			{
				str.Append(*info.pos);
				++info.pos;
				++demicial_count;
			}
		}
		else
		{
			if (str.GetAt(0) == '-')
				if (str.GetLength() > 10)
					value.SetLongInteger(::_ttoll(str.GetConstData()));
				else
					value.SetInteger(::_ttoi(str.GetConstData()));
			else
				if (str.GetLength() > 9)
					value.SetLongInteger(::_ttoll(str.GetConstData()));
				else
					value.SetInteger(::_ttoi(str.GetConstData()));
			return;
		}
		if (*info.pos == 'e' || *info.pos == 'E')
		{
			str.Append(*info.pos);
			++info.pos;
			if (*info.pos == '+' || *info.pos == '-')
			{
				str.Append(*info.pos);
				++info.pos;
			}
			if (!(*info.pos >= '0' && *info.pos <= '9'))
			{
				info.code = EspJsonErrorCode::Error_Invalid_Number;
				return;
			}
			while (*info.pos >= '0' && *info.pos <= '9')
			{
				str.Append(*info.pos);
				++info.pos;
			}
		}
		value.SetFloatingPoint(::_tcstod(str.GetConstData(), nullptr),demicial_count);
	}
	static void ParseString(EspJsonInfo& info, EspJsonValue& value)
	{
		EspString* str = new EspString;
		++info.pos;
		while (true) //Infinite loop, the loop continues until any quote without escape characters are detected
		{
			switch (*info.pos)
			{
			case '\\'://escape character, json format support 8 escape character.
				++info.pos;
				switch (*info.pos)
				{
				case '"':str->Append(*info.pos); ++info.pos; break;
				case '\\':str->Append(*info.pos); ++info.pos; break;
				case '/':str->Append(*info.pos); ++info.pos; break;
				case 'b':str->Append(*info.pos); ++info.pos; break;
				case 'f':str->Append(*info.pos); ++info.pos; break;
				case 'n':str->Append(*info.pos); ++info.pos; break;
				case 'r':str->Append(*info.pos); ++info.pos; break;
				case 't':str->Append(*info.pos); ++info.pos; break;
				case 'u':
					break;
				default:
					info.code = Code::Error_Invalid_Escape_Character;
					return;
				}
				break;

			case '"':
				value.SetStringPtr(str);
				++info.pos; //Skip the quote
				return;

			case '\n':
			case '\r': //Json string do not permit this two characters.
				info.code = Code::Error_Invalid_Character;
				return;

			default: // All other situation is ok and be seemed as normal characters.
				str->Append(*info.pos);
				++info.pos;
				break;
			}
		}
	}

	static void ParseObject(EspJsonInfo& info, EspJsonValue& value)
	{
		EspJsonObject* obj = new EspJsonObject;
		++info.pos; //Skip that '{'
		EspJsonParser::SkipSpace(info);
		if (*info.pos == '}')
		{
			value.SetObjectPtr(obj);
			++info.pos;
			return;
		}

		for (unsigned int index = 0;; ++index) //Infinite loop, the loop continues until '}' is detected
		{
			EspJsonParser::SkipSpace(info);

			if (*info.pos != '"')
			{
				info.code = Code::Error_Miss_Quote;
				return;
			}
			++info.pos;
			obj->AppendMember(EspJsonMember());
			EspString& key_ref = obj->GetAt(index).GetKeyRef();
			EspJsonParser::ParseKey(info, key_ref);

			if (info.code != Code::Error_NoError)
				return;

			EspJsonParser::SkipSpace(info);
			if (*info.pos != ':')
			{
				info.code = Code::Error_Miss_Comma;
				return;
			}
			++info.pos;
			EspJsonParser::SkipSpace(info);

			EspJsonValue& value_ref = obj->GetAt(index).GetValueRef();
			EspJsonParser::ParseValue(info, value_ref);
			if (info.code != Code::Error_NoError)
				return;

			EspJsonParser::SkipSpace(info);
			if (*info.pos == '}')
			{
				value.SetObjectPtr(obj);
				++info.pos;
				return;
			}
			else if (*info.pos != ',')
			{
				info.code = Code::Error_Miss_Colon;
				return;
			}
			++info.pos;
		}
	}
	static void ParseArray(EspJsonInfo& info, EspJsonValue& value)
	{
		EspJsonArray* arr = new EspJsonArray;
		++info.pos; //Skip that '['
		EspJsonParser::SkipSpace(info);
		if (*info.pos == ']')
		{
			value.SetArrayPtr(arr);
			++info.pos;
			return;
		}
		for (unsigned int index = 0;; ++index) //Infinite loop, the loop continues until '}' is detected
		{
			EspJsonParser::SkipSpace(info);

			arr->AppendValue(EspJsonValue());
			EspJsonValue& value_ref = arr->GetAt(index);
			EspJsonParser::ParseValue(info, value_ref);
			if (info.code != Code::Error_NoError)
				return;

			EspJsonParser::SkipSpace(info);
			if (*info.pos == ']')
			{
				value.SetArrayPtr(arr);
				++info.pos;
				return;
			}
			else if (*info.pos != ',')
			{
				info.code = Code::Error_Miss_Colon;
				return;
			}
			++info.pos;
		}
	}
	static void ParseValue(EspJsonInfo& info, EspJsonValue& value)
	{
		if (info.code != Code::Error_NoError)
			return;

		switch (*info.pos)
		{
		case 'n':ParseNull(info, value); return;
		case 't':ParseTrue(info, value); return;
		case 'f':ParseFalse(info, value); return;
		case '-':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':ParseNumber(info, value); return;
		case '"':ParseString(info, value); return;
		case '[':ParseArray(info, value); return;
		case '{':ParseObject(info, value); return;
		default:
			info.code = Code::Error_Invalid_Character;
			return;
		}
	}


public:
	static EspJsonParseResult Parse(const TCHAR* json, unsigned int size, EspJsonObject& result)
	{
		EspJsonInfo info = { json,json,size };

		EspJsonParser::SkipSpace(info); //Skip space and comment: ' ', '\n', '\t', '\r', and /* something */ at first.
		while (*info.pos != '{')
			++info.pos;

		if (*info.pos != '{') //The first character must be '{', the start sign of an json file.
		{
			info.code = Code::Error_Miss_Brace;
			return EspJsonParseResult(info.code, info.pos - info.json);
		}
		++info.pos; //Skip that '{'

		if (*info.pos == '}')
			return EspJsonParseResult(info.code, -1);

		for (unsigned int index = 0;; ++index) //Infinite loop, the loop continues until '}' is detected
		{
			EspJsonParser::SkipSpace(info);

			if (*info.pos != '"')
			{
				info.code = Code::Error_Miss_Quote;
				return EspJsonParseResult(info.code, info.pos - info.json);
			}
			++info.pos;
			result.AppendMember(EspJsonMember());
			EspString& key_ref = result.GetAt(index).GetKeyRef();
			EspJsonParser::ParseKey(info, key_ref);

			if (info.code != Code::Error_NoError)
				return EspJsonParseResult(info.code, info.pos - info.json);

			EspJsonParser::SkipSpace(info);
			if (*info.pos != ':')
			{
				info.code = Code::Error_Miss_Comma;
				return EspJsonParseResult(info.code, info.pos - info.json);
			}
			++info.pos;
			EspJsonParser::SkipSpace(info);

			EspJsonValue& value_ref = result.GetAt(index).GetValueRef();
			EspJsonParser::ParseValue(info, value_ref);
			if (info.code != Code::Error_NoError)
				return EspJsonParseResult(info.code, info.pos - info.json);

			EspJsonParser::SkipSpace(info);
			if (*info.pos == '}')
				return EspJsonParseResult(info.code, -1);
			else if (*info.pos != ',')
			{
				info.code = Code::Error_Miss_Colon;
				return EspJsonParseResult(info.code, info.pos - info.json);
			}
			++info.pos;
		}
	}
};

#ifdef HAS_UNDEFINED_GET_OBJECT_API
#pragma pop_macro("GetObject")
#undef HAS_UNDEFINED_GET_OBJECT_API
#endif