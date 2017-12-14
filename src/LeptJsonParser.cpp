#include "LeptJsonParser.h"
#include <cstring>
#include <cassert>
#include <queue>
#include <iostream>

namespace leptjson {

	LeptJsonParser::LeptJsonParser(const char* json) : m_input(std::istringstream(String(json)))
	{

	}

	Boolean LeptJsonParser::TryMatchChar(std::istream & input, char ch)
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

	Boolean LeptJsonParser::TryMatchString(std::istream & input, const String & src)
	{
		input >> std::ws;

		char ch;
		std::queue<char> buffer;
		while (!buffer.empty())
		{
			buffer.pop();
		}
		size_t len = src.length();
		size_t cnt = 0;
		bool bSuccess = true;

		while (input.good() && !input.eof() && len > 0)
		{
			input.get(ch);
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

	Boolean LeptJsonParser::Parse()
	{
		Value* value = new Value();
		LeptJsonParseRet ret = Parse(m_input, *value);
		if (ret != LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
			delete value;
			return false;
		}
		return true;
	}

	LeptJsonParseRet LeptJsonParser::Parse(std::istream & input, Value & value)
	{
		LeptJsonParseRet ret;
		ret = ParseValue(input, value);
		if (ret == LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
			//如果解析完还有字符
			if (!input.eof()) {
				input >> std::ws;
				if (!input.eof())
					return LeptJsonParseRet::LEPT_JSON_PARSE_NOT_SINGULAR;
			}
		}
		return ret;
	}

	LeptJsonParseRet LeptJsonParser::ParseValue(std::istream & input, Value & value)
	{
		assert(input.good());
		input >> std::ws;

		//std::ostringstream ostr;
		//ostr << input.rdbuf();
		//std::cout << ostr.str() << std::endl;

		//空字符串
		if (input.eof()) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_EXPECT_VALUE;
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
					value.values.arrayValue = new Array();
					ret = ParseArray(input, value);
					if (ret != LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
						delete value.values.arrayValue;
					}
					return ret;
				}
				break;
				case '{': {
					value.values.objectValue = new Object();
					ret = ParseObject(input, value);
					if (ret != LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
						delete value.values.objectValue;
					}
					return ret;
				}
				break;

				case '\"': return ParseString(input, value); break;
				default: return ParseNumber(input, value); break;
			}
		}
	}
	LeptJsonParseRet LeptJsonParser::ParseLiteral(std::istream& input, const char* literal, Value& value)
	{
		if (!TryMatchString(input, literal)) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_LITERAL;
		}
		else {

			if (literal == "null") {
				printf("parse null\n");
				value.SetType(LeptJsonType::LEPT_JSON_NULL);
			}
			else if (literal == "false") {
				printf("parse false\n");
				value.SetType(LeptJsonType::LEPT_JSON_FALSE);
			}
			else if (literal == "true") {
				printf("parse true\n");
				value.SetType(LeptJsonType::LEPT_JSON_TRUE);
			}
			else {

			}
		}
		return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
	}

	LeptJsonParseRet LeptJsonParser::ParseNumber(std::istream& input, Value& value)
	{
		std::streampos rollback = input.tellg();
		double number;
		input >> number;
		if (input.fail()) {
			//fail之后记得clear
			input.clear();
			input.seekg(rollback);
			return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_NUMBER;
		}
		else {
			value.SetType(LeptJsonType::LEPT_JSON_NUMBER);
			value.values.numberValue = number;
			printf("parse number : %.17g\n", value.values.numberValue);
			return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
		}
	}

	LeptJsonParseRet LeptJsonParser::ParseString(std::istream& input, Value& value)
	{
		//开头必须是"
		if (!TryMatchChar(input, '\"')) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_STRING;
		}

		char ch = '\0';
		char delimiter = '\"';

		ch = input.peek();
		if (ch == delimiter) {
			//如果是空字符串""
			input.get(ch);
			value.SetType(LeptJsonType::LEPT_JSON_STRING);
			value.values.stringValue = new String("\"\"");
			printf("parse string : %s\n", value.values.stringValue->c_str());
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
						value.values.stringValue = new String(buffer);
						printf("parse string : %s\n", value.values.stringValue->c_str());
						return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
					break;

					//碰到转义字符 
					case '\\': 
						input.get(ch);
						switch (ch){
							case '\\':
							case '/':
								buffer.push_back(ch);
							break;
							case 'b':buffer.push_back('\b'); break;
							case 'f':buffer.push_back('\f'); break;
							case 'n':buffer.push_back('\n'); break;
							case 'r':buffer.push_back('\r'); break;
							case 't':buffer.push_back('\t'); break;
							//TODO-Unicode
							//否则是不合法的转义字符序列
							default: return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_STRING; break;
						}
					break;

					//默认情况添加字符到buffer
					default: 
						if (static_cast<unsigned char>(ch) < 0x20) {
							return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_STRING;
						}
						else{
							buffer.push_back(ch);
						}
					break;
				}
			}
		}
		return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_STRING;
	}

	LeptJsonParseRet LeptJsonParser::ParseArray(std::istream & input, Value & value)
	{
		if (!TryMatchChar(input, '[')) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_ARRAY;
		}

		char ch = '\0';
		ch = input.peek();
		if (TryMatchChar(input, ']')) {
			value.SetType(LeptJsonType::LEPT_JSON_ARRAY);
			//Empty Array
			value.values.arrayValue = new Array();

			return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
		}

		LeptJsonParseRet ret;

		do {
			Value* v = new Value();
			ret = ParseValue(input, *v);

			if (ret != LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
				delete v;
				break;
			}
			value.values.arrayValue->push_back(v);
		} while (TryMatchChar(input, ','));

		if (!TryMatchChar(input, ']')) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_ARRAY;
		}

		return ret;
	}

	LeptJsonParseRet LeptJsonParser::ParseObject(std::istream & input, Value & value)
	{
		return LeptJsonParseRet();
	}

	LeptJsonParseRet LeptJsonParser::ParseKey(std::istream & input, String & key)
	{
		return LeptJsonParseRet();
	}

	//return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_STRING;
}