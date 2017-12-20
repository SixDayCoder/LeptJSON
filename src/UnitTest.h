#ifndef LEPTJSON_UNITTEST_H_
#define  LEPTJSON_UNITTEST_H_

#include "LeptJsonParser.h"
#include <iostream>


namespace leptjson {

	class UnitTest 
	{
	/************************************************************************/
	/* µ¥Àý                                                                 */
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
	/* ²âÊÔ                                                                 */
	/************************************************************************/
	private:
		static size_t sTotalTestCount;
		static size_t sPassTestCount;
	public:
		void TestAll();
		
		void TestLiteral();

		void TestLiteralCase(const char* json);

		void TestNumber();

		void TestNumberCase(Number expect, const char* json);

		void TestString();

		void TestStringCase(const String& expect, const char* json);

		void TestArray();

		void TestArrayCase(const char* json);

		void TestObject();

		void TestObjectCase(const char* json);

		void TestInvalid();

		void TestInvalidCase(const char* json);

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
