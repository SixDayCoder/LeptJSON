#ifndef  LEPTJSON_H_
#define  LEPTJSON_H_

#include "LeptJsonDefine.h"
#include <assert.h>

class LeptJsonParser 
{
private:
	//解析后的值
	LeptValue m_LeptValue;
	//解析器的上下文
	LeptContext m_LeptContext;

public:
	//解析 空格 Tab 回车
	LeptParseRet LeptParseWhiteSpace();

	//解析json的值
	LeptParseRet LeptParseValue();

	//解析字面值 包括null false true
	LeptParseRet LeptParseLiteral(const char* literal);



public:
	LeptJsonParser() {}
	LeptJsonParser(const LeptContext& context);
	LeptParseRet Parse();

	LeptValue GetLeptValue()const {
		return m_LeptValue;
	}
	void ChangeLeptValueType(LeptType type) {
		m_LeptValue.type = type;
	}

	LeptContext GetLeptContext() {
		return m_LeptContext;
	}
	void ChangeLeptContextJson(const char* json) {
		m_LeptContext.json = json;
	}

};

#endif // ! LEPTJSON_H_
