#ifndef LEPTJSON_JSONVALUE_H_
#define LEPTJSON_JSONVALUE_H_

#include <string>
#include <vector>
#include <map>
#include <istream>

namespace LeptJson {

	//Ç°ÖÃÉùÃ÷
	class Value;

	using Number = double;
	using Boolean = bool;
	using String = std::string;
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

	enum class LeptParseRet
	{
		LEPT_PARSE_SUCCESS
	};

	class Value {

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

		void Reset();

		Boolean IsNull()const {
			return type == LeptJsonType::LEPT_JSON_NULL;
		}

		Boolean IsTrue()const {
			return type == LeptJsonType::LEPT_JSON_TRUE;
		}

		Boolean IsFalse()const {
			return type == LeptJsonType::LEPT_JSON_FALSE;
		}

		Boolean IsNumber()const {
			return type == LeptJsonType::LEPT_JSON_NUMBER;
		}

		Boolean IsString()const {
			return type == LeptJsonType::LEPT_JSON_STRING;
		}

		Boolean IsArray()const {
			return type == LeptJsonType::LEPT_JSON_ARRAY;
		}

		Boolean IsObject()const {
			return type == LeptJsonType::LEPT_JSON_OBJECT;
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
