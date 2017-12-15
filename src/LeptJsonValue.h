#ifndef LEPTJSON_JSONVALUE_H_
#define LEPTJSON_JSONVALUE_H_

#include <string>
#include <vector>
#include <map>
#include <istream>
#include <cassert>

namespace leptjson {

	//Ç°ÖÃÉùÃ÷
	class Value;
	class LeptJsonParser;

	using Number = double;
	using Boolean = bool;
	using String = std::string;
	using Null = String;
	using True = String;
	using False = String;
	using StringPtr = String*;
	using ValuePtr = Value*;
	using Array = std::vector<ValuePtr>;
	using Object = std::map<std::string, ValuePtr>;
	using ArrayPtr = Array*;
	using ObjectPtr = Object*;

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

	class Value 
	{

	public:
		friend class LeptJsonParser;

	private:

		LeptJsonType type;

		union Container
		{
			Number numberValue;
			StringPtr stringValue;
			ArrayPtr arrayValue;
			ObjectPtr objectValue;

		}values;

	public:
		
		Value();
		~Value();
		/*Value(const Value& other);
		Value& operator=(const Value& other);
		Boolean operator==(const Value& other);*/

		void Reset();

		Boolean IsNull()const {
			return type == LeptJsonType::LEPT_JSON_NULL;
		}
		const String& GetNull() {
			assert(IsNull());
			return "null";
		}

		Boolean IsTrue()const {
			return type == LeptJsonType::LEPT_JSON_TRUE;
		}
		const String& GetTrue()const{
			assert(IsTrue());
			return "true";
		}

		Boolean IsFalse()const {
			return type == LeptJsonType::LEPT_JSON_FALSE;
		}
		const String& GetFalse()const {
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

		template<typename ElementType>
		const ElementType& Get()const {
			
		}

	};
}

#endif // !LEPTJSON_JSONVALUE_H_
