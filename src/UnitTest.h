#ifndef LEPTJSON_UNITTEST_H_
#define  LEPTJSON_UNITTEST_H_

#include "LeptJsonParser.h"
#include <iostream>


namespace leptjson {

	class UnitTest 
	{
	/************************************************************************/
	/* 单例                                                                 */
	/************************************************************************/
	private:
		UnitTest() {}
		UnitTest(const UnitTest&) {}
		UnitTest& operator=(const UnitTest&) {}
	public:
		static UnitTest& Instance() 
		{
			static UnitTest instance;
			return instance;
		}
	/************************************************************************/
	/* 测试                                                                 */
	/************************************************************************/
	private:
		static size_t sTotalTestCount;
		static size_t sPassTestCount;
	public:

		void TestAll();

	private:
		/************************************************************************/
		/* 解析器单元测试                                                        */
		/************************************************************************/
		void TestParseAll();
		
		void TestParseLiteral();

		void TestParseLiteralCase(const char* json);

		void TestParseNumber();

		void TestParseNumberCase(Number expect, const char* json);

		void TestParseString();

		void TestParseStringCase(const String& expect, const char* json);

		void TestParseArray();

		void TestParseArrayCase(const char* json);

		void TestParseObject();

		void TestParseObjectCase(const char* json);

		void TestParseInvalid();

		void TestParseInvalidCase(const char* json);

		void TestGenerateAll();

		/************************************************************************/
		/* 生成器单元测试                                                        */
		/************************************************************************/

	private:

		template<typename ElementType>
		void ExpectEqActual(const ElementType& expect, const ElementType& actual)
		{
			sTotalTestCount++;
			if (expect == actual) {
				sPassTestCount++;
			}
			else {
				std::cout << "In file : " << __FILE__ << " In line : " << __LINE__;
				std::cout << " Expect : "  << expect << " Actual : " << actual << std::endl; \
			}
		}

		template<>
		void ExpectEqActual(const LeptJsonParseRet& expect, const LeptJsonParseRet& actual)
		{
			sTotalTestCount++;
			if (expect == actual) {
				sPassTestCount++;
			}
			else {
				std::cout << "In file : " << __FILE__ << " In line : " << __LINE__;
				std::cout << " Expect : " << static_cast<int>(expect) << " Actual : " << static_cast<int>(actual) << std::endl; \
			}
		}

		template<>
		void ExpectEqActual(const LeptJsonType& expect, const LeptJsonType& actual)
		{
			sTotalTestCount++;
			if (expect == actual) {
				sPassTestCount++;
			}
			else {
				std::cout << "In file : " << __FILE__ << " In line : " << __LINE__;
				std::cout << " Expect : " << static_cast<int>(expect) << " Actual : " << static_cast<int>(actual) << std::endl; \
			}
		}

	};

}

#endif // !LEPTJSON_UNITTEST_H_
