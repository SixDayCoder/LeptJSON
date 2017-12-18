#include "LeptJsonParser.h"
#include <iostream>
#include <fstream>

using namespace leptjson;

int main() 
{	
	
	//LeptJsonReader parser1("{\"   title  \":\"windows\", \"   arr\":[123,[12, 23, 34],null,[],\"string\"],\"num  \":1234, \"  liter\":false, \"  obj\":{\"inter\" : 789} }");

	std::ifstream file("example1.txt");
	LeptJsonReader reader(file);
	if (reader.Parse()) {

		const LeptJsonValue& value = reader.ParseResult();

		if (value.IsHaveKeys()) {

			const Object& json = value.GetObject();

			for (auto it = json.begin(); it != json.end(); ++it) {

				const String& key = it->first;
				const LeptJsonValue& val = *it->second;


				printf("key is : %s\n", key.c_str());
			}
		}
	}

	return 0;
}