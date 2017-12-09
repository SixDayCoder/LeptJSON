#ifndef  LEPTJSON_H_
#define  LEPTJSON_H_

#include "LeptJsonDefine.h"
#include <assert.h>

class LeptJsonParse 
{
private:
	//�������ֵ
	LeptValue m_LeptValue;
	//��������������
	LeptContext m_LeptContext;

public:
	//���� �ո� Tab �س�
	LeptParseRet LeptParseWhiteSpace(const char* json = nullptr);

	//��������ֵ ����null false true
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
