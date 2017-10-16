
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


#define EXPECT_EQ_INT(expect, actual)    EXPECT_EQ_BASE( (expect) == (actual), expect, actual, "%d") 

#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE( (expect) == (actual), expect, actual, "%.17g")

#define EXPECT_EQ_STRING(expect, actual, len) \
		EXPECT_EQ_BASE(  (sizeof(expect) - 1 == len) &&  ( memcmp(expect, actual, len) == 0 ) , expect, actual, "%s")



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

	EXPECT_EQ_INT(ParseState::LEPT_PARSE_SUCCESS, LeptParse("true", v));
	EXPECT_EQ_INT(LeptType::LEPT_TRUE, v.type);
}

static void TestParseFalse()
{
	LeptValue v;

	v.type = LeptType::LEPT_NULL;

	EXPECT_EQ_INT(ParseState::LEPT_PARSE_SUCCESS, LeptParse("false", v));
	EXPECT_EQ_INT(LeptType::LEPT_FALSE, v.type);
}


#define TEST_NUMBER(expect, json) \
		do{\
			LeptValue v;\
			EXPECT_EQ_INT(ParseState::LEPT_PARSE_SUCCESS, LeptParse(json, v));\
			EXPECT_EQ_INT(LeptType::LEPT_NUMBER, v.type);\
			EXPECT_EQ_DOUBLE(expect, LeptGetNumber(v));\
		}while(0)


#define TEST_ERROR(parse_state, json) \
		do{\
			LeptValue v;\
			v.type = LeptType::LEPT_FALSE;\
			EXPECT_EQ_INT(parse_state, LeptParse(json, v));\
			EXPECT_EQ_INT(LeptType::LEPT_NULL, v.type);\
		}while(0)


static void TestParseNumber()
{

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


static void TestParseNotSingular()
{

	TEST_ERROR(ParseState::LEPT_PARSE_ROOT_NOT_SINGULAR, "  false  true");

#if 0

	/* invalid number */
	TEST_ERROR(ParseState::LEPT_PARSE_ROOT_NOT_SINGULAR, "0123"); /* after zero should be '.' or nothing */
	TEST_ERROR(ParseState::LEPT_PARSE_ROOT_NOT_SINGULAR, "0x0");
	TEST_ERROR(ParseState::LEPT_PARSE_ROOT_NOT_SINGULAR, "0x123");

#endif

}


static void TestParseExpectValue()
{

	TEST_ERROR(ParseState::LEPT_PARSE_EXPECT_VALUE, "");
	TEST_ERROR(ParseState::LEPT_PARSE_EXPECT_VALUE, " ");
}

static void TestParseInvalidValue()
{

	TEST_ERROR(ParseState::LEPT_PARSE_INVALID_VALUE, "nul");
	TEST_ERROR(ParseState::LEPT_PARSE_INVALID_VALUE, "fals");

#if 0

	/*invalid number*/
	TEST_ERROR(ParseState::LEPT_PARSE_INVALID_VALUE, "+0");
	TEST_ERROR(ParseState::LEPT_PARSE_INVALID_VALUE, "+1");
	TEST_ERROR(ParseState::LEPT_PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
	TEST_ERROR(ParseState::LEPT_PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
	TEST_ERROR(ParseState::LEPT_PARSE_INVALID_VALUE, "INF");
	TEST_ERROR(ParseState::LEPT_PARSE_INVALID_VALUE, "inf");
	TEST_ERROR(ParseState::LEPT_PARSE_INVALID_VALUE, "NAN");
	TEST_ERROR(ParseState::LEPT_PARSE_INVALID_VALUE, "nan");

#endif

}

static void TestParseNumberTooBig()
{

#if 0
	TEST_ERROR(ParseState::LEPT_PARSE_NUMBER_TOO_BIG, "1e309");
	TEST_ERROR(ParseState::LEPT_PARSE_NUMBER_TOO_BIG, "-1e309");
#endif

}

static void Test()
{
	TestParseNull();

	TestParseTrue();

	TestParseFalse();

	TestParseNotSingular();

	TestParseNumber();

	TestParseExpectValue();

	TestParseInvalidValue();

	TestParseNumberTooBig();
}

int main()
{

	Test();

	printf("\nTest Result : \n\n");
	printf("%d / %d (%3.2f%%) passed\n", gTestPass, gTestCount, gTestPass * 100.0 / gTestCount);

	getchar();
	return gMainReturn;
}