#include "TestUnit.h"

//��ʼ����Ԫ������Ŀ
int32_t TestUnit::sTestCount = 0;
//��ʼ��ͨ����Ԫ���Ե���Ŀ
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

	//1.���Խ���null�Ƿ�ɹ�
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_OK, parser.Parse(), "%d");

	//2.���Խ���null�������Ƿ�ı�Ϊnull
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");
}

void TestUnit::TestParseTrue()
{
	LeptContext context;
	context.Init();
	context.json = "  true";
	LeptJsonParser parser(context);


	//1.���Խ���true�Ƿ�ɹ�
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_OK, parser.Parse(), "%d");

	//2.���Խ���null�������Ƿ�ı�Ϊtrue
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_TRUE, value.type, "%d");

}

void TestUnit::TestParseFalse()
{
	LeptContext context;
	context.Init();
	context.json = "  false  ";
	LeptJsonParser parser(context);

	
	//1.���Խ���false�Ƿ�ɹ�
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_OK, parser.Parse(), "%d");

	//2.���Խ���null�������Ƿ�ı�Ϊfalse
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_FALSE, value.type, "%d");
}

void TestUnit::TestParseValue()
{
	LeptContext context;
	context.Init();
	context.json = "";
	LeptJsonParser parser(context);

	//1.���Խ���ֵ�Ƿ�ɹ�
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_EXPCET_VALUE, parser.Parse(), "%d");
	//2.���Խ�����Ϻ��Ƿ��������Ϊnull
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");
}

void TestUnit::TestParseNotSingular()
{
	LeptContext context;
	context.Init();
	context.json = "  null x";
	LeptJsonParser parser(context);
	
	//1.���Խ���ֵ�Ƿ�ɹ�
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_ROOT_NOT_SINGULAR, parser.Parse(), "%d");
	//2.���Խ�����Ϻ��Ƿ��������Ϊnull
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

	//(1)�����jsonֵ nul
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(2)�����jsonֵ ?
	parser.ChangeLeptContextJson("??");
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(3)�����jsonֵ +0
	parser.ChangeLeptContextJson("+0");
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(4)�����jsonֵ +1
	parser.ChangeLeptContextJson("+1");
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(5)�����jsonֵ .123
	parser.ChangeLeptContextJson(".123");
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(6)�����jsonֵ 1.
	parser.ChangeLeptContextJson("1.");
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(7)�����jsonֵ inf
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

	//�Ƿ�ɹ���������
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_OK, parser.Parse(),"%d");

	LeptValue value = parser.GetLeptValue();
	//������Ϻ������Ƿ��ΪLEPT_NUMBER
	IsExpectEqActual(LeptType::LEPT_NUMBRE, value.type, "%d");
	//�������������Ƿ��ԭ�������
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
