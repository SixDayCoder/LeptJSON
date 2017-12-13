#ifndef  LEPTJSON_H_
#define  LEPTJSON_H_

#include "LeptJsonDefine.h"
#include <assert.h>

class LeptJsonParser 
{
private:
	//�������ֵ
	LeptValue m_LeptValue;
	//��������������
	LeptContext m_LeptContext;

private: 
	bool IsDigit1To9(char ch) {
		return (ch >= '1' && ch <= '9');
	}

	bool IsDigit(char ch) {
		return (ch >= '0' && ch <= '9');
	}

public:
	//�����հ��ַ�
	void SkipWhiteSpace(LeptContext& context);

	//����json��ֵ
	LeptParseRet LeptParseValue(LeptContext& context, LeptValue& value);

	//��������ֵ(null false true)
	LeptParseRet LeptParseLiteral(LeptContext& context, const char* literal, LeptValue& value);

	//��������
	LeptParseRet LeptParseNumber(LeptContext& context, LeptValue& value);

	//�����ַ���
	LeptParseRet LeptParseString(LeptContext& context, LeptValue& value);

	//��������
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
