#ifndef  LeptJsonReader_H_
#define  LeptJsonReader_H_

#include "LeptJsonValue.h"
#include <sstream>
#include <fstream>

namespace leptjson {

	enum class LeptJsonParseRet
	{
		LEPT_JSON_PARSE_SUCCESS,
		LEPT_JSON_PARSE_INVALID_VALUE,
	};


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

}

#endif