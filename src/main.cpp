#include "LeptJsonParser.h"
#include <iostream>
#include <fstream>

using namespace leptjson;

int main() 
{	
	const char* text = "{\"   title  \":\"windows\", \"   arr\":[123,[12, 23, 34],null,[] , {}, \"string\"],\"num  \":1234, \"  liter\":false, \"  obj\":{\"inter\" : 789} }";
	LeptJsonReader reader;
	reader.LoadFromString(text);
	//LeptJsonReader reader;
	//reader.LoadFromFile("example2.txt");
	if (reader.Parse()) {
		const LeptJsonValue& value = reader.ParseResult();

		if (value.IsHaveKeys()) {
			const Object& obj = value.GetObject();
			std::cout << obj << std::endl;
		}
		else {
			std::cout << reader.ParseResult() << std::endl;
		}
	}
	return 0;
}