#ifndef LEPTJSON_JSONVALUE_H_
#define LEPTJSON_JSONVALUE_H_

#include <string>
#include <vector>
#include <map>
#include <istream>
#include <cassert>

namespace leptjson {

	//Ç°ÖÃÉùÃ÷
	class LeptJsonReader;
	class LeptJsonValue;
	class Array;

	using Number = double;
	using Boolean = bool;
	using String = std::string;
	using StringPtr = String*;
	using LeptJsonValuePtr = LeptJsonValue*;
	using Object = std::map<std::string, LeptJsonValuePtr>;
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


	class Array {
		private:
			std::vector<LeptJsonValuePtr> m_val;
		public:

			void PushValue(const LeptJsonValuePtr& val) {
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


	};
}

#endif // !LEPTJSON_JSONVALUE_H_
