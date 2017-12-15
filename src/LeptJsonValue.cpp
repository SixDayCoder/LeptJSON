
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

		memset(&values, 0, sizeof(Container));
	}

	//Value::Value(const Value & other)
	//{
	//	if (this == &other) {
	//		return;
	//	}

	//	this->Reset();

	//	switch (other.GetType()) {
	//		case LeptJsonType::LEPT_JSON_EMPTY:break;
	//		case LeptJsonType::LEPT_JSON_NULL:
	//		case LeptJsonType::LEPT_JSON_TRUE:
	//		case LeptJsonType::LEPT_JSON_FALSE:
	//			this->SetType(other.GetType());
	//		break;

	//		case LeptJsonType::LEPT_JSON_NUMBER: {
	//			this->SetType(LeptJsonType::LEPT_JSON_NUMBER);
	//			this->values.numberValue = other.values.numberValue;
	//		}
	//		break;

	//		case LeptJsonType::LEPT_JSON_STRING: {
	//			this->SetType(LeptJsonType::LEPT_JSON_STRING);
	//			this->values.stringValue = new String(*other.GetString());
	//		}
	//		break;

	//		case LeptJsonType::LEPT_JSON_ARRAY: {
	//			this->SetType(LeptJsonType::LEPT_JSON_ARRAY);
	//			this->values.arrayValue = new Array(*other.GetArray());
	//		}
	//		break;

	//		case LeptJsonType::LEPT_JSON_OBJECT: {
	//			this->SetType(LeptJsonType::LEPT_JSON_OBJECT);
	//			this->values.objectValue = new Object(*other.GetObject());
	//		}
	//		break;
	//		
	//		default:break;
	//	}
	//}

	//Value & Value::operator=(const Value & other)
	//{
	//	// TODO: insert return statement here
	//}

	//Boolean Value::operator==(const Value & other)
	//{
	//	if (this == &other)
	//		return true;

	//	if (this->type != other.type) {
	//		return false;
	//	}
	//	else {

	//	}

	//}

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
