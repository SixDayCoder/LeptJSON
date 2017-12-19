#ifndef LEPTJSON_UNITTEST_H_
#define  LEPTJSON_UNITTEST_H_

#include "LeptJsonParser.h"

namespace leptjson {

	class UnitTest 
	{
	/************************************************************************/
	/* ����                                                                 */
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
	/* ����                                                                 */
	/************************************************************************/
	private:
		static size_t sTotalTestCount;
		static size_t sPassTestCount;

	};

}

#endif // !LEPTJSON_UNITTEST_H_
