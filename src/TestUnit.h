//#ifndef  TESTUNIT_H_
//#define  TESTUNIT_H_
//
//#include "LeptJson.h"
//#include <stdint.h>
//#include <stdio.h>
//#include <string.h>
//#include <string>
//
//
//
//class TestUnit 
//{
//private:
//
//	TestUnit() {}
//	TestUnit(const TestUnit&) {}
//	TestUnit& operator=(const TestUnit&) {}
//
//	static int32_t sTestCount;
//	static int32_t sTestPass;
//
//public:
//	static TestUnit& Instance();
//
//	void TestParseAll();
//
//	void TestPareNull();
//
//	void TestParseTrue();
//
//	void TestParseFalse();
//
//	void TestParseValue();
//
//	void TestParseNotSingular();
//
//	void TestParseInvalidValue();
//
//	void TestParseNumber(double n, const char* strNumber);
//
//	void TestParseAllNumber();
//
//	void TestParseString(const char* src, const char* json);
//
//	void TestParseAllString();
//	
//	void TestAccessString();
//
//	void TestParseArray();
//
//	void TestResult();
//
////类内模板函数,判断expect和actual是否相等
//private:
//
//	template<typename ElementType>
//	bool IsExpectEqActual(const ElementType& expect, const ElementType& actual, const char* format = nullptr) 
//	{
//		sTestCount++;
//		if (expect == actual) {
//			sTestPass++;
//			return true;
//		}
//		else {
//
//			std::string strFormat;
//			strFormat = std::string("%s:%d expect :") + std::string(format) +
//						std::string(" actual : ") + std::string(format);
//
//			fprintf(stderr, strFormat.c_str() , __FILE__, __LINE__, expect, actual);
//			fprintf(stderr, "\n");
//			return false;
//		}
//	}
//
//	//特化const char*版本
//	template<>
//	bool IsExpectEqActual(const char* const& expect, const char* const& actual, const char* format) {
//		sTestCount++;
//		if (strcmp(expect,actual) == 0) {
//			sTestPass++;
//			return true;
//		}
//		else {
//
//			std::string strFormat;
//			strFormat = std::string("%s:%d expect :") + std::string(format) +
//				std::string(" actual : ") + std::string(format);
//
//			fprintf(stderr, strFormat.c_str(), __FILE__, __LINE__, expect, actual);
//			fprintf(stderr, "\n");
//			return false;
//		}
//	}
//};
//
//
//#endif // ! TESTUNIT_H_
