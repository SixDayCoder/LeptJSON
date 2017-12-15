#include "TestUnit.h"
#include "LeptJsonParser.h"
//TO-DO 解析Unicode字符
#include <iostream>


void Test(std::istream& input)
{
	input >> std::ws;
	std::ostringstream ostr;
	ostr << input.rdbuf();
	std::cout << ostr.str() << std::endl;
}

void TestNumber(std::istream& input) 
{
	input >> std::ws;

	std::streampos rollback = input.tellg();
	double number;
	input >> number;
	if (input.fail()) {
		//fail之后记得clear
		input.clear();
		input.seekg(rollback);
		
	}
	else {
		printf("number : %.17f\n", number);
		std::ostringstream ostr;
		ostr << input.rdbuf();
		std::cout << ostr.str() << std::endl;
	}
}

int main() 
{
	//TestUnit::Instance().TestParseAll();
	//TestUnit::Instance().TestResult();

	
	leptjson::LeptJsonParser parser("[123, [12, 23, 34], null, true, false, [], \"string\"]");
	//leptjson::LeptJsonParser parser("[ 123,[],[],234]");

	if (parser.Parse()) {
		printf("1\n");
	}
	else {
		printf("2\n");
	}
	
	leptjson::LeptJsonParser parser2("{\"   title  \":\"windows\", \"   arr\":[123,[12, 23, 34],null,[],\"string\"],\"num  \":1234, \"  liter\":false, \"  obj\":{\"inter\" : 789} }");
	//leptjson::LeptJsonParser parser2("{ \"title\":\"windows\", \"obj:\":{\"xxxx\":123} }");
	if (parser2.Parse()) {
		printf("1\n");
	}
	else {
		printf("2\n");
	}

	//std::istringstream istr("  hello world");
	//Test(istr);

	//std::istringstream a("  555  ,[], 123");
	//TestNumber(a);


	return 0;
}