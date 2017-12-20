#include "LeptJsonParser.h"
#include <cstring>
#include <cassert>
#include <queue>
#include <iostream>

namespace leptjson {


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

	LeptJsonReader::~LeptJsonReader()
	{
		if (m_value != 0) {
			delete m_value;
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

		char ch;
		size_t len = src.length();
		size_t cnt = 0;
		bool bSuccess = true;

		while (input.good() && !input.eof() && len > 0){
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

	void LeptJsonReader::Parse()
	{
		if (m_value == 0) {
			m_value = new LeptJsonValue();
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
			//��������껹���ַ�
			if (!input.eof()) {
				input >> std::ws;
				if (!input.eof()) {
					value.SetType(LeptJsonType::LEPT_JSON_EMPTY);
					return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
				}
			}
		}
		return ret;
	}

	LeptJsonParseRet LeptJsonReader::ParseLeptJsonValue(std::istream & input, LeptJsonValue & value)
	{
		assert(input.good());
		input >> std::ws;
		//���ַ���
		if (input.eof()) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
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
					if (ret == LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
						value.SetType(LeptJsonType::LEPT_JSON_ARRAY);
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
					if (ret == LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
						value.SetType(LeptJsonType::LEPT_JSON_OBJECT);
					}
					return ret;
				}
				break;

				case '\"': return ParseString(input, value); break;
				default: {
					if (isalpha(ch)) 
						return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
					else
						return ParseNumber(input, value); break;
				}
			}
		}
	}

	LeptJsonParseRet LeptJsonReader::ParseLiteral(std::istream& input, const char* literal, LeptJsonValue& value)
	{
		if (!TryMatchString(input, literal)) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
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
			//fail֮��ǵ�clear
			input.clear();
			input.seekg(rollback);
			return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
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

		//��ͷ������"
		if (!TryMatchChar(input, delimiter)) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
		}

		ch = input.peek();
		if (ch == delimiter) {
			//����ǿ��ַ���""
			input.get(ch);
			value.SetType(LeptJsonType::LEPT_JSON_STRING);
			value.values.stringValue= new String("");
			return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
		}
		else {

			String buffer;
			while (!input.eof() && input.good()){
				input.get(ch);
				//ת���ַ�
				switch (ch){

					//�ɹ������ַ���
					case '\"': 
						value.SetType(LeptJsonType::LEPT_JSON_STRING);
						value.values.stringValue = new String(buffer);
						return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
					break;

					//����ת���ַ� 
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
							//�����ǲ��Ϸ���ת���ַ�����
							default: return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE; break;
						}
					break;

					//Ĭ���������ַ���buffer
					default: 
						if (static_cast<unsigned char>(ch) < 0x20) {
							return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
						}
						else{
							buffer.push_back(ch);
						}
					break;
				}
			}
		}
		return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
	}

	LeptJsonParseRet LeptJsonReader::ParseArray(std::istream & input, LeptJsonValue & value)
	{
		if (!TryMatchChar(input, '[')) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
		}

		char ch = '\0';
		ch = input.peek();
		if (TryMatchChar(input, ']')) {
			value.SetType(LeptJsonType::LEPT_JSON_ARRAY);
			value.values.arrayValue = new Array();
			return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
		}

		LeptJsonParseRet ret;

		do {
			LeptJsonValue* v = new LeptJsonValue();
			ret = ParseLeptJsonValue(input, *v);
			if (ret != LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
				delete v;
				break;
			}
			value.values.arrayValue->push_back(v);
		} while (TryMatchChar(input, ','));

		if (!TryMatchChar(input, ']')) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
		}

		return ret;
	}

	LeptJsonParseRet LeptJsonReader::ParseObject(std::istream & input, LeptJsonValue & value)
	{
		if (!TryMatchChar(input, '{')) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
		}
		char ch = '\0';
		ch = input.peek();
		if (TryMatchChar(input, '}')) {
			value.SetType(LeptJsonType::LEPT_JSON_OBJECT);
			value.values.objectValue = new Object();
			return LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS;
		}
		LeptJsonParseRet ret;
		do {
			String key;
			if (!ParseKey(input, key)) {
				ret = LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
				break;
			}

			if (!TryMatchChar(input, ':')) {
				ret = LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
				break;
			}

			LeptJsonValue *v = new LeptJsonValue();
			ret = ParseLeptJsonValue(input, *v);

			if (ret != LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS) {
				delete v;
				break;
			}
			value.isHaveKeys = true;
			value.values.objectValue->insert(std::make_pair(key, v));

		} while (TryMatchChar(input, ','));

		if (!TryMatchChar(input, '}')) {
			return LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE;
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

		//key��ͷ����������,key��ֵֻ���Ǵ�Сд��ĸ,�»���,����
		while (input.good() && !input.eof()){
			input.get(ch);
			//�ɹ������ַ���
			if (ch == delimiter) {
				//ȥ��buffer��β�Ŀո�,string.trim
				buffer.erase(0, buffer.find_first_not_of(" "));
				buffer.erase(buffer.find_last_not_of(' ') + 1);
				if (buffer.empty()) {
					return false;
				}
				//��ȷ����
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
		//ֱ����������û����delimiter,˵������ʧ��
		return false;
	}

}