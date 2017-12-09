#include "TestUnit.h"

//��ʼ����Ԫ������Ŀ
int32_t TestUnit::sTestCount = 0;
//��ʼ��ͨ����Ԫ���Ե���Ŀ
int32_t TestUnit::sTestPass = 0;

TestUnit& TestUnit::Instance()
{
	static TestUnit instance;
	return instance;
	// TODO: �ڴ˴����� return ���
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
