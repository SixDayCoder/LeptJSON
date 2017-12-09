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

void TestUnit::TestPareNull()
{
	LeptJsonParse parse;

	IsExpectEqActual(LeptParseRet::LEPT_PASER_OK, parse.LeptParseLiteral("null", "null"), "%d");

	LeptValue value = parse.GetLeptValue();

	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");
}

void TestUnit::TestResult()
{
	printf("\nTestResult : %d/%d (%3.2f%%) passed\n", sTestPass, sTestCount, sTestPass * 100.0 / sTestCount);
}
