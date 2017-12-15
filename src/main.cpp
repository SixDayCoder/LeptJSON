#include "LeptJsonParser.h"
#include <iostream>


int main() 
{	
	leptjson::LeptJsonParser parser("{\"   title  \":\"windows\", \"   arr\":[123,[12, 23, 34],null,[],\"string\"],\"num  \":1234, \"  liter\":false, \"  obj\":{\"inter\" : 789} }");

	if (parser.Parse()) {

		const leptjson::Value& value = parser.JsonValue();
		const leptjson::Object& json = value.GetObject();

		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

		for (auto it = json.begin(); it != json.end(); ++it) {

			const leptjson::String& key = it->first;
			const leptjson::Value& value = *it->second;

			printf("key is : %s\n", key.c_str());
		}

	}

	return 0;
}