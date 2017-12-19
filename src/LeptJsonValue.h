#ifndef LEPTJSON_JSONVALUE_H_
#define LEPTJSON_JSONVALUE_H_

#include <string>
#include <vector>
#include <map>
#include <istream>
#include <cassert>

namespace leptjson {

	/************************************************************************/
	/* 前置声明和类型重定义                                                  */
	/************************************************************************/
	class LeptJsonReader;
	class LeptJsonValue;
	class Array;

	using Number = double;
	using Boolean = bool;
	using String = std::string;
	using StringPtr = String*;
	using LeptJsonValuePtr = LeptJsonValue*;

	/************************************************************************/
	/* 预定义enum                                                            */
	/************************************************************************/
	enum class LeptJsonType
	{
		LEPT_JSON_EMPTY,
		LEPT_JSON_NULL,
		LEPT_JSON_TRUE,
		LEPT_JSON_FALSE,
		LEPT_JSON_NUMBER,
		LEPT_JSON_STRING,
		LEPT_JSON_ARRAY,
		LEPT_JSON_OBJECT
	};

	/************************************************************************/
	/* 封装Array                                                            */
	/************************************************************************/
	class Array {
	private:
		std::vector<LeptJsonValuePtr> m_val;
	public:

		Array() {
			m_val.clear();
		}

		size_t size()const {
			return m_val.size();
		}

		Boolean empty()const {
			return m_val.empty();
		}

		void push_back(const LeptJsonValuePtr& val) {
				m_val.push_back(val);
			}

		const LeptJsonValuePtr& operator[](size_t index)const {
				return m_val[index];
			}

		LeptJsonValuePtr& operator[](size_t index){
				return m_val[index];
			}

		friend std::ostream& operator<<(std::ostream& output, const Array& rhs);
	};
	using ArrayPtr = Array*;

	/************************************************************************/
	/* 封装Object                                                           */
	/************************************************************************/
	class Object {
	private:
		std::map<String, LeptJsonValuePtr> m_map;
	public:

		using iterator = std::map<std::string, LeptJsonValuePtr>::iterator;
		using const_iterator = std::map<std::string, LeptJsonValuePtr>::const_iterator;

		Object() {
			m_map.clear();
		}

		const_iterator begin() const {
			return m_map.begin();
		}
		const_iterator end() const{
			return m_map.end();
		}
		const_iterator find(const String& key)const {
			return m_map.find(key);
		}

		iterator begin() {
			return m_map.begin();
		}
		iterator end() {
			return m_map.end();
		}
		iterator find(const String& key) {
			return m_map.find(key);
		}

		Boolean empty()const {
			return m_map.empty();
		}

		size_t size()const {
			return m_map.size();
		}

		void insert(const std::pair<std::string, LeptJsonValuePtr>& key_value)
		{
			m_map.insert(key_value);
		}
		
		const LeptJsonValuePtr& operator[](const String& key)
		{
			return m_map[key];
		}

		friend std::ostream& operator<<(std::ostream& output, const Object& rhs);

	};
	using ObjectPtr = Object*;

	/************************************************************************/
	/* 抽象,JsonValue                                                       */
	/************************************************************************/
	class LeptJsonValue 
	{

	public:
		static std::ostream& FormatValue(std::ostream& output, const LeptJsonValue& v);

	public:
		friend class LeptJsonReader;
		friend std::ostream& operator<<(std::ostream& output, const LeptJsonValue& v);
	private:

		LeptJsonType type;

		union Container
		{
			Number numberValue;
			StringPtr stringValue;
			ArrayPtr arrayValue;
			ObjectPtr objectValue;

		}values;

		Boolean isHaveKeys;

	public:
		
		LeptJsonValue();
		~LeptJsonValue();

		void Reset();
		
		Boolean IsHaveKeys()const {
			return isHaveKeys == true;
		}

		Boolean IsNull()const {
			return type == LeptJsonType::LEPT_JSON_NULL;
		}
		String GetNull()const{
			assert(IsNull());
			return "null";
		}

		Boolean IsTrue()const {
			return type == LeptJsonType::LEPT_JSON_TRUE;
		}
		String GetTrue()const{
			assert(IsTrue());
			return "true";
		}

		Boolean IsFalse()const {
			return type == LeptJsonType::LEPT_JSON_FALSE;
		}
		String GetFalse()const {
			assert(IsFalse());
			return "false";
		}

		Boolean IsNumber()const {
			return type == LeptJsonType::LEPT_JSON_NUMBER;
		}
		Number GetNumber()const {
			assert(IsNumber());
			return values.numberValue;
		}

		Boolean IsString()const {
			return type == LeptJsonType::LEPT_JSON_STRING;
		}
		const String& GetString()const {
			assert(IsString());
			return *values.stringValue;
		}
		String& GetString() {
			assert(IsString());
			return *values.stringValue;
		}

		Boolean IsArray()const {
			return type == LeptJsonType::LEPT_JSON_ARRAY;
		}
		const Array& GetArray()const {
			assert(IsArray());
			return *values.arrayValue;
		}
		Array& GetArray() {
			assert(IsArray());
			return *values.arrayValue;
		}

		Boolean IsObject()const {
			return type == LeptJsonType::LEPT_JSON_OBJECT;
		}
		const Object& GetObject()const {
			assert(IsObject());
			return *values.objectValue;
		}
		Object& GetObject() {
			assert(IsObject());
			return *values.objectValue;
		}

		void SetType(LeptJsonType type) {
			this->type = type;
		}
		LeptJsonType GetType()const {
			return type;
		}


	};
}

#endif // !LEPTJSON_JSONVALUE_H_
