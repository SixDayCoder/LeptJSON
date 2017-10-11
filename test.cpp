
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "leptjson.h"

static int gMainReturn = 0;
static int gTestCount = 0;
static int gTestPass = 0;

//__FILE__ : 文件名
//__LINE__ : 行号

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
		do{\
			gTestCount++;\
			if (equality)\
				gTestPass++;\
			else{\
				fprintf(stderr, "In      %s\n", __FILE__);\
				fprintf(stderr, "Line    %d\n", __LINE__);\
				fprintf(stderr, "Expect  value is :  " format "\n", expect);\
				fprintf(stderr, "Actual  value is :  " format "\n", actual);\
				gMainReturn = -1; \
			}\
		}while(0)


#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE( (expect) == (actual), expect, actual, "%d") 

#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE( (expect) == (actual), expect, actual, "%.17g")


#define TEST_ERROR(error, json) \
		do{\
			LeptValue v;\
			v.type = LeptType:::LEPT_FALSE;\
			EXPECT_EQ_INT(error, LeptParse(json, v));\
			EXPECT_EQ_INT(LeptType::LEPT_NULL, v.type);\
		}while(0)


#define TEST_NUMBER(expect, json) \
		do{\
			LeptValue v;\
			EXPECT_EQ_INT(ParseState::LEPT_PARSE_SUCCESS, LeptParse(json, v));\
			EXPECT_EQ_INT(LeptType::LEPT_NUMBER, v.type);\
			EXPECT_EQ_DOUBLE(expect, LeptGetNumber(v));\
		}while(0)


static void TestParseNull() 
{
	LeptValue v;

	v.type = LeptType::LEPT_FALSE;

	EXPECT_EQ_INT(ParseState::LEPT_PARSE_SUCCESS, LeptParse("null", v));
	EXPECT_EQ_INT(LeptType::LEPT_NULL, v.type);

}

static void TestParseTrue()
{
	LeptValue v;

	v.type = LeptType::LEPT_FALSE;

	EXPECT_EQ_INT(ParseState::LEPT_PARSE_SUCCESS, LeptParse("true", v) );
	EXPECT_EQ_INT(LeptType::LEPT_TRUE, v.type);
}

static void TestParseFalse()
{
	LeptValue v;

	v.type = LeptType::LEPT_NULL;

	EXPECT_EQ_INT(ParseState::LEPT_PARSE_SUCCESS, LeptParse("false", v));
	EXPECT_EQ_INT(LeptType::LEPT_FALSE, v.type);
}

static void TestParseNotSingular()
{

	LeptValue v;

	v.type = LeptType::LEPT_FALSE;

	EXPECT_EQ_INT(ParseState::LEPT_PARSE_ROOT_NOT_SINGULAR, LeptParse("  false  true ", v) );
	EXPECT_EQ_INT(LeptType::LEPT_NOT_SINGULAR, v.type);

}

static void TestParseExpectValue()
{

	LeptValue v;

	v.type = LeptType::LEPT_FALSE;

	EXPECT_EQ_INT(ParseState::LEPT_PARSE_EXPECT_VALUE, LeptParse("", v));
	EXPECT_EQ_INT(LeptType::LEPT_NULL, v.type);

}

static void TestParseInvalidValue()
{

	LeptValue v1;

	v1.type = LeptType::LEPT_FALSE;

	EXPECT_EQ_INT(ParseState::LEPT_PARSE_INVALID_VALUE, LeptParse("nul", v1));
	EXPECT_EQ_INT(LeptType::LEPT_NULL, v1.type);

	LeptValue v2;

	v2.type = LeptType::LEPT_FALSE;

	EXPECT_EQ_INT(ParseState::LEPT_PARSE_INVALID_VALUE, LeptParse("nul", v2));
	EXPECT_EQ_INT(LeptType::LEPT_NULL, v2.type);

}

static void TestParseNumber() {

	TEST_NUMBER(0.0, "0");
	TEST_NUMBER(0.0, "-0");
	TEST_NUMBER(0.0, "-0.0");
	TEST_NUMBER(1.0, "1");
	TEST_NUMBER(-1.0, "-1");
	TEST_NUMBER(1.5, "1.5");
	TEST_NUMBER(-1.5, "-1.5");
	TEST_NUMBER(3.1416, "3.1416");
	TEST_NUMBER(1E10, "1E10");
	TEST_NUMBER(1e10, "1e10");
	TEST_NUMBER(1E+10, "1E+10");
	TEST_NUMBER(1E-10, "1E-10");
	TEST_NUMBER(-1E10, "-1E10");
	TEST_NUMBER(-1e10, "-1e10");
	TEST_NUMBER(-1E+10, "-1E+10");
	TEST_NUMBER(-1E-10, "-1E-10");
	TEST_NUMBER(1.234E+10, "1.234E+10");
	TEST_NUMBER(1.234E-10, "1.234E-10");
	TEST_NUMBER(0.0, "1e-10000"); /* must underflow */

}

static void Test()
{
	TestParseNull();

	TestParseTrue();

	TestParseFalse();

	TestParseNotSingular();

	TestParseExpectValue();

	TestParseInvalidValue();

	TestParseNumber();
}

int main()
{

	Test();

	printf("\nTest Result : \n\n");
	printf("%d / %d (%3.2f%%) passed\n", gTestPass, gTestCount, gTestPass * 100.0 / gTestCount);
	
	return gMainReturn;
}