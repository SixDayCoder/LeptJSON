#include "LeptJsonParser.h"
#include <iostream>
#include <fstream>

using namespace leptjson;

int main() 
{	
	
	LeptJsonParser parser1("{\"   title  \":\"windows\", \"   arr\":[123,[12, 23, 34],null,[],\"string\"],\"num  \":1234, \"  liter\":false, \"  obj\":{\"inter\" : 789} }");

	if (parser1.Parse()) {

		const Value& value = parser1.JsonValue();
		const Object& json = value.GetObject();

		printf("parser1 : xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

		for (auto it = json.begin(); it != json.end(); ++it) {

			const String& key = it->first;
			const Value& value = *it->second;


			printf("key is : %s\n", key.c_str());
		}

	}

	std::ifstream file1("example1.txt");
	LeptJsonParser parser2(file1);
	if (parser2.Parse()) {

		const Value& value = parser2.JsonValue();
		const Object& json = value.GetObject();

		printf("parser2 : xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

		for (auto it = json.begin(); it != json.end(); ++it) {

			const String& key = it->first;
			const Value& value = *it->second;


			printf("key is : %s\n", key.c_str());
		}

	}

	std::ifstream file2("example2.txt");
	LeptJsonParser parser3(file2);
	if (parser3.Parse()) {

		const Value& value = parser3.JsonValue();
		const Object& json = value.GetObject();

		printf("parser3 : xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

		for (auto it = json.begin(); it != json.end(); ++it) {

			const String& key = it->first;
			const Value& value = *it->second;


			printf("key is : %s\n", key.c_str());
		}

	}
	return 0;
}