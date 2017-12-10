#include "TestUnit.h"

//初始化单元测试数目
int32_t TestUnit::sTestCount = 0;
//初始化通过单元测试的数目
int32_t TestUnit::sTestPass = 0;

TestUnit& TestUnit::Instance()
{
	static TestUnit instance;
	return instance;
	// TODO: 在此处插入 return 语句
}

void TestUnit::TestParseAll()
{
	TestPareNull();

	TestParseFalse();

	TestParseTrue();

	TestParseValue();

	TestParseNotSingular();

	TestParseInvalidValue();
}

void TestUnit::TestPareNull()
{
	LeptContext context;
	context.json = "null";
	LeptJsonParser parser(context);

	//1.测试解析null是否成功
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PASER_OK, parser.Parse(), "%d");

	//2.测试解析null后类型是否改变为null
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");
}

void TestUnit::TestParseTrue()
{
	LeptContext context;
	context.json = "  true";
	LeptJsonParser parser(context);


	//1.测试解析true是否成功
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PASER_OK, parser.Parse(), "%d");

	//2.测试解析null后类型是否改变为true
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_TRUE, value.type, "%d");

}

void TestUnit::TestParseFalse()
{
	LeptContext context;
	context.json = "  false  ";
	LeptJsonParser parser(context);

	
	//1.测试解析false是否成功
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PASER_OK, parser.Parse(), "%d");

	//2.测试解析null后类型是否改变为false
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_FALSE, value.type, "%d");
}

void TestUnit::TestParseValue()
{
	LeptContext context;
	context.json = "";
	LeptJsonParser parser(context);

	//1.测试解析值是否成功
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_EXPCET_VALUE, parser.Parse(), "%d");
	//2.测试解析完毕后是否更改类型为null
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");
}

void TestUnit::TestParseNotSingular()
{
	LeptContext context;
	context.json = "  null x";
	LeptJsonParser parser(context);
	
	//1.测试解析值是否成功
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_ROOT_NOT_SINGULAR, parser.Parse(), "%d");
	//2.测试解析完毕后是否更改类型为null
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");
}

void TestUnit::TestParseInvalidValue()
{
	LeptContext context;
	context.json = "  nul";
	LeptJsonParser parser(context);

	//(1)错误的json值 nul
	//1.测试解析值是否成功
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	//2.测试解析完毕后是否更改类型为null
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(2)错误的json值 ?
	parser.ChangeLeptContextJson("??");
	//1.测试解析值是否成功
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	//2.测试解析完毕后是否更改类型为null
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");
}

void TestUnit::TestResult()
{
	printf("\nTestResult : %d/%d (%3.2f%%) passed\n", sTestPass, sTestCount, sTestPass * 100.0 / sTestCount);
}
