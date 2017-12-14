#ifndef  LEPTJSONPARSER_H_
#define  LEPTJSONPARSER_H_

#include "LeptJsonValue.h"
#include <sstream>

namespace leptjson {

	enum class LeptJsonParseRet
	{
		LEPT_JSON_PARSE_SUCCESS,
		LEPT_JSON_PARSE_INVALID_LITERAL,
		LEPT_JSON_PARSE_INVALID_NUMBER,
		LEPT_JSON_PARSE_INVALID_STRING,
		LEPT_JSON_PARSE_INVALID_ARRAY,
		LEPT_JSON_PARSE_INVALID_OBJECT,
		LEPT_JSON_PARSE_EXPECT_VALUE,
		LEPT_JSON_PARSE_NOT_SINGULAR
	};


	class LeptJsonParser 
	{

	public:
		LeptJsonParser(const char* json);
		std::istringstream m_input;
	public:
				
		Boolean TryMatchChar(std::istream& input, char ch);
		Boolean TryMatchString(std::istream& input, const String& src);
		Boolean Parse();

		LeptJsonParseRet Parse(std::istream& input, Value& value);

		LeptJsonParseRet ParseValue(std::istream& input, Value& value);

		LeptJsonParseRet ParseLiteral(std::istream& input, const char* literal, Value& value);

		LeptJsonParseRet ParseNumber(std::istream& input, Value& value);

		LeptJsonParseRet ParseString(std::istream& input, Value& value);

		LeptJsonParseRet ParseArray(std::istream& input, Value& value);

		LeptJsonParseRet ParseObject(std::istream& input, Value& value);

		LeptJsonParseRet ParseKey(std::istream& input, String& key);

	};

}

#endif