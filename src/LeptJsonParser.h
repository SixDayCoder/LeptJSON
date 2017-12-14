#ifndef  LEPTJSONPARSER_H_
#define  LEPTJSONPARSER_H_

#include "LeptJsonValue.h"
#include <sstream>

namespace leptjson {

	enum class LeptJsonParseRet
	{
		LEPT_JSON_PARSE_SUCCESS,
		LEPT_JSON_PARSE_INVALID_VALUE
	};


	class LeptJsonParser 
	{

	public:
		LeptJsonParser(const char* json);
		std::istringstream m_input;
	public:
				
		Boolean TryMatchChar(std::istream& input, char ch);

		void Parse();

		LeptJsonParseRet Parse(std::istream& input, Value& value);

		LeptJsonParseRet ParseValue(std::istream& input, Value& value);

		LeptJsonParseRet ParseLiteral(std::istream& input, const char* literal, Value& value);

		LeptJsonParseRet ParseNumber(std::istream& input, Value& value);

		LeptJsonParseRet ParseString(std::istream& input, Value& value);

	};

}

#endif