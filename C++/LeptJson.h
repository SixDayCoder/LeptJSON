#ifndef  LEPTJSON_H_
#define  LEPTJSON_H_

#include "LeptJsonDefine.h"
#include <assert.h>

class LeptJsonParse 
{
private:
	//解析后的值
	LeptValue m_LeptValue;
	//解析器的上下文
	LeptContext m_LeptContext;

public:
	//解析 空格 Tab 回车
	LeptParseRet LeptParseWhiteSpace(const char* json = nullptr);

	//解析字面值 包括null false true
	LeptParseRet LeptParseLiteral(const char* json, const char* literal);

public:
	LeptJsonParse() {}
	LeptJsonParse(const char* json);
	LeptParseRet Parse();

	LeptValue GetLeptValue()const {
		return m_LeptValue;
	}

	LeptContext GetLepContext() {
		return m_LeptContext;
	}


};

#endif // ! LEPTJSON_H_
