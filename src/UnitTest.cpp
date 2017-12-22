#include "UnitTest.h"

namespace leptjson {

	size_t UnitTest::sTotalTestCount = 0;
	size_t UnitTest::sPassTestCount = 0;


	void UnitTest::TestAll()
	{
		std::cout << std::endl;
		TestParseAll();
		std::cout << std::endl;
		TestGenerateAll();
		std::cout << std::endl;
	}

	void UnitTest::TestGenerateAll()
	{
	}

	void UnitTest::TestParseAll()
	{
		TestParseLiteral();

		TestParseNumber();

		TestParseString();

		TestParseArray();

		TestParseObject();

		TestParseInvalid();

		printf("Test Parser : %d/%d (%3.2f%%) passed\n", sPassTestCount, sTotalTestCount, sPassTestCount * 100.0 / sTotalTestCount);
	}

	void UnitTest::TestParseLiteral()
	{
		TestParseLiteralCase("  null ");

		TestParseLiteralCase("null");

		TestParseLiteralCase("\n null");

		TestParseLiteralCase("  false   ");

		TestParseLiteralCase("false");

		TestParseLiteralCase("\n false");

		TestParseLiteralCase("  true  ");

		TestParseLiteralCase("true");

		TestParseLiteralCase("\n true");
	}

	void UnitTest::TestParseLiteralCase(const char * json)
	{
		LeptJsonReader reader;
		reader.LoadFromString(json);
		reader.Parse();
		ExpectEqActual(LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS, reader.ParseState());
		LeptJsonValue& value = reader.ParseResult();
		if (value.IsNull()) {
			ExpectEqActual(LeptJsonType::LEPT_JSON_NULL, value.GetType());
		}
		else if (value.IsTrue()) {
			ExpectEqActual(LeptJsonType::LEPT_JSON_TRUE, value.GetType());
		}
		else if (value.IsFalse()) {
			ExpectEqActual(LeptJsonType::LEPT_JSON_FALSE, value.GetType());
		}
	}

	void UnitTest::TestParseNumber()
	{
		TestParseNumberCase(0.0, "0");

		TestParseNumberCase(0.0, "-0");

		TestParseNumberCase(0.0, "-0.0");

		TestParseNumberCase(1.0, "1");

		TestParseNumberCase(-1.0, "-1");

		TestParseNumberCase(1.5, "1.5");

		TestParseNumberCase(-1.5, "-1.5");

		TestParseNumberCase(3.1416, "3.1416");

		TestParseNumberCase(1E10, "1E10");

		TestParseNumberCase(1e10, "1e10");

		TestParseNumberCase(1E+10, "1E+10");

		TestParseNumberCase(1E-10, "1E-10");

		TestParseNumberCase(-1E10, "-1E10");

		TestParseNumberCase(-1e10, "-1e10");

		TestParseNumberCase(-1E+10, "-1E+10");

		TestParseNumberCase(-1E-10, "-1E-10");

		TestParseNumberCase(1.234E+10, "1.234E+10");

		TestParseNumberCase(1.234E-10, "1.234E-10");

		TestParseNumberCase(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */

		TestParseNumberCase(4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */

		TestParseNumberCase(-4.9406564584124654e-324, "-4.9406564584124654e-324");

		TestParseNumberCase(2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */

		TestParseNumberCase(-2.2250738585072009e-308, "-2.2250738585072009e-308");

		TestParseNumberCase(2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */

		TestParseNumberCase(-2.2250738585072014e-308, "-2.2250738585072014e-308");

		TestParseNumberCase(1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */

		TestParseNumberCase(-1.7976931348623157e+308, "-1.7976931348623157e+308");

	}

	void UnitTest::TestParseNumberCase(Number expect, const char * json)
	{
		LeptJsonReader reader; 
		reader.LoadFromString(json); 
		reader.Parse(); 
		ExpectEqActual(LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS, reader.ParseState());
		LeptJsonValue& value = reader.ParseResult(); 
		ExpectEqActual(LeptJsonType::LEPT_JSON_NUMBER, value.GetType());
		Number number = value.GetNumber();
		ExpectEqActual(expect, number);
	}

	void UnitTest::TestParseString()
	{

		TestParseStringCase("", "\"\"");

		TestParseStringCase("Hello", "\"Hello\"");

		TestParseStringCase("Hello\nWorld", "\"Hello\\nWorld\"");

		TestParseStringCase("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
	}

	void UnitTest::TestParseStringCase(const String & expect, const char * json)
	{
		LeptJsonReader reader;
		reader.LoadFromString(json);
		reader.Parse();
		ExpectEqActual(LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS, reader.ParseState());
		LeptJsonValue& value = reader.ParseResult();
		ExpectEqActual(LeptJsonType::LEPT_JSON_STRING, value.GetType());
		String str = value.GetString();
		ExpectEqActual(expect, str);
	}

	void UnitTest::TestParseArray()
	{
		TestParseArrayCase("[]");

		TestParseArrayCase("[123, 456, 78, [123,4,\"xxxx\", null], \"ok\"]");

		TestParseArrayCase("[123,[1, 2, 3, [4, 5, 6]]]");
	}

	void UnitTest::TestParseArrayCase(const char * json)
	{
		LeptJsonReader reader;
		reader.LoadFromString(json);
		reader.Parse();
		ExpectEqActual(LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS, reader.ParseState());
		LeptJsonValue& value = reader.ParseResult();
		ExpectEqActual(LeptJsonType::LEPT_JSON_ARRAY, value.GetType());
		//Array& vec = value.GetArray();
		//std::cout << vec << std::endl;
	}

	void UnitTest::TestParseObject()
	{
		TestParseObjectCase("{}");

		TestParseObjectCase("{ \"a\" : \"1234\", \"ooo\" : { \"aa\" : 123, \"bbb\" : null, \"ccc\" : {} } }");

		const char *testcase =
			" { "

			"\"n\" : null , "

			"\"f\" : false , "

			"\"t\" : true , "

			"\"i\" : 123 , "

			"\"s\" : \"abc\", "

			"\"a\" : [ 1, 2, 3 ],"

			"\"o\" : { \"1\" : 1, \"2\" : 2, \"3\" : 3 }"

			" } ";

		LeptJsonReader reader;
		reader.LoadFromString(testcase);
		reader.Parse();
		ExpectEqActual(LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS, reader.ParseState());
		LeptJsonValue& value = reader.ParseResult();
		ExpectEqActual(LeptJsonType::LEPT_JSON_OBJECT, value.GetType());

		Object& obj = value.GetObject();

		ExpectEqActual(LeptJsonType::LEPT_JSON_NULL, obj["n"]->GetType());
		ExpectEqActual(String("null"), obj["n"]->GetNull());

		ExpectEqActual(LeptJsonType::LEPT_JSON_FALSE, obj["f"]->GetType());
		ExpectEqActual(String("false"), obj["f"]->GetFalse());

		ExpectEqActual(LeptJsonType::LEPT_JSON_TRUE, obj["t"]->GetType());
		ExpectEqActual(String("true"), obj["t"]->GetTrue());

		ExpectEqActual(LeptJsonType::LEPT_JSON_NUMBER, obj["i"]->GetType());
		ExpectEqActual(123.0, obj["i"]->GetNumber());

		ExpectEqActual(LeptJsonType::LEPT_JSON_STRING, obj["s"]->GetType());
		ExpectEqActual(String("abc"), obj["s"]->GetString());

		ExpectEqActual(LeptJsonType::LEPT_JSON_ARRAY, obj["a"]->GetType());
		Array& vec = obj["a"]->GetArray();
		for (size_t i = 0; i < 3; ++i) {
			LeptJsonValuePtr& p = vec[i];
			ExpectEqActual(static_cast<double>(i + 1), p->GetNumber());
		}

		ExpectEqActual(LeptJsonType::LEPT_JSON_OBJECT, obj["o"]->GetType());
		Object& o = obj["o"]->GetObject();
		size_t count = 1;
		for (auto it = o.begin(); it != o.end(); it++){
			auto& key = it->first;
			auto& val = it->second;
			ExpectEqActual(std::to_string(count), key);
			ExpectEqActual(static_cast<double>(count), val->GetNumber());
			count++;
		}
	}

	void UnitTest::TestParseObjectCase(const char * json)
	{
		LeptJsonReader reader;
		reader.LoadFromString(json);
		reader.Parse();
		ExpectEqActual(LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS, reader.ParseState());
		LeptJsonValue& value = reader.ParseResult();
		ExpectEqActual(LeptJsonType::LEPT_JSON_OBJECT, value.GetType());
		//Object& obj = value.GetObject();
		//std::cout << obj << std::endl;
	}

	void UnitTest::TestParseInvalid()
	{
		/************************************************************************/
		/* 非法字面值                                                            */
		/************************************************************************/
		TestParseInvalidCase("nul");
		TestParseInvalidCase("nu");
		TestParseInvalidCase("fals");
		TestParseInvalidCase("fal se");
		TestParseInvalidCase("  trxe ");
		TestParseInvalidCase("tru");
		TestParseInvalidCase("hello");
		TestParseInvalidCase("?");
		TestParseInvalidCase("");

		/************************************************************************/
		/* 非法数字                                                              */
		/************************************************************************/
		TestParseInvalidCase("0.1 * 20");
		TestParseInvalidCase("+1 + -2");
		TestParseInvalidCase(".123xx"); /* at least one digit before '.' */
		TestParseInvalidCase("1.xx");   /* at least one digit after '.' */
		TestParseInvalidCase("inf");
		TestParseInvalidCase("1e309");

		/************************************************************************/
		/* 非法字符串                                                            */
		/************************************************************************/
		TestParseInvalidCase("\"\\v\"");
		TestParseInvalidCase("\"\\'\"");
		TestParseInvalidCase("\"\\0\"");
		TestParseInvalidCase("\"\\x12\"");
		TestParseInvalidCase("\"\x01\"");
		TestParseInvalidCase("\"\x1F\"");

		/************************************************************************/
		/* 非法数组                                                              */
		/************************************************************************/
		TestParseInvalidCase("[1");
		TestParseInvalidCase("[1}");
		TestParseInvalidCase("[1 2");
		TestParseInvalidCase("[[]");
		TestParseInvalidCase("[1,]");
		TestParseInvalidCase("[\"a\", nul]");
		TestParseInvalidCase("[\"a\" : nul]");

		/************************************************************************/
		/* 非法对象                                                              */
		/************************************************************************/
		TestParseInvalidCase("{:1,");
		TestParseInvalidCase("{1:1,");
		TestParseInvalidCase("{true:1,");
		TestParseInvalidCase("{false:1,");
		TestParseInvalidCase("{null:1,");
		TestParseInvalidCase("{[]:1,");
		TestParseInvalidCase("{{}:1,");
		TestParseInvalidCase("{\"a\":1,");
		TestParseInvalidCase("{\"a\"}");
		TestParseInvalidCase("{\"a\",\"b\"}");
		TestParseInvalidCase("{\"a\":1");
		TestParseInvalidCase("{\"a\":1]");
		TestParseInvalidCase("{\"a\":1 \"b\"");
		TestParseInvalidCase("{\"a\":{}");
	}

	void UnitTest::TestParseInvalidCase(const char* json)
	{
		LeptJsonReader reader;
		reader.LoadFromString(json);
		reader.Parse();
		ExpectEqActual(LeptJsonParseRet::LEPT_JSON_PARSE_FAIL, reader.ParseState());
		LeptJsonValue& value = reader.ParseResult();
		ExpectEqActual(LeptJsonType::LEPT_JSON_EMPTY, value.GetType());
	}

}