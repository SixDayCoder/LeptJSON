#include "LeptJsonParser.h"
#include <cstring>
#include <cassert>
#include <queue>
#include <iostream>

namespace leptjson {

	/************************************************************************/
	/* 解析器                                                                */
	/************************************************************************/
	void LeptJsonReader::LoadFromString(const char * json)
	{
		m_input = std::istringstream(json);
		if (m_value != 0) {
			m_value->Reset();
		}

	}

	void LeptJsonReader::LoadFromFile(const char * filepath)
	{
		std::ifstream file(filepath);
		assert(file.is_open());

		std::ostringstream out;
		out << file.rdbuf();
		m_input = std::istringstream(out.str());
		file.close();
		if (m_value != 0) {
			m_value->Reset();
		}
	}

	Boolean LeptJsonReader::TryMatchChar(std::istream & input, char ch)
	{
		//skip ws
		input >> std::ws;

		if (input.good() && !input.eof()) {
			char nextchar = input.peek();
			if (ch == nextchar) {
				input.get(nextchar);
				return true;
			}
			else {
				return false;
			}
		}

		return false;
		
	}

	Boolean LeptJsonReader::TryMatchString(std::istream & input, const String & src)
	{
		input >> std::ws;

		std::queue<char> buffer;
		while (!buffer.empty())
			buffer.pop();

		//char ch;
		size_t len = src.length();
		size_t cnt = 0;
		bool bSuccess = true;
		char ch;
		while (input.good() && len > 0){

			input.get(ch);

			if (input.eof()) {
				bSuccess = false;
				break;
			}

			if (src[cnt] == ch) {
				buffer.push(ch);
				cnt++;
				len--;
			}
			else {
				input.putback(ch);
				bSuccess = false;
				break;
			}
		}

		if (bSuccess == false) {
			while (!buffer.empty() && input.good()) {
				input.putback(buffer.front());
				buffer.pop();
			}
		}

		return bSuccess;
	}

	void LeptJsonReader::Parse()
	{
		if (m_value == 0) {
			m_value = LeptJsonValue::CreateLeptJsonValue();
		}
		else {
			m_value->Reset();
		}
		m_state = Parse(m_input, *m_value);
	}

	LeptJsonParseRet LeptJsonReader::Parse(std::istream & input, LeptJsonValue & value)
	{
		LeptJsonParseRet ret;
		ret = ParseLeptJsonValue(input, value);
		if (ret == LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
			//如果解析完还有字符
			if (!input.eof()) {
				input >> std::ws;
				if (!input.eof()) {
					value.SetType(LeptJsonType::LEPT_JSON_EMPTY);
					return LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
				}
			}
		}
		return ret;
	}

	LeptJsonParseRet LeptJsonReader::ParseLeptJsonValue(std::istream & input, LeptJsonValue & value)
	{
		assert(input.good());
		input >> std::ws;
		//空字符串
		if (input.eof()) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
		}
		else {
			char ch = input.peek();
			LeptJsonParseRet ret;
			switch (ch)
			{
				case 'n': return ParseLiteral(input, "null", value); break;
				case 't': return ParseLiteral(input, "true", value); break;
				case 'f': return ParseLiteral(input, "false", value); break;
				case '[': {
					value.values.arrayValue = std::make_shared<Array>();
					ret = ParseArray(input, value);
					if (ret != LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
						value.Reset();
					}
					if (ret == LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
						value.SetType(LeptJsonType::LEPT_JSON_ARRAY);
					}
					return ret;
				}
				break;
				case '{': {
					value.values.objectValue = std::make_shared <Object>();
					ret = ParseObject(input, value);
					if (ret != LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
						value.Reset();
					}
					if (ret == LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
						value.SetType(LeptJsonType::LEPT_JSON_OBJECT);
					}
					return ret;
				}
				break;

				case '\"': return ParseString(input, value); break;
				default: {
					if (isalpha(ch)) 
						return LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
					else
						return ParseNumber(input, value); break;
				}
			}
		}
	}

	LeptJsonParseRet LeptJsonReader::ParseLiteral(std::istream& input, const char* literal, LeptJsonValue& value)
	{
		if (!TryMatchString(input, literal)) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
		}
		else {
			if (literal == "null") {
				value.SetType(LeptJsonType::LEPT_JSON_NULL);
			}
			else if (literal == "false") {

				value.SetType(LeptJsonType::LEPT_JSON_FALSE);
			}
			else if (literal == "true") {
				value.SetType(LeptJsonType::LEPT_JSON_TRUE);
			}
			else {

			}
			return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
		}
	}

	LeptJsonParseRet LeptJsonReader::ParseNumber(std::istream& input, LeptJsonValue& value)
	{
		std::streampos rollback = input.tellg();
		double number;
		input >> number;
		if (input.fail()) {
			//fail之后记得clear
			input.clear();
			input.seekg(rollback);
			return LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
		}
		else {
			value.SetType(LeptJsonType::LEPT_JSON_NUMBER);
			value.values.numberValue = number;
			return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
		}
	}

	LeptJsonParseRet LeptJsonReader::ParseString(std::istream& input, LeptJsonValue& value)
	{
		char ch = '\0';
		char delimiter = '\"';

		//开头必须是"
		if (!TryMatchChar(input, delimiter)) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
		}

		ch = input.peek();
		if (ch == delimiter) {
			//如果是空字符串""
			input.get(ch);
			value.SetType(LeptJsonType::LEPT_JSON_STRING);
			value.values.stringValue= std::make_shared<String>("");
			return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
		}
		else {

			String buffer;
			while (!input.eof() && input.good()){
				input.get(ch);
				//转义字符
				switch (ch){

					//成功解析字符串
					case '\"': 
						value.SetType(LeptJsonType::LEPT_JSON_STRING);
						value.values.stringValue = std::make_shared<String>(buffer);
						return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
					break;

					//碰到转义字符 
					case '\\': 
						input.get(ch);
						switch (ch){
							case '\"':buffer.push_back('\"'); break;
							case '\\':buffer.push_back('\\'); break;
							case '/': buffer.push_back('/');  break;
							case 'b': buffer.push_back('\b'); break;
							case 'f': buffer.push_back('\f'); break;
							case 'n': buffer.push_back('\n'); break;
							case 'r': buffer.push_back('\r'); break;
							case 't': buffer.push_back('\t'); break;
							//TODO-Unicode
							//否则是不合法的转义字符序列
							default: return LeptJsonParseRet::LEPT_JSON_PARSE_FAIL; break;
						}
					break;

					//默认情况添加字符到buffer
					default: 
						if (static_cast<unsigned char>(ch) < 0x20) {
							return LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
						}
						else{
							buffer.push_back(ch);
						}
					break;
				}
			}
		}
		return LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
	}

	LeptJsonParseRet LeptJsonReader::ParseArray(std::istream & input, LeptJsonValue & value)
	{
		if (!TryMatchChar(input, '[')) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
		}

		char ch = '\0';
		ch = input.peek();
		if (TryMatchChar(input, ']')) {
			value.SetType(LeptJsonType::LEPT_JSON_ARRAY);
			value.values.arrayValue = std::make_shared<Array>();
			return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
		}

		LeptJsonParseRet ret;

		do {
			LeptJsonValuePtr v = LeptJsonValue::CreateLeptJsonValue();
			ret = ParseLeptJsonValue(input, *v);
			if (ret != LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
				v->Reset();
				break;
			}
			value.values.arrayValue->push_back(v);
		} while (TryMatchChar(input, ','));

		if (!TryMatchChar(input, ']')) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
		}

		return ret;
	}

	LeptJsonParseRet LeptJsonReader::ParseObject(std::istream & input, LeptJsonValue & value)
	{
		if (!TryMatchChar(input, '{')) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
		}
		char ch = '\0';
		ch = input.peek();
		if (TryMatchChar(input, '}')) {
			value.SetType(LeptJsonType::LEPT_JSON_OBJECT);
			value.values.objectValue = std::make_shared<Object>();
			return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
		}
		LeptJsonParseRet ret;
		do {
			String key;
			if (!ParseKey(input, key)) {
				ret = LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
				break;
			}

			if (!TryMatchChar(input, ':')) {
				ret = LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
				break;
			}

			LeptJsonValuePtr v = LeptJsonValue::CreateLeptJsonValue();
			ret = ParseLeptJsonValue(input, *v);

			if (ret != LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
				v->Reset();
				break;
			}
			value.isHaveKeys = true;
			value.values.objectValue->insert(std::make_pair(key, v));

		} while (TryMatchChar(input, ','));

		if (!TryMatchChar(input, '}')) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_FAIL;
		}
		return ret;
	}

	Boolean LeptJsonReader::ParseKey(std::istream & input, String & key)
	{
		char ch = '\0';
		char delimiter = '\"';

		if (!TryMatchChar(input, delimiter)) {
			return false;
		}

		String buffer;

		//key开头不能是数字,key的值只能是大小写字母,下划线,数字
		while (input.good() && !input.eof()){
			input.get(ch);
			//成功解析字符串
			if (ch == delimiter) {
				//去掉buffer首尾的空格,string.trim
				buffer.erase(0, buffer.find_first_not_of(" "));
				buffer.erase(buffer.find_last_not_of(' ') + 1);
				if (buffer.empty()) {
					return false;
				}
				//正确解析
				key = buffer;
				return true;
			}
			else {

				if (static_cast<unsigned char>(ch) < 0x20) {
					return false;
				}
				else {
					buffer.push_back(ch);
				}
			}
		}
		//直到流结束都没碰到delimiter,说明解析失败
		return false;
	}

	/************************************************************************/
	/* 生成器                                                                */
	/************************************************************************/

	void LeptJsonWriter::Push(const String & key, const LeptJsonValue & val)
	{
		LeptJsonType type = val.GetType();

		switch (type){
			case leptjson::LeptJsonType::LEPT_JSON_EMPTY:
			break;
			case leptjson::LeptJsonType::LEPT_JSON_NULL:
				Push(key, "null");
			break;
			case leptjson::LeptJsonType::LEPT_JSON_TRUE:
				Push(key, "true");
			break;
			case leptjson::LeptJsonType::LEPT_JSON_FALSE:
				Push(key, "false");
			break;
			case leptjson::LeptJsonType::LEPT_JSON_NUMBER:
				Push(key, val.GetNumber());
			break;
			case leptjson::LeptJsonType::LEPT_JSON_STRING:
				Push(key, val.GetString());
			break;
			case leptjson::LeptJsonType::LEPT_JSON_ARRAY:
				Push(key, val.GetArray());
			break;
			case leptjson::LeptJsonType::LEPT_JSON_OBJECT:
				Push(key, val.GetObject());
			break;
			default:	break;
		}
	}

	void LeptJsonWriter::Push(const String & key, const String & str)
	{
		String src(str);

		src.erase(0, src.find_first_not_of(" "));
		src.erase(src.find_last_not_of(' ') + 1);

		//literal
		if (src == "null") {
			LeptJsonValuePtr val = LeptJsonValue::CreateLeptJsonValue();
			val->SetType(LeptJsonType::LEPT_JSON_NULL);
			m_map.insert(std::make_pair(key, val));
		}
		else if (src == "true") {
			LeptJsonValuePtr val = LeptJsonValue::CreateLeptJsonValue();
			val->SetType(LeptJsonType::LEPT_JSON_TRUE);
			m_map.insert(std::make_pair(key, val));
		}
		else if (src == "false") {
			LeptJsonValuePtr val = LeptJsonValue::CreateLeptJsonValue();
			val->SetType(LeptJsonType::LEPT_JSON_FALSE);
			m_map.insert(std::make_pair(key, val));
		}
		else {
			LeptJsonValuePtr val = LeptJsonValue::CreateLeptJsonValue();
			val->SetString(src);
			m_map.insert(std::make_pair(key, val));
			//insert
		}
	}

	void LeptJsonWriter::Push(const String & key, Number number)
	{
		LeptJsonValuePtr val = LeptJsonValue::CreateLeptJsonValue();
		val->SetNumber(number);
		m_map.insert(std::make_pair(key, val));
	}

	void LeptJsonWriter::Push(const String & key, const Array & vec)
	{
		LeptJsonValuePtr val = LeptJsonValue::CreateLeptJsonValue();
		val->SetArray(vec);
		m_map.insert(std::make_pair(key, val));
	}

	void LeptJsonWriter::Push(const String & key, const Object & obj)
	{
		LeptJsonValuePtr val = LeptJsonValue::CreateLeptJsonValue();
		val->SetObject(obj);
		m_map.insert(std::make_pair(key, val));
	}

	void LeptJsonWriter::Stringify()
	{
		if (m_value == 0) {
			m_value = std::make_shared<LeptJsonValue>(m_map);
		}
		StringifyValue(m_output, *m_value);
		m_isStringifiied = true;
	}

	void LeptJsonWriter::StringifyValue(std::ostream & output, const LeptJsonValue & val)
	{
		LeptJsonType type = val.GetType();

		switch (type)
		{
			case leptjson::LeptJsonType::LEPT_JSON_EMPTY:
				output << "Empty Value" << std::endl;
			break;
			case leptjson::LeptJsonType::LEPT_JSON_NULL:
				output << "null";
			break;
			case leptjson::LeptJsonType::LEPT_JSON_TRUE:
				output << "true";
			break;
			case leptjson::LeptJsonType::LEPT_JSON_FALSE:
				output << "false";
			break;
			case leptjson::LeptJsonType::LEPT_JSON_NUMBER: {
				Number number = val.GetNumber();
				output << number;
			}
			break;
			case leptjson::LeptJsonType::LEPT_JSON_STRING: {
				const String& src = val.GetString();
				StringifyString(output, src);
			}
			break;
			case leptjson::LeptJsonType::LEPT_JSON_ARRAY: {
				const Array& vec = val.GetArray();
				output << '[';
				for (size_t i = 0; i < vec.size(); ++i) {
					StringifyValue(output, *vec[i]);
					if (i != vec.size() - 1) output << ',';
				}
				output << ']';
			}
			break;
			case leptjson::LeptJsonType::LEPT_JSON_OBJECT: {
				const Object& obj = val.GetObject();
				output << '{';
				auto it = obj.begin();
				while (it != obj.end()) {
					const String& key = it->first;
					const LeptJsonValue& v = *it->second;
					output << key << " : ";
					StringifyValue(output, v);
					it++;
					if (it != obj.end()) output << ',';
				}
				output << '}';
			}
			break;
			default:
			break;
		}
	}

	void LeptJsonWriter::StringifyString(std::ostream & output, const String & val)
	{
		output  << '"';

		for (size_t i = 0; i < val.length(); ++i) {
			//char curr = val[i];
			unsigned char ch = static_cast<unsigned char>(val[i]);
			switch (ch){
				case '\"': output << "\\\""; break;
				case '\\': output << "\\\\"; break;
				case '\b': output << "\\b";  break;
				case '\f': output << "\\f";  break;
				case '\n': output << "\\n";  break;
				case '\r': output << "\\r";  break;
				case '\t': output << "\\t";  break;
				default:   output << ch;     break;
			}

		}

		output << '"';
	}

	void LeptJsonWriter::Write(const char * filepath)
	{
		if (m_isStringifiied == false) {
			Stringify();
		}
		
		std::cout << m_output.str() << std::endl;

		std::wfstream file(filepath, std::ios::out);

		assert(file.is_open());

		file << m_output.str().c_str() << std::endl;

		file.close();
	}

}