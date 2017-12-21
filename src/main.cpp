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
	//reader.LoadFromString(text);
	reader.LoadFromFile("example1.txt");
	reader.Parse();

	LeptJsonWriter writer;

	if (LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS == reader.ParseState()) {
		LeptJsonValue& value = reader.ParseResult();
		if (value.IsHaveKeys()) {
			Object& obj = value.GetObject();
			for (auto it = obj.begin(); it != obj.end(); ++it) {
				const String& key = it->first;
				const LeptJsonValue& val = *it->second;
				writer.Push(key, val);

			}
		}
	}

	writer.Write("output.txt");

	//if (LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS == reader.ParseState()) {
	//	LeptJsonValue& value = reader.ParseResult();
	//	if (value.IsHaveKeys()) {
	//		Object& obj = value.GetObject();
	//		std::cout << obj << std::endl;
	//	}
	//	else {
	//		std::cout << reader.ParseResult() << std::endl;
	//	}
	//}

	return 0;
}