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

private: 
	bool IsDigit1To9(char ch) {
		return (ch >= '1' && ch <= '9');
	}

	bool IsDigit(char ch) {
		return (ch >= '0' && ch <= '9');
	}

public:
	//跳过空白字符
	void SkipWhiteSpace(LeptContext& context);

	//解析json的值
	LeptParseRet LeptParseValue(LeptContext& context, LeptValue& value);

	//解析字面值(null false true)
	LeptParseRet LeptParseLiteral(LeptContext& context, const char* literal, LeptValue& value);

	//解析数字
	LeptParseRet LeptParseNumber(LeptContext& context, LeptValue& value);

	//解析字符串
	LeptParseRet LeptParseString(LeptContext& context, LeptValue& value);

	//解析数组
	LeptParseRet LeptParseArray(LeptContext& context, LeptValue& value);

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
