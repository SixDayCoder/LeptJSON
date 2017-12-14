#include "LeptJsonParser.h"
#include <cstring>
#include <cassert>


namespace leptjson {

	LeptJsonParser::LeptJsonParser(const char* json)
	{
		String text(json);
		std::istringstream m_input(text);

		if (TryMatchChar(m_input, 'h')) {
			printf("xxx\n");
		}
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
		
	}

	void LeptJsonParser::Parse()
	{
		
	}

	LeptJsonParseRet LeptJsonParser::Parse(std::istream & input, Value & value)
	{
		return LeptJsonParseRet();
	}

	LeptJsonParseRet LeptJsonParser::ParseValue(std::istream & input, Value & value)
	{
		return LeptJsonParseRet();
	}
	LeptJsonParseRet LeptJsonParser::ParseLiteral(std::istream& input, const char* literal, Value& value)
	{
		return LeptJsonParseRet();
	}

	LeptJsonParseRet LeptJsonParser::ParseNumber(std::istream& input, Value& value)
	{
		return LeptJsonParseRet();
	}

	LeptJsonParseRet LeptJsonParser::ParseString(std::istream& input, Value& value)
	{
		return LeptJsonParseRet();
	}

}