#ifndef  LeptJsonReader_H_
#define  LeptJsonReader_H_

#include "LeptJsonValue.h"
#include <sstream>
#include <fstream>

namespace leptjson {

	enum class LeptJsonParseRet
	{
		LEPT_JSON_PARSE_SUCCESS,
		LEPT_JSON_PARSE_FAIL
	};

	enum class LeptJsonGenRet
	{
		LEPT_JSON_Stringify_SUCCESS,
		LEPT_JSON_Stringify_FAIL
	};

	/************************************************************************/
	/* 解析器                                                                */
	/************************************************************************/
	class LeptJsonReader 
	{

	private:

		std::istringstream m_input;
		LeptJsonValuePtr m_value;
		LeptJsonParseRet m_state;

	public:
		
		LeptJsonReader() {}
		~LeptJsonReader();

		void LoadFromString(const char* json);
		void LoadFromFile(const char* filepath);

		void Parse();

		const LeptJsonValue& ParseResult()const 
		{
			return *m_value;
		}

		LeptJsonValue& ParseResult()
		{
			return *m_value;
		}

		const LeptJsonParseRet ParseState()const {
			return m_state;
		}
		
	private:

		Boolean TryMatchChar(std::istream& input, char ch);

		Boolean TryMatchString(std::istream& input, const String& src);

		LeptJsonParseRet Parse(std::istream& input, LeptJsonValue& LeptJsonValue);

		LeptJsonParseRet ParseLeptJsonValue(std::istream& input, LeptJsonValue& LeptJsonValue);

		LeptJsonParseRet ParseLiteral(std::istream& input, const char* literal, LeptJsonValue& LeptJsonValue);

		LeptJsonParseRet ParseNumber(std::istream& input, LeptJsonValue& LeptJsonValue);

		LeptJsonParseRet ParseString(std::istream& input, LeptJsonValue& LeptJsonValue);

		LeptJsonParseRet ParseArray(std::istream& input, LeptJsonValue& LeptJsonValue);

		LeptJsonParseRet ParseObject(std::istream& input, LeptJsonValue& LeptJsonValue);

		Boolean ParseKey(std::istream& input, String& key);

	};
	

	/************************************************************************/
	/* 生成器                                                                */
	/************************************************************************/
	class LeptJsonWriter {
	private:

		LeptJsonValuePtr m_value;

		Boolean m_isStringifiied;

		std::ostringstream m_output;

	public:

		LeptJsonWriter() { m_isStringifiied = false; }
		~LeptJsonWriter();

		//字面值和String
		void Push(const String& key, const String& str);

		void Push(const String& key, Number number);
		
		void Push(const String& key, const Array& vec);

		void Push(const String& key, const Object& obj);

		void Stringify();
		
		void StringifyValue(std::ostream& output, const LeptJsonValue& val);

		void StringifyString(std::ostream& output, const String& val);

		void Write(const char* filepath);

	};
}

#endif