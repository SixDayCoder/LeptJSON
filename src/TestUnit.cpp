#include "TestUnit.h"

//初始化单元测试数目
int32_t TestUnit::sTestCount = 0;
//初始化通过单元测试的数目
int32_t TestUnit::sTestPass = 0;

TestUnit& TestUnit::Instance()
{
	static TestUnit instance;
	return instance;
}

void TestUnit::TestParseAll()
{
	TestPareNull();

	TestParseFalse();

	TestParseTrue();

	TestParseValue();

	TestParseNotSingular();

	TestParseInvalidValue();

	TestParseAllNumber();
	
	TestParseAllString();

	TestAccessString();
}

void TestUnit::TestPareNull()
{
	LeptContext context;
	context.Init();
	context.json = "null";
	LeptJsonParser parser(context);

	//1.测试解析null是否成功
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_OK, parser.Parse(), "%d");

	//2.测试解析null后类型是否改变为null
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");
}

void TestUnit::TestParseTrue()
{
	LeptContext context;
	context.Init();
	context.json = "  true";
	LeptJsonParser parser(context);


	//1.测试解析true是否成功
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_OK, parser.Parse(), "%d");

	//2.测试解析null后类型是否改变为true
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_TRUE, value.type, "%d");

}

void TestUnit::TestParseFalse()
{
	LeptContext context;
	context.Init();
	context.json = "  false  ";
	LeptJsonParser parser(context);

	
	//1.测试解析false是否成功
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_OK, parser.Parse(), "%d");

	//2.测试解析null后类型是否改变为false
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_FALSE, value.type, "%d");
}

void TestUnit::TestParseValue()
{
	LeptContext context;
	context.Init();
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
	context.Init();
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
	context.Init();
	context.json = "  nul";
	LeptJsonParser parser(context);

	LeptValue value;
	value.Init();

	//(1)错误的json值 nul
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(2)错误的json值 ?
	parser.ChangeLeptContextJson("??");
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(3)错误的json值 +0
	parser.ChangeLeptContextJson("+0");
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(4)错误的json值 +1
	parser.ChangeLeptContextJson("+1");
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(5)错误的json值 .123
	parser.ChangeLeptContextJson(".123");
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(6)错误的json值 1.
	parser.ChangeLeptContextJson("1.");
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(7)错误的json值 inf
	parser.ChangeLeptContextJson("inf");
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");
}

void TestUnit::TestParseNumber(double n, const char* strNumber)
{
	LeptContext context;
	context.Init();
	context.json = strNumber;
	LeptJsonParser parser(context);

	//是否成功解析数字
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_OK, parser.Parse(),"%d");

	LeptValue value = parser.GetLeptValue();
	//解析完毕后类型是否变为LEPT_NUMBER
	IsExpectEqActual(LeptType::LEPT_NUMBRE, value.type, "%d");
	//解析完后的数字是否和原数字相等
	IsExpectEqActual(n, value.GetNumber(), "%.17g");
}

void TestUnit::TestParseAllNumber()
{
	TestParseNumber(0.0, "0");
	TestParseNumber(0.0, "-0");
	TestParseNumber(0.0, "-0.0");
	TestParseNumber(1.0, "1");
	TestParseNumber(-1.0, "-1");
	TestParseNumber(1.5, "1.5");
	TestParseNumber(-1.5, "-1.5");
	TestParseNumber(3.1416, "3.1416");
	TestParseNumber(1E10, "1E10");
	TestParseNumber(1e10, "1e10");
	TestParseNumber(1E+10, "1E+10");
	TestParseNumber(1E-10, "1E-10");
	TestParseNumber(-1E10, "-1E10");
	TestParseNumber(-1e10, "-1e10");
	TestParseNumber(-1E+10, "-1E+10");
	TestParseNumber(-1E-10, "-1E-10");
	TestParseNumber(1.234E+10, "1.234E+10");
	TestParseNumber(1.234E-10, "1.234E-10");
	TestParseNumber(0.0, "1e-10000"); /* must underflow */
	TestParseNumber(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
	TestParseNumber(4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
	TestParseNumber(-4.9406564584124654e-324, "-4.9406564584124654e-324");
	TestParseNumber(2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
	TestParseNumber(-2.2250738585072009e-308, "-2.2250738585072009e-308");
	TestParseNumber(2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
	TestParseNumber(-2.2250738585072014e-308, "-2.2250738585072014e-308");
	TestParseNumber(1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
	TestParseNumber(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

void TestUnit::TestParseString(const char* src, const char* json)
{
	LeptContext context;
	context.Init();
	context.json = json;
	LeptJsonParser parser(context);

	LeptValue value;

	IsExpectEqActual(LeptParseRet::LEPT_PARSE_OK, parser.Parse(), "%d");
	value = parser.GetLeptValue();

	IsExpectEqActual(LeptType::LEPT_STRING, value.type, "%d");
	IsExpectEqActual(src, static_cast<const char*>(value.GetString().cstr), "%s");

}

void TestUnit::TestParseAllString()
{
	TestParseString("hello world", "\"hello world\"");
	TestParseString("", "\"\"");
	TestParseString("hello\n world", "\"hello\\n world\"");
	TestParseString("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");

	//invalid escape
	LeptContext context;
	context.Init();

	LeptValue value;
	value.Init();

	context.json = "\"\\v\"";
	LeptJsonParser parser(context);

	IsExpectEqActual(LeptParseRet::LEPT_PARSE_STRING_INVALID_ESCAPE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	parser.ChangeLeptContextJson("\"\\'\"");
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_STRING_INVALID_ESCAPE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	parser.ChangeLeptContextJson("\"\\0\"");
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_STRING_INVALID_ESCAPE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	parser.ChangeLeptContextJson("\"\\x12\"");
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_STRING_INVALID_ESCAPE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

}

void TestUnit::TestAccessString()
{
	LeptValue value;
	value.Init();
	value.SetString("", 0);
	IsExpectEqActual(static_cast<const char*>(""), static_cast<const char*>(value.str.cstr), "%s");
	value.SetString("hello", 5);
	IsExpectEqActual(static_cast<const char*>("hello"), static_cast<const char*>(value.str.cstr), "%s");
}

void TestUnit::TestResult()
{
	printf("\nTestResult : %d/%d (%3.2f%%) passed\n", sTestPass, sTestCount, sTestPass * 100.0 / sTestCount);
}
