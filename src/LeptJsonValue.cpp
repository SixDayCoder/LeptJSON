
#include "LeptJsonValue.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

namespace leptjson {

	std::ostream & LeptJsonValue::FormatValue(std::ostream & output, const LeptJsonValue& v)
	{
		switch (v.GetType()) {

		case LeptJsonType::LEPT_JSON_NULL:
			output << "null" << std::endl;
			break;

		case LeptJsonType::LEPT_JSON_TRUE:
			output << "true" << std::endl;
			break;

		case LeptJsonType::LEPT_JSON_FALSE:
			output << "false" << std::endl;
			break;

		case LeptJsonType::LEPT_JSON_NUMBER:
			output << v.GetNumber() << std::endl;
			break;

		case LeptJsonType::LEPT_JSON_STRING:
			output << v.GetString() << std::endl;
			break;

		case LeptJsonType::LEPT_JSON_ARRAY:
			output << std::endl << v.GetArray();
			break;

		case LeptJsonType::LEPT_JSON_OBJECT:
			output << std::endl << v.GetObject();

		default:break;
		}

		return output;
	}

	LeptJsonValue::LeptJsonValue()
	{
		type = LeptJsonType::LEPT_JSON_EMPTY;
		isHaveKeys = false;
		//清空共享内存
		memset(&values, 0, sizeof(Container));
	}

	LeptJsonValue::~LeptJsonValue()
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

	void LeptJsonValue::Reset()
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

	std::ostream & operator<<(std::ostream & output, const Array & rhs)
	{
		output << '[' << std::endl;
		size_t cnt = rhs.m_val.size();
		for (size_t i = 0; i < cnt ; ++i) {
			const LeptJsonValuePtr& val = rhs.m_val[i];
			LeptJsonValue::FormatValue(output, *val);
		}
		output << ']' << std::endl;
		return output;
	}


	std::ostream & operator<<(std::ostream & output, const Object & rhs)
	{
		output << '{' << std::endl;
		for (auto it = rhs.m_map.begin(); it != rhs.m_map.end(); it++) {
			const String& key = it->first;
			const LeptJsonValue&  val = *it->second;
			output << key << " : ";
			LeptJsonValue::FormatValue(output, val);
		}
		output << '}' << std::endl;
		return output;
	}

	std::ostream & operator<<(std::ostream & output, const LeptJsonValue & v)
	{
		if (v.isHaveKeys) {
			const Object& json = v.GetObject();
			output << json << std::endl;
		}
		//只有一个LeptJsonValue不含键
		else {
			LeptJsonValue::FormatValue(output, v);
		}
		return output;
	}

	void Object::InsertValue(const std::pair<std::string, LeptJsonValuePtr>& key_value)
	{
		m_map.insert(key_value);
	}

	LeptJsonValuePtr & Object::operator[](const String & key)
	{
		return m_map[key];
	}
}
