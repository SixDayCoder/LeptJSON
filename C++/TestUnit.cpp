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

	//1.���Խ���null�Ƿ�ɹ�
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PASER_OK, parser.Parse(), "%d");

	//2.���Խ���null�������Ƿ�ı�Ϊnull
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");
}

void TestUnit::TestParseTrue()
{
	LeptContext context;
	context.json = "  true";
	LeptJsonParser parser(context);


	//1.���Խ���true�Ƿ�ɹ�
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PASER_OK, parser.Parse(), "%d");

	//2.���Խ���null�������Ƿ�ı�Ϊtrue
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_TRUE, value.type, "%d");

}

void TestUnit::TestParseFalse()
{
	LeptContext context;
	context.json = "  false  ";
	LeptJsonParser parser(context);

	
	//1.���Խ���false�Ƿ�ɹ�
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PASER_OK, parser.Parse(), "%d");

	//2.���Խ���null�������Ƿ�ı�Ϊfalse
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_FALSE, value.type, "%d");
}

void TestUnit::TestParseValue()
{
	LeptContext context;
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
	context.json = "  nul";
	LeptJsonParser parser(context);

	//(1)�����jsonֵ nul
	//1.���Խ���ֵ�Ƿ�ɹ�
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	//2.���Խ�����Ϻ��Ƿ��������Ϊnull
	LeptValue value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");

	//(2)�����jsonֵ ?
	parser.ChangeLeptContextJson("??");
	//1.���Խ���ֵ�Ƿ�ɹ�
	parser.ChangeLeptValueType(LeptType::LEPT_OBJECT);
	IsExpectEqActual(LeptParseRet::LEPT_PARSE_INVALID_VALUE, parser.Parse(), "%d");
	//2.���Խ�����Ϻ��Ƿ��������Ϊnull
	value = parser.GetLeptValue();
	IsExpectEqActual(LeptType::LEPT_NULL, value.type, "%d");
}

void TestUnit::TestResult()
{
	printf("\nTestResult : %d/%d (%3.2f%%) passed\n", sTestPass, sTestCount, sTestPass * 100.0 / sTestCount);
}
