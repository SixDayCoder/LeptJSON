
#include "LeptJsonValue.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

namespace leptjson {

	std::ostream & LeptJsonValue::FormatValue(std::ostream & output, const LeptJsonValue& v)
	{
		switch (v.GetType()) {
		case LeptJsonType::LEPT_JSON_NULL:
			output << "LeptJsonValue is : " << "null" << std::endl;
			break;

		case LeptJsonType::LEPT_JSON_TRUE:
			output << "LeptJsonValue is : " << "true" << std::endl;
			break;

		case LeptJsonType::LEPT_JSON_FALSE:
			output << "LeptJsonValue is : " << "false" << std::endl;
			break;

		case LeptJsonType::LEPT_JSON_NUMBER:
			output << "LeptJsonValue is : " << v.GetNumber() << std::endl;
			break;

		case LeptJsonType::LEPT_JSON_STRING:
			output << "LeptJsonValue is : " << v.GetString() << std::endl;
			break;

		case LeptJsonType::LEPT_JSON_ARRAY:
			output << "LeptJsonValue is : " << v.GetArray() << std::endl;
			break;

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
		for (size_t i = 0; i < rhs.m_val.size(); ++i) {
			const LeptJsonValuePtr& val = rhs.m_val[i];
			LeptJsonType type = val->GetType();
			LeptJsonValue::FormatValue(output, *val);
		}
		return output;
	}

	std::ostream & operator<<(std::ostream & output, const LeptJsonValue & v)
	{
		if (v.isHaveKeys) {
			const Object& json = v.GetObject();
			for (auto it = json.begin(); it != json.end(); ++it) {
				const String& key = it->first;
				const LeptJsonValue&  val = *it->second;

				output << "key is : " << key;

				LeptJsonValue::FormatValue(output, val);

				/*switch (type) {
					case LeptJsonType::LEPT_JSON_NULL:
						output << "LeptJsonValue is : " << "null" << std::endl;
					break;

					case LeptJsonType::LEPT_JSON_TRUE:
						output << "LeptJsonValue is : " << "true" << std::endl;
					break;

					case LeptJsonType::LEPT_JSON_FALSE:
						output << "LeptJsonValue is : " << "false" << std::endl;
					break;

					case LeptJsonType::LEPT_JSON_NUMBER:
						output << "LeptJsonValue is : " << val.GetNumber() << std::endl;
					break;

					case LeptJsonType::LEPT_JSON_STRING:
						output << "LeptJsonValue is : " << val.GetString() << std::endl;
					break;

					case LeptJsonType::LEPT_JSON_ARRAY:
						output << "LeptJsonValue is : " << val.GetArray() << std::endl;
					break;

					default:break;
				}*/

			}
		}
		//只有一个LeptJsonValue不含键
		else {
			LeptJsonValue::FormatValue(output, v);
		}

		return output;
	}
}
