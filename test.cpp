
#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leptjson.h"

static int gMainRet = 0;
static int gTestCount = 0;
static int gTestPass = 0;


//__FILE__ : 文件名
//__LINE__ : 行号

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        gTestCount++;\
        if (equality)\
            gTestPass++;\
        else {\
            fprintf(stderr, "In      %s\n", __FILE__);\
			fprintf(stderr, "Line    %d\n", __LINE__); \
			fprintf(stderr, "Expect  value is :  " format "\n", expect); \
			fprintf(stderr, "Actual  value is :  " format "\n", actual); \
			gMainRet = -1; \
        }\
    } while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")

#define EXPECT_EQ_STRING(expect, actual, alength) \
		EXPECT_EQ_BASE(sizeof(expect) - 1 == alength && memcmp(expect, actual, alength) == 0, expect, actual, "%s")

#define EXPECT_TRUE(actual)  EXPECT_EQ_BASE( (actual) != 0, "true", "false", "%s")

#define EXPECT_FALSE(actual) EXPECT_EQ_BASE( (actual) == 0, "false", "true", "%s")

static void TestParseNull() {
	LeptValue v;
	LeptInit(&v);
	LeptSetBoolean(&v, 0);
	EXPECT_EQ_INT(LeptParseSuccess, LeptParse(&v, "null"));
	EXPECT_EQ_INT(LEPT_NULL, LeptGetType(&v));
	LeptFree(&v);
}

static void TestParseTrue() {
	LeptValue v;
	LeptInit(&v);
	LeptSetBoolean(&v, 0);
	EXPECT_EQ_INT(LeptParseSuccess, LeptParse(&v, "true"));
	EXPECT_EQ_INT(LEPT_TRUE, LeptGetType(&v));
	LeptFree(&v);
}

static void TestParseFalse() {
	LeptValue v;
	LeptInit(&v);
	LeptSetBoolean(&v, 1);
	EXPECT_EQ_INT(LeptParseSuccess, LeptParse(&v, "false"));
	EXPECT_EQ_INT(LEPT_FALSE, LeptGetType(&v));
	LeptFree(&v);
}

#define TEST_NUMBER(expect, json)\
    do {\
        LeptValue v;\
        LeptInit(&v);\
        EXPECT_EQ_INT(LeptParseSuccess, LeptParse(&v, json));\
        EXPECT_EQ_INT(LEPT_NUMBER, LeptGetType(&v));\
        EXPECT_EQ_DOUBLE(expect, LeptGetNumber(&v));\
        LeptFree(&v);\
    } while(0)

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

	TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
	TEST_NUMBER(4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
	TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
	TEST_NUMBER(2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
	TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
	TEST_NUMBER(2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
	TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
	TEST_NUMBER(1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
	TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");

}

#define TEST_STRING(expect, json)\
    do {\
        LeptValue v;\
        LeptInit(&v);\
        EXPECT_EQ_INT(LeptParseSuccess, LeptParse(&v, json));\
        EXPECT_EQ_INT(LEPT_STRING, LeptGetType(&v));\
        EXPECT_EQ_STRING(expect, LeptGetString(&v), LeptGetStringLength(&v));\
        LeptFree(&v);\
    } while(0)

static void TestParseString() {

	TEST_STRING("", "\"\"");
	TEST_STRING("Hello", "\"Hello\"");
	TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
	TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");

}

#define TEST_ERROR(error, json)\
    do {\
        LeptValue v;\
        LeptInit(&v);\
        v.type = LEPT_FALSE;\
        EXPECT_EQ_INT(error, LeptParse(&v, json));\
        EXPECT_EQ_INT(LEPT_NULL, LeptGetType(&v));\
        LeptFree(&v);\
    } while(0)

static void TestParseExpectValue() {
	TEST_ERROR(LeptParse_EXPECT_VALUE, "");
	TEST_ERROR(LeptParse_EXPECT_VALUE, " ");
}

static void TestParseInvalidValue() {
	TEST_ERROR(LeptParse_INVALID_VALUE, "nul");
	TEST_ERROR(LeptParse_INVALID_VALUE, "?");

	/* invalid number */
	TEST_ERROR(LeptParse_INVALID_VALUE, "+0");
	TEST_ERROR(LeptParse_INVALID_VALUE, "+1");
	TEST_ERROR(LeptParse_INVALID_VALUE, ".123"); /* at least one digit before '.' */
	TEST_ERROR(LeptParse_INVALID_VALUE, "1.");   /* at least one digit after '.' */
	TEST_ERROR(LeptParse_INVALID_VALUE, "INF");
	TEST_ERROR(LeptParse_INVALID_VALUE, "inf");
	TEST_ERROR(LeptParse_INVALID_VALUE, "NAN");
	TEST_ERROR(LeptParse_INVALID_VALUE, "nan");
}

static void TestParseRootNotSingular() {
	TEST_ERROR(LeptParse_ROOT_NOT_SINGULAR, "null x");

	/* invalid number */
	TEST_ERROR(LeptParse_ROOT_NOT_SINGULAR, "0123"); /* after zero should be '.' or nothing */
	TEST_ERROR(LeptParse_ROOT_NOT_SINGULAR, "0x0");
	TEST_ERROR(LeptParse_ROOT_NOT_SINGULAR, "0x123");
}

static void TestParseNumberTooBig() {
	TEST_ERROR(LeptParseNumber_TOO_BIG, "1e309");
	TEST_ERROR(LeptParseNumber_TOO_BIG, "-1e309");
}

static void TestParseMissingQuotationMark() {
	TEST_ERROR(LeptParse_MISS_QUOTATION_MARK, "\"");
	TEST_ERROR(LeptParse_MISS_QUOTATION_MARK, "\"abc");
}

static void TestParseInvalidStringEscape() {
	TEST_ERROR(LeptParse_INVALID_STRING_ESCAPE, "\"\\v\"");
	TEST_ERROR(LeptParse_INVALID_STRING_ESCAPE, "\"\\'\"");
	TEST_ERROR(LeptParse_INVALID_STRING_ESCAPE, "\"\\0\"");
	TEST_ERROR(LeptParse_INVALID_STRING_ESCAPE, "\"\\x12\"");
}

static void TestParseInvalidStringChar() {
	TEST_ERROR(LeptParse_INVALID_STRING_CHAR, "\"\x01\"");
	TEST_ERROR(LeptParse_INVALID_STRING_CHAR, "\"\x1F\"");
}

static void TestAcessNull() {
	LeptValue v;
	LeptInit(&v);
	LeptSetString(&v, "a", 1);
	LeptFree(&v);
	EXPECT_EQ_INT(LEPT_NULL, LeptGetType(&v));
	LeptFree(&v);
}

static void TestAcessBoolean() {
	LeptValue v;
	LeptInit(&v);
	LeptSetString(&v, "a", 1);
	LeptSetBoolean(&v, 1);
	EXPECT_TRUE(LeptGetBoolean(&v));
	LeptSetBoolean(&v, 0);
	EXPECT_FALSE(LeptGetBoolean(&v));
	LeptFree(&v);
}

static void TestAcessNumber() {
	LeptValue v;
	LeptInit(&v);
	LeptSetString(&v, "a", 1);
	LeptSetNumber(&v, 1234.5);
	EXPECT_EQ_DOUBLE(1234.5, LeptGetNumber(&v));
	LeptFree(&v);
}

static void TestAcessString() {
	LeptValue v;
	LeptInit(&v);
	LeptSetString(&v, "", 0);
	EXPECT_EQ_STRING("", LeptGetString(&v), LeptGetStringLength(&v));
	LeptSetString(&v, "Hello", 5);
	EXPECT_EQ_STRING("Hello", LeptGetString(&v), LeptGetStringLength(&v));
	LeptFree(&v);
}

static void TestParse() {

	TestParseNull();
	TestParseTrue();
	TestParseFalse();
	TestParseNumber();
	TestParseString();
	TestParseExpectValue();
	TestParseInvalidValue();
	TestParseRootNotSingular();
	TestParseNumberTooBig();
	TestParseMissingQuotationMark();
	TestParseInvalidStringEscape();
	TestParseInvalidStringChar();

	TestAcessNull();
	TestAcessBoolean();
	TestAcessNumber();
	TestAcessString();
}

int main() {

#ifdef _WINDOWS
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	TestParse();
	printf("%d/%d (%3.2f%%) passed\n", gTestPass, gTestCount, gTestPass * 100.0 / gTestCount);

	getchar();
	return gMainRet;
}
