#include "LeptJsonParser.h"
#include "UnitTest.h"
#include <iostream>
#include <fstream>
#include <queue>

using namespace leptjson;

int main() 
{	
	/************************************************************************/
	/*parse的单元测试                                                        */
	/************************************************************************/
	std::cout << std::endl;
	UnitTest::Instance().TestParseAll();
	std::cout << std::endl;
	
	/************************************************************************/
	/* 代码                                                                 */
	/************************************************************************/
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
	
	return 0;
}