#include "UnitTest.h"

namespace leptjson {

	size_t UnitTest::sTotalTestCount = 0;
	size_t UnitTest::sPassTestCount = 0;


	void UnitTest::TestAll()
	{
		TestLiteral();

		TestNumber();

		TestString();

		TestArray();

		TestObject();

		TestInvalid();

		printf("%d/%d (%3.2f%%) passed\n", sPassTestCount, sTotalTestCount, sPassTestCount * 100.0 / sTotalTestCount);
	}

	void UnitTest::TestLiteral()
	{
		TestLiteralCase("  null ");

		TestLiteralCase("null");

		TestLiteralCase("\n null");

		TestLiteralCase("  false   ");

		TestLiteralCase("false");

		TestLiteralCase("\n false");

		TestLiteralCase("  true  ");

		TestLiteralCase("true");

		TestLiteralCase("\n true");
	}

	void UnitTest::TestLiteralCase(const char * json)
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

	void UnitTest::TestNumber()
	{
		TestNumberCase(0.0, "0");

		TestNumberCase(0.0, "-0");

		TestNumberCase(0.0, "-0.0");

		TestNumberCase(1.0, "1");

		TestNumberCase(-1.0, "-1");

		TestNumberCase(1.5, "1.5");

		TestNumberCase(-1.5, "-1.5");

		TestNumberCase(3.1416, "3.1416");

		TestNumberCase(1E10, "1E10");

		TestNumberCase(1e10, "1e10");

		TestNumberCase(1E+10, "1E+10");

		TestNumberCase(1E-10, "1E-10");

		TestNumberCase(-1E10, "-1E10");

		TestNumberCase(-1e10, "-1e10");

		TestNumberCase(-1E+10, "-1E+10");

		TestNumberCase(-1E-10, "-1E-10");

		TestNumberCase(1.234E+10, "1.234E+10");

		TestNumberCase(1.234E-10, "1.234E-10");

		TestNumberCase(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */

		TestNumberCase(4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */

		TestNumberCase(-4.9406564584124654e-324, "-4.9406564584124654e-324");

		TestNumberCase(2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */

		TestNumberCase(-2.2250738585072009e-308, "-2.2250738585072009e-308");

		TestNumberCase(2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */

		TestNumberCase(-2.2250738585072014e-308, "-2.2250738585072014e-308");

		TestNumberCase(1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */

		TestNumberCase(-1.7976931348623157e+308, "-1.7976931348623157e+308");

	}

	void UnitTest::TestNumberCase(Number expect, const char * json)
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

	void UnitTest::TestString()
	{

		TestStringCase("", "\"\"");

		TestStringCase("Hello", "\"Hello\"");

		TestStringCase("Hello\nWorld", "\"Hello\\nWorld\"");

		TestStringCase("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
	}

	void UnitTest::TestStringCase(const String & expect, const char * json)
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

	void UnitTest::TestArray()
	{
		TestArrayCase("[]");

		TestArrayCase("[123, 456, 78, [123,4,\"xxxx\", null], \"ok\"]");

		TestArrayCase("[123,[1, 2, 3, [4, 5, 6]]]");
	}

	void UnitTest::TestArrayCase(const char * json)
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

	void UnitTest::TestObject()
	{
		TestObjectCase("{}");

		TestObjectCase("{ \"a\" : \"1234\", \"ooo\" : { \"aa\" : 123, \"bbb\" : null, \"ccc\" : {} } }");

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

	void UnitTest::TestObjectCase(const char * json)
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

	void UnitTest::TestInvalid()
	{
		/************************************************************************/
		/* 非法字面值                                                            */
		/************************************************************************/
		//TestInvalidCase("nul");  TOFIX
		TestInvalidCase("nu");
		TestInvalidCase("fals");
		TestInvalidCase("fal se");
		TestInvalidCase("  trxe ");
		TestInvalidCase("tru");
		TestInvalidCase("hello");
		TestInvalidCase("?");
		TestInvalidCase("");

		/************************************************************************/
		/* 非法数字                                                              */
		/************************************************************************/
		TestInvalidCase("0.1 * 20");
		TestInvalidCase("+1 + -2");
		TestInvalidCase(".123xx"); /* at least one digit before '.' */
		TestInvalidCase("1.xx");   /* at least one digit after '.' */
		TestInvalidCase("inf");
		TestInvalidCase("1e309");

		/************************************************************************/
		/* 非法字符串                                                            */
		/************************************************************************/
		TestInvalidCase("\"\\v\"");
		TestInvalidCase("\"\\'\"");
		TestInvalidCase("\"\\0\"");
		TestInvalidCase("\"\\x12\"");
		TestInvalidCase("\"\x01\"");
		TestInvalidCase("\"\x1F\"");

		/************************************************************************/
		/* 非法数组                                                              */
		/************************************************************************/
		TestInvalidCase("[1");
		TestInvalidCase("[1}");
		TestInvalidCase("[1 2");
		TestInvalidCase("[[]");
		TestInvalidCase("[1,]");
		TestInvalidCase("[\"a\", nul]");
		TestInvalidCase("[\"a\" : nul]");

		/************************************************************************/
		/* 非法对象                                                              */
		/************************************************************************/
		TestInvalidCase("{:1,");
		TestInvalidCase("{1:1,");
		TestInvalidCase("{true:1,");
		TestInvalidCase("{false:1,");
		TestInvalidCase("{null:1,");
		TestInvalidCase("{[]:1,");
		TestInvalidCase("{{}:1,");
		TestInvalidCase("{\"a\":1,");
		TestInvalidCase("{\"a\"}");
		TestInvalidCase("{\"a\",\"b\"}");
		TestInvalidCase("{\"a\":1");
		TestInvalidCase("{\"a\":1]");
		TestInvalidCase("{\"a\":1 \"b\"");
		TestInvalidCase("{\"a\":{}");
	}

	void UnitTest::TestInvalidCase(const char* json)
	{
		LeptJsonReader reader;
		reader.LoadFromString(json);
		reader.Parse();
		ExpectEqActual(LeptJsonParseRet::LEPT_JSON_PARSE_INVALID_VALUE, reader.ParseState());
		LeptJsonValue& value = reader.ParseResult();
		ExpectEqActual(LeptJsonType::LEPT_JSON_EMPTY, value.GetType());
	}

}