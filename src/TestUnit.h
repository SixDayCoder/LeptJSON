#ifndef  TESTUNIT_H_
#define  TESTUNIT_H_

#include "LeptJson.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string>



class TestUnit 
{
private:

	TestUnit() {}
	TestUnit(const TestUnit&) {}
	TestUnit& operator=(const TestUnit&) {}

	static int32_t sTestCount;
	static int32_t sTestPass;

public:
	static TestUnit& Instance();

	void TestParseAll();

	void TestPareNull();

	void TestParseTrue();

	void TestParseFalse();

	void TestParseValue();

	void TestParseNotSingular();

	void TestParseInvalidValue();

	void TestParseNumber(double n, const char* strNumber);

	void TestParseAllNumber();

	void TestResult();

//����ģ�庯��,�ж�expect��actual�Ƿ����
private:

	template<typename ElementType>
	bool IsExpectEqActual(const ElementType& expect, const ElementType& actual, const char* format = nullptr) 
	{
		sTestCount++;
		if (expect == actual) {
			sTestPass++;
			return true;
		}
		else {

			std::string strFormat;
			strFormat = std::string("%s:%d expect :") + std::string(format) +
						std::string(" actual : ") + std::string(format);

			fprintf(stderr, strFormat.c_str() , __FILE__, __LINE__, expect, actual);
			fprintf(stderr, "\n");
			return false;
		}
	}
};


#endif // ! TESTUNIT_H_
