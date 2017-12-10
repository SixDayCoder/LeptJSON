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

public:
	//���� �ո� Tab �س�
	LeptParseRet LeptParseWhiteSpace();

	//����json��ֵ
	LeptParseRet LeptParseValue();

	//��������ֵ ����null false true
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
