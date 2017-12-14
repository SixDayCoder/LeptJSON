
#include "LeptJsonValue.h"
#include <cstring>
#include <cstdlib>

namespace leptjson {

	Value::Value()
	{
		type = LeptJsonType::LEPT_JSON_EMPTY;
		//Çå¿Õ¹²ÏíÄÚ´æ
		memset(&values, 0, sizeof(Container));

	}

	Value::~Value()
	{
		if (type == LeptJsonType::LEPT_JSON_STRING && values.stringValue != 0) {
			delete values.stringValue;
			values.stringValue = 0;
		}
		else if (type == LeptJsonType::LEPT_JSON_ARRAY && values.arrayValue != 0) {
			delete values.arrayValue;
			values.arrayValue = 0;
		}
		else if (type == LeptJsonType::LEPT_JSON_OBJECT && values.objectValue != 0) {
			delete values.objectValue;
			values.objectValue = 0;
		}
	}

	void Value::Reset()
	{
		if (type == LeptJsonType::LEPT_JSON_STRING) {
			delete values.stringValue;
			values.stringValue = 0;
		}
		else if (type == LeptJsonType::LEPT_JSON_ARRAY) {
			delete values.arrayValue;
			values.arrayValue = 0;
		}
		else if (type == LeptJsonType::LEPT_JSON_OBJECT) {
			delete values.objectValue;
			values.objectValue = 0;
		}
		else if (type == LeptJsonType::LEPT_JSON_NUMBER) {
			values.numberValue = 0;
		}
	}

}
