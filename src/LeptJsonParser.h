#ifndef  LeptJsonReader_H_
#define  LeptJsonReader_H_

#include "LeptJsonValue.h"
#include <sstream>
#include <fstream>

namespace leptjson {

	enum class LeptJsonParseRet
	{
		LEPT_JSON_PARSE_SUCCESS,
		LEPT_JSON_PARSE_INVALID_LITERAL,
		LEPT_JSON_PARSE_INVALID_NUMBER,
		LEPT_JSON_PARSE_INVALID_STRING,
		LEPT_JSON_PARSE_INVALID_ARRAY,
		LEPT_JSON_PARSE_INVALID_OBJECT,
		LEPT_JSON_PARSE_EXPECT_LeptJsonValue,
		LEPT_JSON_PARSE_NOT_SINGULAR
	};


	class LeptJsonReader 
	{

	public:
		LeptJsonReader(const char* json);
		LeptJsonReader(std::ifstream& json);
		~LeptJsonReader();
		std::istringstream m_input;
		LeptJsonValuePtr m_value;
				
		Boolean TryMatchChar(std::istream& input, char ch);
		Boolean TryMatchString(std::istream& input, const String& src);
		Boolean Parse();

		LeptJsonParseRet Parse(std::istream& input, LeptJsonValue& LeptJsonValue);

		LeptJsonParseRet ParseLeptJsonValue(std::istream& input, LeptJsonValue& LeptJsonValue);

		LeptJsonParseRet ParseLiteral(std::istream& input, const char* literal, LeptJsonValue& LeptJsonValue);

		LeptJsonParseRet ParseNumber(std::istream& input, LeptJsonValue& LeptJsonValue);

		LeptJsonParseRet ParseString(std::istream& input, LeptJsonValue& LeptJsonValue);

		LeptJsonParseRet ParseArray(std::istream& input, LeptJsonValue& LeptJsonValue);

		LeptJsonParseRet ParseObject(std::istream& input, LeptJsonValue& LeptJsonValue);

		Boolean ParseKey(std::istream& input, String& key);

		Boolean IsValidKeyChar(char ch);

		const LeptJsonValue& ParseResult()const {
			return *m_value;
		}

	};

}

#endif