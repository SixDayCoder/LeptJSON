#include "LeptJsonParser.h"
#include "UnitTest.h"
#include <iostream>
#include <fstream>
#include <queue>

using namespace leptjson;

int main() 
{	
	/*
	const char* text = "{\"   title  \":\"windows\", \"empty\":[], \"   arr\":[123,[12, 23, 34],null,[] , {}, \"string\"],\"num  \":1234, \"  liter\":false, \"  obj\":{\"inter\" : 789} }";
	LeptJsonReader reader;
	reader.LoadFromString(text);
	//LeptJsonReader reader;
	//reader.LoadFromFile("example2.txt");
	reader.Parse();
	if (LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS == reader.ParseState()) {

		LeptJsonValue& value = reader.ParseResult();

		if (value.IsHaveKeys()) {
			Object& obj = value.GetObject();
			std::cout << obj << std::endl;
			const LeptJsonValuePtr& val = obj["title"];
			//auto val = obj["empty"];
			//std::cout << *val << std::endl;
		}
		else {
			std::cout << reader.ParseResult() << std::endl;
		}

	}
	*/

	//UnitTest::Instance().TestAll();
	std::istringstream input("nul");
	std::queue<char> buffer;
	while (!buffer.empty())
		buffer.pop();

	char ch;
	std::string src("null");
	size_t len = src.length();
	size_t cnt = 0;
	bool bSuccess = true;

	while (input && !input.eof() && len > 0) {
		input.get(ch);
		std::cout << ch;
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
	std::cout << std::endl;
	while (!buffer.empty()) {
		std::cout << buffer.front();
		buffer.pop();
	}
	std::cout << std::endl;

	return 0;
}