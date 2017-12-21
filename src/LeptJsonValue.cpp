
#include "LeptJsonValue.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

namespace leptjson {

	LeptJsonValue::LeptJsonValue()
	{
		type = LeptJsonType::LEPT_JSON_EMPTY;
		isHaveKeys = false;	
	}

	LeptJsonValue::LeptJsonValue(Number number)
	{
		isHaveKeys = false;

		values.numberValue = number;
		SetType(LeptJsonType::LEPT_JSON_NUMBER);
	}

	LeptJsonValue::LeptJsonValue(const String & src)
	{
		isHaveKeys = false;
		memset(&values, 0, sizeof(Container));

		if (src == "null") {
			SetType(LeptJsonType::LEPT_JSON_NULL);
		}
		else if (src == "false") {
			SetType(LeptJsonType::LEPT_JSON_FALSE);
		}
		else if (src == "true") {
			SetType(LeptJsonType::LEPT_JSON_TRUE);
		}
		else {
			values.stringValue = std::make_shared<String>(src);
			SetType(LeptJsonType::LEPT_JSON_STRING);
		}
	}

	LeptJsonValue::LeptJsonValue(const Array & vec)
	{
		isHaveKeys = false;
		values.arrayValue = std::make_shared<Array>(vec);
		SetType(LeptJsonType::LEPT_JSON_ARRAY);
	}

	LeptJsonValue::LeptJsonValue(const Object & obj)
	{
		isHaveKeys = false;
		values.objectValue = std::make_shared<Object>(obj);
		SetType(LeptJsonType::LEPT_JSON_OBJECT);
	}


	void LeptJsonValue::Reset()
	{
		if (type == LeptJsonType::LEPT_JSON_STRING) {
			values.stringValue = 0;
		}
		else if (type == LeptJsonType::LEPT_JSON_ARRAY) {
			values.arrayValue = 0;
		}
		else if (type == LeptJsonType::LEPT_JSON_OBJECT) {
			values.objectValue = 0;
		}
		else if (type == LeptJsonType::LEPT_JSON_NUMBER) {
			values.numberValue = 0;
		}
	}



	std::ostream & LeptJsonValue::FormatValue(std::ostream & output, const LeptJsonValue& v)
	{
		switch (v.GetType()) {

		case LeptJsonType::LEPT_JSON_NULL:
			output << v.GetNull();
			break;

		case LeptJsonType::LEPT_JSON_TRUE:
			output << v.GetTrue();
			break;

		case LeptJsonType::LEPT_JSON_FALSE:
			output << v.GetFalse();
			break;

		case LeptJsonType::LEPT_JSON_NUMBER:
			output << v.GetNumber();
			break;

		case LeptJsonType::LEPT_JSON_STRING:
			output << v.GetString();
			break;

		case LeptJsonType::LEPT_JSON_ARRAY:
			output << v.GetArray();
			break;

		case LeptJsonType::LEPT_JSON_OBJECT:
			output << v.GetObject();
			break;

		default:break;
		}

		return output;
	}

	std::ostream & operator<<(std::ostream & output, const Array & rhs)
	{
		if (rhs.empty()) {
			output << "[]";
		}
		else {
			output << '[';
			size_t cnt = rhs.m_val.size();
			for (size_t i = 0; i < cnt; ++i) {
				const LeptJsonValuePtr& val = rhs.m_val[i];
				LeptJsonValue::FormatValue(output, *val);
				if(i != cnt - 1)
					output << ",";
			}
			output << ']';
		}
		return output;
	}

	std::ostream & operator<<(std::ostream & output, const Object & rhs)
	{
		if (rhs.empty()) {
			output << "{}";
		}
		else
		{
			output << '{';
			auto it = rhs.m_map.begin();
			while (it != rhs.m_map.end()) {
				const String& key = it->first;
				const LeptJsonValue&  val = *it->second;
				output << key << " : ";
				LeptJsonValue::FormatValue(output, val);
				it++;
				if (it != rhs.m_map.end())
					output << ',';
			}
			output << '}';
		}
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

}
