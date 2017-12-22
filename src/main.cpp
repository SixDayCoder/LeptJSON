#include "LeptJsonParser.h"
#include "UnitTest.h"
#include <iostream>
#include <fstream>
#include <queue>

using namespace leptjson;

int main() 
{	

	UnitTest::Instance().TestAll();
	
	LeptJsonReader reader;
	reader.LoadFromFile("example1.txt");
	reader.Parse();
	if (LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS == reader.ParseState()) {
		LeptJsonValue& value = reader.ParseResult();
		std::cout << value << std::endl;
	}


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
		else {
			writer.Push(value);
		}
	}

	writer.Write("output.txt");

	return 0;
}