#include "TestUnit.h"


int main() 
{
	
	TestUnit::Instance().TestParseAll();

	TestUnit::Instance().TestResult();

	return 0;
}